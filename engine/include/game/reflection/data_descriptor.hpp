#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

#include "maths.hpp"

enum class EDataType
{
	INT,
	FLOAT,
	BOOL,
	STRING,
    ANIMATION, // A table for the animation graph
    SKELETON, // A tree of bones, to visualize and add model
    NONE_DATA, // A limit between serialized data and editor tools
    TEXT, // A simple text for inspector
    BUTTON // A button for inspector
};

using KKEditorAction = std::function<void(unsigned char*)>;
using KKEditorDropAction = std::function<void(unsigned char*, void*)>;

/**
 * @brief Store informations about a class' field, to reflect class datas.
 */
struct DataDescriptor
{
    DataDescriptor() = default;
	DataDescriptor(const std::string& name, EDataType dataType, int count, size_t offset, bool viewOnly = false, const Vector2& range = Vector2())
		:name(name), dataType(dataType), count(count), offset(offset), viewOnly(viewOnly), range(range) {}

	std::string name;
	EDataType dataType = EDataType::INT;
	size_t offset = 0;
    mutable bool changed {false};

	int count = 1;
	bool viewOnly = false;
    Vector2 range;

    bool sameLine = false;
    std::string toolTip;

    std::string dropID;
    KKEditorDropAction onDrop;

    KKEditorAction onChanged;
};

namespace Game
{
    class Component;
}
/**
 * @brief Store all the reflected field of a class
 */
struct ClassMetaData
{
	std::string className;
    std::string parentClassName;
    bool publicClass = true;
    std::function<Game::Component*()> constructor; // [](){ return new PlayerComponent(); }
	std::vector<DataDescriptor> descriptors;
};
