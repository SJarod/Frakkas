#include <imgui.h>
#include <ImGuizmo.h>
#include <string>
#include <functional>

#include "game/transform.hpp"

#include "renderer/light.hpp"
#include "renderer/graph.hpp"
#include "renderer/skeletal_model.hpp"

#include "animation/animation_graph.hpp"

#include "editor/editor_render.hpp"

#include "utils/dragdrop_constants.hpp"
#include "utils/platform.hpp"

#include "helpers/game_edit.hpp"

/**
 * @brief Useful function to receive resource path from a drag drop imgui event
 * @param i_extensions The list of extension that allowed.
 * @param func The event function to process if extension is found.
 * @return true if drop a
 */
inline bool DragDropTarget(const std::string& i_extensions, std::filesystem::path& o_path)
{
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Utils::ResourcePathDragDropID))
		{
			o_path = *static_cast<std::filesystem::path*>(payload->Data);
			std::string dropExtension = o_path.extension().string();
			if (Utils::FindExtension(dropExtension, i_extensions))
			{
				ImGui::EndDragDropTarget();
				return true;
			}
		}

		ImGui::EndDragDropTarget();
	}

	return false;
}

bool Helpers::Edit(std::string& io_string, const char* i_label)
{
	char strCopy[256] = "";

#ifdef KK_WINDOWS
	strcpy_s(strCopy, io_string.data());
#else
	strcpy(strCopy, io_string.data());
#endif
	bool returnValue = ImGui::InputText(i_label, strCopy, 255, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll);
	io_string = std::string(strCopy);

	// Editing text while item is activated
	if (ImGui::IsAnyItemActive())
		Editor::EditorRender::editingText = true;
	// Stop editing text when pressed enter
	else if (returnValue)
		Editor::EditorRender::editingText = false;

	return returnValue;
}

void Helpers::Edit(Game::Transform& io_transform)
{
	Game::Transform& trs = io_transform;
	Game::ScaleLockParams& scParams = trs.GetScaleLockParameters();

	ImGui::Text("Transform");

	ImGui::Spacing();

	Vector3 pos = trs.position;
	Vector3 rot = Maths::ToDegrees(trs.rotation);
	Vector3 sc = trs.scale;


	// Position edit
	ImGui::Text("Position");
	DragScalar("XYZ###P", pos.element, 3);

	ImGui::Text("Rotation");
	DragScalar("XYZ###R", rot.element, 3);

	rot = Maths::Modulo(rot, 360.f);
	if (trs.colliderComponentCount > 0) // X and Z rotation change if rot(Y) >= 90°. We prefer to prevent this.
		rot.y = Maths::Modulo(rot.y, 90.f);

#pragma region Scale edit
	ImGui::Text("Scale");

	if (scParams.isLock && scParams.origScale == Vector3::zero)
		scParams.ratio = 0.f;

	if (DragScalar("XYZ###S", sc.element, 3, 0.f, std::numeric_limits<float>::max(), 0.1f))
	{
		if (scParams.isLock)
		{
			if (sc.x != trs.scale.get().x && scParams.origScale.x != 0.f)
				scParams.ratio = sc.x / scParams.origScale.x;
			else if (sc.y != trs.scale.get().y && scParams.origScale.y != 0.f)
				scParams.ratio = sc.y / scParams.origScale.y;
			else if (scParams.origScale.z != 0.f)
				scParams.ratio = sc.z / scParams.origScale.z;
			else
				scParams.ratio = sc.x + sc.y + sc.z;
		}
	}

	if (scParams.isLock && scParams.origScale != Vector3::zero)
		sc = scParams.origScale * scParams.ratio;
	else if (scParams.isLock && scParams.origScale == Vector3::zero)
		sc = Vector3(scParams.ratio, scParams.ratio, scParams.ratio);

	ImGui::SameLine();
	ImGui::Checkbox("Lock scale", &scParams.isLock);
#pragma endregion

	trs.position = pos;
	trs.rotation = Maths::ToRadians(rot);
	trs.scale = sc;
}

void Helpers::Edit(Animation::AnimationGraph& io_animGraph)
{
	std::vector<std::string_view> names;
	io_animGraph.GetAnimationsName(names);

	ImGui::Text("\nAnimation graph");

	ImGuiTableFlags features = 0;
	ImGuiTableFlags decorations = 0;
	decorations |= ImGuiTableFlags_RowBg;
	decorations |= ImGuiTableFlags_BordersV;
	decorations |= ImGuiTableFlags_BordersH;

	if (ImGui::BeginTable("Available animations", 1, features | decorations))
	{
		bool openDeletePopup = false;
		static int selected = 0;

		for (int i = 0; i < names.size(); ++i)
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("%s", names[i].data());

			if (ImGui::IsItemHovered() && Game::Inputs::IsPressed(Game::EButton::MOUSE_RIGHT))
			{
				openDeletePopup = true;
				selected = i;
			}
		}

		if (openDeletePopup)
			ImGui::OpenPopup("DELETE");

		if (ImGui::BeginPopup("DELETE", ImGuiWindowFlags_AlwaysAutoResize))
		{
			if (ImGui::Selectable("Delete"))
			{
				io_animGraph.FindPackAndRemove(std::string(names[selected]));
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
		ImGui::TableNextColumn();

		{
			std::string buffer;
			if (Edit(buffer, "Drop to load a new animation file"))
				io_animGraph.AddAnimationPack(buffer);
		}
		{
			std::filesystem::path buffer;
			if (DragDropTarget(Utils::AnimationExtensions, buffer))
				io_animGraph.AddAnimationPack(buffer.string());
		}

		ImGui::EndTable();
	}
}

inline void DisplayBone(const Resources::SkeletonNodeData& i_bone, int i_id, Renderer::SkeletalModel& io_skmodel, bool& o_openPopup, int& o_selected)
{
	if (ImGui::TreeNodeEx(i_bone.name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::IsItemHovered() && Game::Inputs::IsPressed(Game::EButton::MOUSE_RIGHT))
		{
			o_openPopup = true;
			o_selected = i_id;
		}

		for (Renderer::Socket& s : io_skmodel.sockets)
		{
			if (s.boneID == i_id)
			{
				if (ImGui::TreeNodeEx(s.model.mesh.lock()->name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				{
                    ImGui::Checkbox("Render", &s.render);
					Helpers::Edit(s.transform);
					if (ImGui::Selectable("Remove socket"))
					{
						io_skmodel.RemoveSocket(s.boneID);
						ImGui::TreePop();
						break;
					}
					ImGui::TreePop();
				}
			}
		}

		for (const SkeletonNodeData& child : i_bone.children)
		{
			DisplayBone(child, io_skmodel.skmesh.lock()->boneInfoMap[child.name].id, io_skmodel, o_openPopup, o_selected);
		}

		ImGui::TreePop();
	}
}

void Helpers::DisplaySkeleton(Renderer::SkeletalModel& io_skmodel)
{
	bool openPopup = false;
	static int selected = -1;

	std::shared_ptr<SkeletalMesh> skmesh = io_skmodel.skmesh.lock();
	DisplayBone(skmesh->rootNode, skmesh->boneInfoMap[skmesh->rootNode.name].id, io_skmodel, openPopup, selected);

	if (openPopup)
		ImGui::OpenPopup("SOCKET");

	if (ImGui::BeginPopup("SOCKET", ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Add socket");
		ImGui::Separator();

		static std::string meshName;
		static std::string textureName;
		{
			Helpers::Edit(meshName, "mesh");
			Helpers::Edit(textureName, "texture");
		}
		{
			std::filesystem::path buff0;
			if (DragDropTarget(Utils::MeshExtensions, buff0))
				meshName = buff0.string();
			std::filesystem::path buff1;
			if (DragDropTarget(Utils::MeshExtensions, buff1))
				textureName = buff1.string();
		}
		if (ImGui::Selectable("Add model"))
		{
			Game::Transform t;
			io_skmodel.AddSocket(meshName, textureName, false, selected, t);
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void Helpers::Edit(Renderer::SkeletalModel& io_skmodel)
{
	if (ImGui::Button("Show skeleton bone hierarchy"))
		io_skmodel.showSkeleton = !io_skmodel.showSkeleton;

	if (io_skmodel.showSkeleton)
	{
		ImGui::Begin("Skeleton");

		DisplaySkeleton(io_skmodel);

		ImGui::End();
	}
}

void Helpers::Edit(Game::Entity& io_entity, ImGuizmo::OPERATION& i_guizmoOperation)
{
	int guizmoOperation = static_cast<int>(i_guizmoOperation);

	ImGui::RadioButton("None", &guizmoOperation, ImGuizmo::OPERATION::BOUNDS); // Bounds is not used, so equal to NONE

	ImGui::SameLine();
	ImGui::RadioButton("Translate", &guizmoOperation, ImGuizmo::OPERATION::TRANSLATE);

#ifdef ROTATION_GUIZMO
	ImGui::SameLine();
	ImGui::RadioButton("Rotate", &guizmoOperation, ImGuizmo::OPERATION::ROTATE);
#endif

	ImGui::SameLine();
	ImGui::RadioButton("Scale", &guizmoOperation, ImGuizmo::OPERATION::SCALE);

	i_guizmoOperation = static_cast<ImGuizmo::OPERATION>(guizmoOperation);

	ImGui::Separator();

	Edit(io_entity.name, "Name");

	ImGui::Separator();

#ifdef KK_WINDOWS
	ImGui::Text("ID : %llu", io_entity.GetID());
#else
	ImGui::Text("ID : %lu", io_entity.GetID());
#endif

	ImGui::SameLine();
	ImGui::Checkbox("Serialize", &io_entity.serialize);


	if (io_entity.parent && ImGui::Button("Unset parent"))
		io_entity.unsettingParent = true;

	// Edit transform
	Edit(io_entity.transform);

    /// EDIT COMPONENTS
    ImGui::Spacing();
    ImGui::Text("COMPONENTS");
    ImGui::Separator();

    bool enabled = io_entity.enabled;
    ImGui::Checkbox("Enable", &enabled);
    io_entity.enabled = enabled;

    int index = 0, removeIndex = -1;
    for (const std::unique_ptr<Game::Component>& comp : io_entity.components)
    {
        ImGui::PushID(index);

		bool compEnabled = comp->enabled;
		if (!Edit(reinterpret_cast<unsigned char*>(comp.get()), comp->GetMetaData(), compEnabled))
			removeIndex = index;
		comp->enabled = compEnabled;

		ImGui::PopID();
		index++;
	}

	if (removeIndex != -1)
		io_entity.RemoveComponentAt(removeIndex);

	ImGui::Spacing();
	ImGui::Separator();
	ImGui::Spacing();

	if (ImGui::Button("Add Component", ImVec2(200, 0)))
		ImGui::OpenPopup("Add component.");
}

void Helpers::Edit(Engine& io_engine, bool& o_showMap)
{
	Renderer::Light& light = io_engine.graph->light;
	ImGui::Checkbox("ToonShading", &light.toonShading);

	if (light.toonShading)
	{
		ImGui::SliderInt("Step amount", &light.stepAmount, 1, 20);
		ImGui::SliderFloat("Step size", &light.stepSize, 0.05f, 1.f);
		ImGui::SliderFloat("Spec size", &light.specSize, 0.f, 1.f);
		ImGui::Separator();
	}

	ImGui::Spacing();

	ImGui::ColorEdit3("Ambient", light.ambient.element);
	ImGui::ColorEdit3("Diffuse", light.diffuse.element);
	ImGui::ColorEdit3("Specular", light.specular.element);

	ImGui::Spacing();

	ImGui::SliderFloat3("Position", light.position.element, -1.f, 1.f);

	ImGui::Spacing();
	ImGui::Separator();

	ImGui::Checkbox("Cast shadows", &light.shadow);
	ImGui::DragInt("Shadow PCF", &light.shadowPCF, 1.f, 0.f, 100.f);
	ImGui::Checkbox("Show light's depth map", &o_showMap);

	ImGui::Separator();

	if (ImGui::Button("Reset light"))
	{
		light.position = { 1.f, 1.f, 1.f, 0.f };
		light.ambient = { 0.3f, 0.3f, 0.3f };
		light.diffuse = { 1.f, 1.f, 1.f };
		light.specular = { 0.5f, 0.5f, 0.5f };

		light.toonShading = true;

		light.stepAmount = 5;
		light.stepSize = 0.1;

		light.specSize = 0.1;

		light.shadow = true;
		light.shadowPCF = 0;
	}
}

bool Helpers::Edit(unsigned char* io_component, const ClassMetaData& io_metaData, bool& io_enabled)
{
	bool keepOnEntity = true;
	bool openDeletePopup = false;
	if (ImGui::TreeNodeEx(io_metaData.className.c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanFullWidth))
	{
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			openDeletePopup = true;

		ImGui::Checkbox("Activate", &io_enabled);

		Edit(io_component, io_metaData);

		ImGui::Spacing();

		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.5f, 0.5f, 1.f));
		if (ImGui::Selectable(("Remove " + io_metaData.className).c_str()))
			keepOnEntity = false;
		ImGui::PopStyleColor();

		ImGui::Spacing();

		ImGui::TreePop();
	}
	else
		if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
			openDeletePopup = true;

	if (openDeletePopup)
		ImGui::OpenPopup("COMPONENT");

	if (ImGui::BeginPopup("COMPONENT"))
	{
		if (ImGui::Selectable("Remove"))
			keepOnEntity = false;

		ImGui::EndPopup();
	}

	return keepOnEntity;
}

void Helpers::Edit(unsigned char* io_component, const ClassMetaData& io_metaData)
{
	if (!io_metaData.parentClassName.empty())
		Edit(io_component, *Game::Component::FindMetaData(io_metaData.parentClassName));

	for (const DataDescriptor& desc : io_metaData.descriptors)
	{
		unsigned char* componentData = io_component + desc.offset;
		desc.changed = false;

		if (desc.sameLine)
			ImGui::SameLine();

		if (desc.viewOnly) // Just print the variables
		{
			std::string fmt;
			switch (desc.dataType)
			{
			case EDataType::INT:
				ImGui::Text("%s", ScalarDataToString<int>(desc.name, componentData, desc.count).c_str());
				break;
			case EDataType::FLOAT:
				ImGui::Text("%s", ScalarDataToString<float>(desc.name, componentData, desc.count).c_str());
				break;
			case EDataType::STRING:
				ImGui::Text("%s", (*reinterpret_cast<std::string*>(componentData)).c_str());
				break;
			case EDataType::TEXT:
				ImGui::Text("%s", desc.name.c_str());
				break;
			default:
				break;
			}
			ImGui::Spacing();
		}
		else // Edit the variables
		{
			switch (desc.dataType)
			{
			case EDataType::BOOL:
				desc.changed = ImGui::Checkbox(desc.name.c_str(), reinterpret_cast<bool*>(componentData));
				break;
			case EDataType::INT:
				desc.changed = DragScalar(desc.name, reinterpret_cast<int*>(componentData), desc.count, desc.range);
				break;
			case EDataType::FLOAT:
				if (desc.count == 4 && desc.range.y == 1) // A little trick to guess color variable
					desc.changed = ImGui::ColorEdit4(desc.name.c_str(), reinterpret_cast<float*>(componentData), ImGuiColorEditFlags_Float);
				else
					desc.changed = DragScalar(desc.name, reinterpret_cast<float*>(componentData), desc.count, desc.range);
				break;
			case EDataType::STRING:
				desc.changed = Edit(*reinterpret_cast<std::string*>(componentData), desc.name.c_str());
				break;
			case EDataType::BUTTON:
				desc.changed = ImGui::Button(desc.name.c_str());
				break;
			case EDataType::SKELETON:
				Edit(*reinterpret_cast<Renderer::SkeletalModel*>(componentData));
				break;
			case EDataType::ANIMATION:
				Edit(*reinterpret_cast<Animation::AnimationGraph*>(componentData));
				break;
			default:
				break;
			}
			// Call callback function if value changed and callback defined
			if (desc.changed && desc.onChanged)
				desc.onChanged(io_component);
		}
		if (!desc.dropID.empty())
		{
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(desc.dropID.c_str()))
					desc.onDrop(io_component, payload->Data);

				ImGui::EndDragDropTarget();
			}
		}

		if (!desc.toolTip.empty() && ImGui::IsItemHovered())
			ImGui::SetTooltip("%s", desc.toolTip.c_str());
	}
}