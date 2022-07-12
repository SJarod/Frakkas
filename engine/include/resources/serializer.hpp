#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <filesystem>

#include "maths.hpp"
#include "game/lowcomponent/component.hpp"

struct ClassMetaData;

namespace Game
{
    struct Transform;
    class Entity;
    class EntityManager;
}

namespace Renderer
{
    class Light;
    class Model;
    class SkeletalModel;

    namespace LowLevel
    {
        class Camera;
    }
}

namespace Animation
{
    class AnimationGraph;
}

namespace Resources
{
    class Serializer
    {
    public:
        // Store the current read-variable name
        static std::string attribute;

        static constexpr int transformParameterCount = 3;
        static constexpr int lightParameterCount = 10;

    private:
        static bool skipAttribute;

    public: /// READ FUNCTIONS
        /**
         *
         * @param i_file the opened input file to read line
         * @param o_attribute the output attribute's name
         * @return the output attribute's name. So that we can use this function in condition structure.
         */
        static std::string& GetAttribute(std::ifstream& i_file);

        /**
         * @brief read all the components of an Entity
         * @param i_file the opened input file
         * @param o_entity the Entity to build
         */
        static void Read(std::ifstream& i_file, Game::Entity& o_entity);

        /**
         * @brief Read all the information about an Entity and create it into entity manager.
         * @param i_file The opened input file
         * @param io_entityContainer The entity container to create and link entity
         * @param i_parent A parent entity to set to the created entity. Can be nullptr.
         */
        static Game::Entity* CreateAndReadEntity(std::ifstream& i_file, Game::EntityContainer& io_entityContainer, Game::Entity* i_parent = nullptr);

        /**
         * @brief read a component and store data in an unsigned char pointer.
         * @param i_file the opened input file.
         * @param o_component the Component to build.
         * @param i_metaData The class meta data to read the info properly.
         * @param o_enabled the enabled state of the component.
         */
        static void Read(std::ifstream& i_file, unsigned char* o_component, const ClassMetaData& i_metaData, bool& o_enabled);

        /**
         * @brief Read all the data descriptors from a component metadata, and store them in an unsigned char pointer.
         * @param i_file the opened input file.
         * @param o_component the Component to build.
         * @param i_metaData The class meta data to read the info properly.
         */
        static void Read(std::ifstream& i_file, unsigned char* o_component, const ClassMetaData& i_metaData);

        /**
         * @brief LoadScene a certain number of scalar and store them.
         * @tparam TScalarType The scalar type : int, float, unsigned int.
         * @param i_file The opened input file.
         * @param o_vector The pointer to the scalar array.
         * @param i_count The number of scalar in the pointer.
         */
        template<typename TScalarType>
        static void Read(std::ifstream& i_file, TScalarType* o_vector, int i_count = 1);

        /**
         * @brief read a string in the next line of i_file
         * @param i_file the opened input file
         * @param o_string the string to build
         */
        static void Read(std::ifstream& i_file, std::string& o_string);

        /**
         * @brief read am bool in the next line of i_file
         * @param i_file the opened input file
         * @param o_value the bool to build
         */
        static void Read(std::ifstream& i_file, bool& o_value);

        /**
         * @brief read a transform in the next line of i_file
         * @param i_file the opened input file
         * @param o_transform the transform to build
         */
        static void Read(std::ifstream& i_file, Game::Transform& o_transform);

        /**
         * @brief read a light in the next line of i_file
         * @param i_file the opened input file
         * @param o_light the light to build
         */
        static void Read(std::ifstream& i_file, Renderer::Light& o_light);

        /**
         * @brief read an animation graph in the next line of i_file
         * @param i_file the opened input file
         * @param o_animGraph the graph to build
         */
        static void Read(std::ifstream& i_file, Animation::AnimationGraph& o_animGraph);

        /**
         * @brief read a skeletal model's sockets.
         * @param i_file the opened input file
         * @param o_skmodel the skeletal model
         */
        static void Read(std::ifstream& i_file, Renderer::SkeletalModel& o_skmodel);

    public: /// WRITE FUNCTIONS

        /**
         * @brief SaveScene an attribute in frakkas text format
         * @param io_file the opened output file to write in
         * @param i_attribute the variable type to write
         */
        static void WriteAttribute(std::ofstream& io_file, const std::string& i_attribute);

        /**
         * @brief Write all the entity's components in frakkas text format
         * @param io_file the opened output file to write in
         */
        static void Write(std::ofstream& io_file, const Game::Entity& i_entity);

        /**
         * @brief Write a component in frakkas text format
         * @param io_file the opened output file to write in
         * @param i_component the component data
         * @param i_metaData the component reflection info to write data properly
         * @param i_enabled the enabled state of the component
         */
        static void Write(std::ofstream& io_file, unsigned char* i_component, const ClassMetaData& i_metaData, bool i_enabled);

        /**
         * @brief Write all the data descriptors from a component metadata in frakkas text format
         * @param io_file The opened input file.
         * @param i_component The component data.
         * @param i_metaData The component reflection info.
         */
        static void Write(std::ofstream& io_file, unsigned char* i_component, const ClassMetaData& i_metaData);

        /**
         * @brief Write an array of scalar in frakkas text format
         * @tparam TScalarType The type of scalar : int, float, unsigned int...
         * @param io_file The opened output file.
         * @param i_vector The pointer to scalar array.
         * @param i_count The number of scalar in the array.
         */
        template<typename TScalarType>
        static void Write(std::ofstream& io_file, const std::string& i_attributeName, const TScalarType* i_vector, int i_count = 1);

        /**
         * @brief Write a bool in frakkas text format
         * @param io_file the opened output file to write in
         */
        static void Write(std::ofstream& io_file, const std::string& i_attributeName, bool i_bool);

        /**
         * @brief SaveScene a component in frakkas text format
         * @param io_file the opened output file to write in
         */
        static void Write(std::ofstream& io_file, const std::string& i_attributeName, const std::string& i_string);

        /**
         * @brief SaveScene a transform in frakkas text format
         * @param io_file the opened output file to write in
         */
        static void Write(std::ofstream& io_file, const std::string& i_attributeName, const Game::Transform& i_transform);

        /**
         * @brief SaveScene a light in frakkas text format
         * @param io_file the opened output file to write in
         */
        static void Write(std::ofstream& io_file, const std::string& i_attributeName, const Renderer::Light& i_light);

        /**
         * @brief SaveScene an animation graph in frakkas text format
         * @param io_file the opened output file to write in
         */
        static void Write(std::ofstream& io_file, const std::string& i_attributeName, const Animation::AnimationGraph& i_animGraph);

        /**
         * @brief SaveScene a skeletal model's sockets in frakkas text format.
         * @param io_file the opened output file to write in
         */
        static void Write(std::ofstream& io_file, const std::string& i_attributeName, const Renderer::SkeletalModel& i_skmodel);

    private:
        /**
         * @return '\t', so a tabulation in character format. Useful to write tabulation with std::cout
         */
        static char Tab();
    };
}

template<typename TScalarType>
void Resources::Serializer::Read(std::ifstream& i_file, TScalarType* o_vector, int i_count)
{
    char eatComma;
    // Read first scalar
    i_file >> o_vector[0];
    // Read each scalar and eat comma
    for (int i = 1; i < i_count; i++)
        i_file >> eatComma >> o_vector[i];
    // Ignore end of line
    i_file.ignore();
}

template<typename TScalarType>
void Resources::Serializer::Write(std::ofstream& io_file, const std::string& i_attributeName, const TScalarType* i_vector, int i_count)
{
    WriteAttribute(io_file, i_attributeName);
    // Write first scalar
    io_file << Tab() << i_vector[0];
    // Write others scalar with comma.
    for(int i = 1; i < i_count; i++)
        io_file << ", " << i_vector[i];
    // End line
    io_file << std::endl;
}