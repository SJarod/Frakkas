#pragma once

#include <sstream>
#include <string>

class StringFormat
{
public:
    /**
     * Centralize all the arguments log into one string.
     * @param i_args The arguments to compose the string.
     */
    template<typename... TArgs>
    static std::string GetFormat(TArgs&&... i_args);

private:
    /**
     * Centralize all the arguments log into one string stream.
     * @param o_stream The output stream to print.
     * @param i_firstArg The first variadic object.
     * @param i_args The others variadic objects
     */
    template<typename TArg, typename... TArgs>
    static void GetFormat(std::stringstream& o_stream, TArg i_firstArg, TArgs&&... i_args);

    /**
    * This function end the GetFormat template process.
    * @param i_stream The string stream with all the logs.
    */
    static void GetFormat(std::stringstream& i_stream) {};
};

template<typename TArg, typename... TArgs>
void StringFormat::GetFormat(std::stringstream& o_stream, TArg i_firstArg, TArgs&&... i_args)
{
    o_stream << i_firstArg;
    GetFormat(o_stream, std::forward<TArgs>(i_args)...);
}

template<typename... TArgs>
std::string StringFormat::GetFormat(TArgs&&... i_args)
{
    std::stringstream stream;
    GetFormat(stream, std::forward<TArgs>(i_args)...);
    return stream.str();
}