#include "debug/log.hpp"
#include "game/reflection/data_descriptor.hpp"

#include "game/transform.hpp"
#include "game/entity.hpp"
#include "game/lowcomponent/component.hpp"
#include "game/entity_manager.hpp"

#include "animation/animation_graph.hpp"

#include "renderer/light.hpp"
#include "renderer/skeletal_model.hpp"

#include "resources/serializer.hpp"

using namespace Resources;

std::string Serializer::attribute;
bool Serializer::skipAttribute = false;

///////////////////////// READ FUNCTIONS

std::string& Resources::Serializer::GetAttribute(std::ifstream& i_file)
{
    if (i_file.eof())
    {
        attribute = "End of file";
        return attribute;
    }

    std::getline(i_file, attribute);

    if (attribute.empty() || attribute[0] != '>')
    {
        if (!attribute.empty())
            Log::Warning("Scene loader can't find an Attribute, process once again. Current value is : '", attribute, "'");
        return GetAttribute(i_file); // Call Get attribute until we find an attribute.
    }

    attribute = attribute.substr(1);
    return attribute;
}

Game::Entity* Serializer::CreateAndReadEntity(std::ifstream& i_file, Game::EntityContainer& io_entityContainer, Game::Entity* i_parent)
{
    int childCount = 0;
    if (!skipAttribute) // Skip Attribute is true if last component didn't find a field attribute, so it found 'entity' attribute
        GetAttribute(i_file); // '>entity'

    skipAttribute = false;

    if (attribute != "entity")
        return nullptr;

    GetAttribute(i_file); // '>childs'
    if (attribute != "childs")
        return nullptr;
    Read(i_file, &childCount, 1);

    std::string entityName;
    GetAttribute(i_file); // '>name'
    if (attribute != "name")
        return nullptr;
    Read(i_file, entityName);

    Game::Entity* entity = io_entityContainer.CreateEntity(entityName);

    if (i_parent)
        io_entityContainer.SetEntityParent(*entity, *i_parent);

    Read(i_file, *entity); // Read components

    if (!skipAttribute) // If skip attribute true, the cursor is already at the good place
        i_file.ignore(); // skip lines

    // Load child which are the next written entities, and set this entity as parent.
    for (int i = 0; i < childCount; i++)
        CreateAndReadEntity(i_file, io_entityContainer, entity);

    return entity;
}

void Serializer::Read(std::ifstream& i_file, Game::Entity& o_entity)
{
    int componentCount = 0;
    GetAttribute(i_file);
    Read(i_file, &componentCount, 1);

    GetAttribute(i_file);
    Read(i_file, o_entity.transform);

    for (int i = 0; i < componentCount; i++)
    {
        if (!skipAttribute)
            GetAttribute(i_file); // name of component

        auto& registry = Game::Component::GetRegistry();
        auto it = std::find_if(registry.begin(), registry.end(), [](ClassMetaData* md){return attribute == md->className;});
        if (it == registry.end())
        {
            Log::Error("Can't read '" + attribute + "' component's data.");
            break;
        }

        Game::Component* comp = o_entity.AddComponent(std::unique_ptr<Game::Component>((*it)->constructor()));
        bool compEnabled = comp->enabled;
        Read(i_file, reinterpret_cast<unsigned char*>(comp), comp->GetMetaData(), compEnabled);
        comp->enabled = compEnabled;
    }
}

void Serializer::Read(std::ifstream& i_file, unsigned char* o_component, const ClassMetaData& i_metaData, bool& o_enabled)
{
    GetAttribute(i_file);
    Read(i_file, o_enabled);
    Read(i_file, o_component, i_metaData);
}

void Serializer::Read(std::ifstream& i_file, unsigned char* o_component, const ClassMetaData& i_metaData)
{
    if (!i_metaData.parentClassName.empty())
        Read(i_file, o_component, *Game::Component::FindMetaData(i_metaData.parentClassName));

    for (const DataDescriptor& desc : i_metaData.descriptors)
    {
        bool isSerialized = static_cast<int>(desc.dataType) < static_cast<int>(EDataType::NONE_DATA);

        if (desc.viewOnly || !isSerialized)
            continue;

        unsigned char* componentData = o_component + desc.offset;
        if ((skipAttribute && attribute == desc.name) || GetAttribute(i_file) == desc.name)
        {
            skipAttribute = false;
            switch (desc.dataType)
            {
                case EDataType::BOOL:
                        Read(i_file, *reinterpret_cast<bool*>(componentData));
                    break;
                case EDataType::INT:
                        Read(i_file, reinterpret_cast<int*>(componentData), desc.count);
                    break;
                case EDataType::FLOAT:
                        Read(i_file, reinterpret_cast<float*>(componentData), desc.count);
                    break;
                case EDataType::STRING:
                    Read(i_file, *reinterpret_cast<std::string*>(componentData));
                    break;
                case EDataType::ANIMATION:
                    Read(i_file, *reinterpret_cast<Animation::AnimationGraph*>(componentData));
                    break;
                case EDataType::SKELETON:
                    Read(i_file, *reinterpret_cast<Renderer::SkeletalModel*>(componentData));
                    break;
                default:
                    break;
            }
            if (desc.onChanged)
                desc.onChanged(o_component);
        }
        else
            skipAttribute = true;
    }
}

void Serializer::Read(std::ifstream& i_file, std::string& o_string)
{
    i_file.ignore();
    std::getline(i_file, o_string);
}

void Serializer::Read(std::ifstream& i_file, bool& o_value)
{
    int val = 0;
    Read(i_file, &val, 1);
    o_value = static_cast<bool>(val);
}

void Serializer::Read(std::ifstream& i_file, Game::Transform& o_transform)
{
    for(int i = 0; i < transformParameterCount; i++)
    {
        GetAttribute(i_file);
        if (attribute == "position")
        {
            Vector3 position = o_transform.position;
            Read(i_file, position.element, 3);
            o_transform.position = position;
        }
        else if (attribute == "rotation")
        {
            Vector3 rotation = o_transform.rotation;
            Read(i_file, rotation.element, 3);
            o_transform.rotation = rotation;
        }
        else if (attribute == "scale")
        {
            Vector3 scale = o_transform.scale;
            Read(i_file, scale.element, 3);
            o_transform.scale = scale;
        }
    }
}

void Serializer::Read(std::ifstream& i_file, Renderer::Light& o_light)
{
    for (int i = 0; i < lightParameterCount; i++)
    {
        GetAttribute(i_file);
        if (attribute == "position")
            Read(i_file, o_light.position.element, 4);
        else if (attribute == "ambient")
            Read(i_file, o_light.ambient.element, 3);
        else if (attribute == "diffuse")
            Read(i_file, o_light.diffuse.element, 3);
        else if (attribute == "specular")
            Read(i_file, o_light.specular.element, 3);
        else if (attribute == "toonShading")
            Read(i_file, o_light.toonShading);
        else if (attribute == "stepAmount")
            Read(i_file, &o_light.stepAmount, 1);
        else if (attribute == "stepSize")
            Read(i_file, &o_light.stepSize, 1);
        else if (attribute == "specSize")
            Read(i_file, &o_light.specSize, 1);
        else if (attribute == "shadow")
            Read(i_file, o_light.shadow);
        else if (attribute == "shadowPCF")
            Read(i_file, &o_light.shadowPCF, 1);
    }
}

void Serializer::Read(std::ifstream& i_file, Animation::AnimationGraph& o_animGraph)
{
    GetAttribute(i_file);
    int count;
    Read(i_file, &count);

    for (int i = 0; i < count; ++i)
    {
        GetAttribute(i_file);
        std::string animFile;
        Read(i_file, &animFile);

        o_animGraph.AddAnimationPack(animFile);
    }
}

void Serializer::Read(std::ifstream& i_file, Renderer::SkeletalModel& o_skmodel)
{
    GetAttribute(i_file);
    int count;
    Read(i_file, &count);

    for (int i = 0; i < count; ++i)
    {
        Game::Transform transform;
        GetAttribute(i_file);
        Read(i_file, transform);

        int id;
        GetAttribute(i_file);
        Read(i_file, &id);

        std::string meshName, textureName;
        GetAttribute(i_file);
        Read(i_file, meshName);
        GetAttribute(i_file);
        Read(i_file, textureName);

        o_skmodel.AddSocket(meshName, textureName, false, id, transform);
    }
}

///////////////////////// WRITE FUNCTIONS

void Serializer::WriteAttribute(std::ofstream &io_file, const std::string &i_attribute)
{
    io_file << '>' << i_attribute << std::endl;
}

void Serializer::Write(std::ofstream& io_file, const Game::Entity& i_entity)
{
    if (i_entity.serialize)
    {
        io_file << '\n';

        WriteAttribute(io_file, "entity");

        int childCount = 0;
        for (Game::Entity* child : i_entity.childs)
            childCount += child->serialize ? 1 : 0;
        Write(io_file, "childs", &childCount, 1);

        Write(io_file, "name", i_entity.name);

        int compCount = static_cast<int>(i_entity.components.size());
        Write(io_file, "component_count", &compCount, 1);

        Write(io_file, "transform", i_entity.transform);

        for(const std::unique_ptr<Game::Component>& comp : i_entity.components)
            Write(io_file, reinterpret_cast<unsigned char*>(comp.get()), comp->GetMetaData(), comp->enabled.get());

        for (const Game::Entity* child : i_entity.childs)
            Write(io_file, *child);
    }
}

void Serializer::Write(std::ofstream& io_file, unsigned char* i_component, const ClassMetaData& i_metaData, bool i_enabled)
{
    WriteAttribute(io_file, i_metaData.className);
    Write(io_file, "enabled", i_enabled);
    Write(io_file, i_component, i_metaData);
}

void Serializer::Write(std::ofstream& io_file, unsigned char* i_component, const ClassMetaData& i_metaData)
{
    if(!i_metaData.parentClassName.empty())
        Write(io_file, i_component, *Game::Component::FindMetaData(i_metaData.parentClassName));

    for (const DataDescriptor& desc : i_metaData.descriptors)
    {
        if (desc.viewOnly)
            continue;

        unsigned char* componentData = i_component + desc.offset;

        switch (desc.dataType)
        {
            case EDataType::BOOL:
                Write(io_file, desc.name, *reinterpret_cast<bool*>(componentData));
                break;
            case EDataType::INT:
                Write(io_file, desc.name, reinterpret_cast<int*>(componentData), desc.count);
                break;
            case EDataType::FLOAT:
                Write(io_file, desc.name, reinterpret_cast<float*>(componentData), desc.count);
                break;
            case EDataType::STRING:
                Write(io_file, desc.name, *reinterpret_cast<std::string*>(componentData));
                break;
            case EDataType::ANIMATION:
                Write(io_file, desc.name, *reinterpret_cast<Animation::AnimationGraph*>(componentData));
                break;
            case EDataType::SKELETON:
                Write(io_file, desc.name, *reinterpret_cast<Renderer::SkeletalModel*>(componentData));
                break;
            default:
                break;
        }
    }

}

void Serializer::Write(std::ofstream& io_file, const std::string& i_attributeName, bool i_bool)
{
    int i = i_bool ? 1 : 0;
    Write(io_file, i_attributeName, &i, 1);
}

void Serializer::Write(std::ofstream& io_file, const std::string& i_attributeName, const std::string& i_string)
{
    WriteAttribute(io_file, i_attributeName);
    io_file << Tab() << i_string << std::endl;
}

void Serializer::Write(std::ofstream& io_file, const std::string& i_attributeName, const Game::Transform& i_transform)
{
    WriteAttribute(io_file, i_attributeName);
    Write(io_file, "position", i_transform.position.get().element, 3);
    Write(io_file, "rotation", i_transform.rotation.get().element, 3);
    Write(io_file, "scale", i_transform.scale.get().element, 3);
    // 3 parameter serialized
}

void Serializer::Write(std::ofstream& io_file, const std::string& i_attributeName, const Renderer::Light& i_light)
{
    WriteAttribute(io_file, i_attributeName);

    Write(io_file, "position", i_light.position.element, 4);
    Write(io_file, "ambient", i_light.ambient.element, 3);
    Write(io_file, "diffuse", i_light.diffuse.element, 3);
    Write(io_file, "specular", i_light.specular.element, 3);
    Write(io_file, "toonShading", &i_light.toonShading);

    Write(io_file, "stepAmount", &i_light.stepAmount);
    Write(io_file, "stepSize", &i_light.stepSize);
    Write(io_file, "specSize", &i_light.specSize);
    Write(io_file, "shadow", &i_light.shadow);
    Write(io_file, "shadowPCF", &i_light.shadowPCF);
    // 10 parameters serialized
}

void Serializer::Write(std::ofstream& io_file, const std::string& i_attributeName, const Animation::AnimationGraph& i_animGraph)
{
    WriteAttribute(io_file, i_attributeName);

    std::vector<std::string_view> files;
    i_animGraph.GetFilesName(files);

    int count = static_cast<int>(files.size());
    Write(io_file, "count", &count, 1);

    for (auto & file : files)
        Write(io_file, "file", &file);
}

void Serializer::Write(std::ofstream& io_file, const std::string& i_attributeName, const Renderer::SkeletalModel& i_skmodel)
{
    WriteAttribute(io_file, i_attributeName);

    int count = static_cast<int>(i_skmodel.sockets.size());
    Write(io_file, "count", &count, 1);

    std::list<Renderer::Socket> sockets = i_skmodel.sockets;
    for (auto it = sockets.begin(); it != sockets.end(); ++it)
    {
        Write(io_file, "transform", it->transform);
        Write(io_file, "boneID", &it->boneID, 1);
        Write(io_file, "socketMesh", it->model.mesh.lock()->name);
        Write(io_file, "socketTexture", it->model.textureName);
    }
}

char Serializer::Tab()
{
    return '\t';
}