#pragma once

#include <string>

#include "debug/log.hpp"
#include "game/inputs_manager.hpp"
#include "game/time_manager.hpp"
#include "game/world.hpp"

#include "game/entity_container.hpp"
#include "game/lowcomponent/component.hpp"
#include "game/reflection/data_descriptor.hpp"

/**
 * @brief Define a struct in-class to reflect field,
 * declare a static metaData variable and define the constructor of the class.
 */

#define KK_COMPONENT(compClass) \
\
class Entity;                                \
class compClass : public Component\
{\
private:\
                                \
    struct ComponentRegister       \
    {                           \
        ComponentRegister()\
        {                       \
            compClass::MetaData().className = #compClass;       \
            compClass::MetaData().constructor = [](){ return new compClass(); };\
            Log::Info("Register '", #compClass, "' component.");\
            GetRegistry().push_back(&MetaData());\
            AddFields();\
        }                                             \
                                                      \
        void AddFields();\
    };\
	static const ComponentRegister componentRegister;                           \
public: \
    static ClassMetaData& MetaData();           \
\
	const ClassMetaData& GetMetaData() const override { return MetaData(); };      \
    const std::string& GetID() const override { return MetaData().className; };

/**
 * @brief Same as KK_COMPONENT. However, it will not be shown in the editor, nor serialize.
 */
#define KK_PRIVATE_COMPONENT(compClass) \
\
class Entity;                                \
class compClass : public Component\
{\
private:\
                                \
    struct ComponentRegister       \
    {                           \
        ComponentRegister()\
        {                       \
            compClass::MetaData().className = #compClass;       \
            compClass::MetaData().constructor = [](){ return new compClass(); };                                              \
            compClass::MetaData().publicClass = false;\
            Log::Info("Register '", #compClass, "' component.");\
            GetRegistry().push_back(&MetaData());               \
            AddFields();\
        }                                             \
                                                      \
        void AddFields();\
    };\
	static const ComponentRegister componentRegister;                           \
public: \
    static ClassMetaData& MetaData();           \
\
	const ClassMetaData& GetMetaData() const override { return MetaData(); };      \
    const std::string& GetID() const override { return MetaData().className; };

/**
 * Create a component inheriting from a parent component.
 */
#define KK_COMPONENT_FROM(compClass, parentCompClass) \
                                \
class Entity;                                \
class compClass : public parentCompClass\
{\
private:\
                                \
    struct ComponentRegister       \
    {                           \
        ComponentRegister()\
        {                       \
            compClass::MetaData().className = #compClass;                                                                     \
            compClass::MetaData().parentClassName = #parentCompClass;\
            compClass::MetaData().constructor = [](){ return new compClass(); };\
            GetRegistry().push_back(&MetaData());\
            Log::Info("Register '", #compClass, "' component.");                                                              \
            AddFields();\
        }                                             \
                                                      \
        void AddFields();\
    };\
    static const ComponentRegister componentRegister;                           \
public:                                               \
    static ClassMetaData& MetaData();           \
\
    const ClassMetaData& GetMetaData() const override { return MetaData(); };      \
    const std::string& GetID() const override { return MetaData().className; };

#define KK_COMPONENT_END };

/**
 * @brief Define the static component's metadata and component register in .cpp.
 * Begin the definition of the register function that will add class's field to metadata.
 * @param compClass The class to define and push field in.
 */
#define KK_COMPONENT_IMPL_BEGIN(compClass) \
	using namespace Game;               \
    const compClass::ComponentRegister compClass::componentRegister;\
    ClassMetaData& compClass::MetaData()\
    {                                \
        static ClassMetaData metaData; \
        return metaData; \
    }\
    _Pragma("GCC diagnostic push")                                                         \
    _Pragma("GCC diagnostic ignored \"-Winvalid-offsetof\"")                                                         \
    void Game::compClass::ComponentRegister::AddFields() \
    {                                  \
        DataDescriptor desc;           \
        std::vector<DataDescriptor>& descriptors = compClass::MetaData().descriptors;

/**
 * @brief End the definition of metadata's field pushing.
 */
#define KK_COMPONENT_IMPL_END \
    }                     \
    _Pragma("GCC diagnostic pop")

/**
 * @brief Push a new field in the metadata.
 * @param compClass The component class to use offsetof() properly.
 * @param fieldName The component's field to reflect.
 * @param dataType An EDataType to define the type of reflected field.
 */
#define KK_FIELD_PUSH(compClass, fieldName, dataType) \
    desc = DataDescriptor(#fieldName, dataType, 1, offsetof(compClass, fieldName)); \
    descriptors.emplace_back(desc);

/**
 * @brief Set the last pushed field as a vector of vectorCount element.
 */
#define KK_FIELD_COUNT(vectorCount) descriptors.back().count = vectorCount;

/**
 * @brief Add a range to the last pushed field.
 * @param valueMin The minimum scalar value to clamp field at.
 * @param valueMax The maximum scalar value to clamp field at.
 */
#define KK_FIELD_RANGE(valueMin, valueMax) descriptors.back().range = {valueMin, valueMax};

/**
 * @brief Mark the last pushed field as ViewOnly
 */
#define KK_FIELD_VIEWONLY descriptors.back().viewOnly = true;

/**
 * @brief Add a callback function to the last pushed field when field's value changed.
 * @param changeFunc function that will be called on value changed. Signature is void(unsigned char* io_component).
 */
#define KK_FIELD_CHANGECALLBACK(changeFunc) descriptors.back().onChanged = [](unsigned char* io_component){changeFunc(io_component);};

/**
 * @brief Mark the last pushed field as a drop target.
 * That means that you could receive data drag & drop on the field, according to its ID.
 * @param ID The string ID of the drag & drop, useful to receive only the awaited data.
 * @param dropFunc A global function with following signature : void(unsigned char* io_component, void* io_dropData)
 */
#define KK_FIELD_DROPTARGET(ID, dropFunc) \
    descriptors.back().dropID = ID;    \
    descriptors.back().onDrop = [](unsigned char* io_component, void* dropData){dropFunc(io_component, dropData);};

/**
 * @brief Push a button field for editor only that will call clickFunc when clicked.
 * @param label The text that will appears on button.
 * @param clickFunc The function that will be called on button clicked. Signature is void(unsigned char* io_component).
 */
#define KK_FIELD_PUSH_BUTTON(label, clickFunc) \
    desc = DataDescriptor(label, EDataType::BUTTON, 1, 0); \
    descriptors.emplace_back(desc);\
    descriptors.back().onChanged = [](unsigned char* io_component){clickFunc(io_component);};

/**
 * @brief Set the last pushed field to the same line that the previous last pushed field.
 */
#define KK_FIELD_SAMELINE descriptors.back().sameLine = true;

/**
 * @brief Push a new text-field to display simple text in editor, like title or section separator.
 */
#define KK_FIELD_PUSH_TEXT(text) \
    desc = DataDescriptor(text, EDataType::TEXT, 1, 0); \
    desc.viewOnly = true;\
    descriptors.emplace_back(desc);

/**
 * @brief Add a tooltip to the last pushed field. Appears when you put the mouse cursor on the field.
 */
#define KK_FIELD_TOOLTIP(text) descriptors.back().toolTip = text;

/**
 * @brief Replace the field name by a different name.
 */
#define KK_FIELD_RENAME(newName) descriptors.back().name = newName;

/**
 * @brief Create new .cpp/.hpp component file if does not already exist.
 * @param i_compName name of the component class.
 * @return true if the component is added, false if not.
 */
bool CreateNewComponentScript(const std::string& i_compName);
