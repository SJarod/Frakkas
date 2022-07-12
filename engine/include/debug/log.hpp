#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <mutex>

#include "helpers/path_constants.hpp"
#include "utils/string_format.hpp"

#include "utils/singleton.hpp"

class Log : public Singleton<Log>
{
    friend class Singleton<Log>;

public:
    Log() = default;
    ~Log();

    static void Init()
    {
#ifdef IN_BUILD_DIRECTORY
        Instance().logFile.open("../build/FrakkasLog.log", std::ios::out);
#else
        Instance().logFile.open("build/FrakkasLog.log", std::ios::out);
#endif
    }

    /**
     * Print log on console, and save in .log file with label [INFO].
     * @param i_logs The logs to print, it could be any object that works with 'ostream<<' operator.
     */
    template <typename... Args>
    static void Info(Args&&... i_logs);
    /**
     * Print log on console, and save in .log file with label [WARNING].
     * @param i_logs The logs to print, it could be any object that works with 'ostream<<' operator.
     */
    template <typename... Args>
    static void Warning(Args&&... i_logs);
    /**
     * Print log on console, and save in .log file with label [ERROR].
     * @param i_logs The logs to print, it could be any object that works with 'ostream<<' operator.
     */
    template <typename... Args>
    static void Error(Args&&... i_logs);

    /**
    * Print log on console, and save in .log file with label [DEBUG].
    * @param i_logs The logs to print, it could be any object that works with 'ostream<<' operator.
    */
    template <typename... Args>
    static void Debug(Args&&... i_logs);

    static std::vector<std::string>& GetLogList()
    {
        return Instance().logList;
    }

private:
    std::ofstream logFile;
    std::vector<std::string> logList;
    std::mutex logMX;

    

    /**
    * @Summary Output the casted log
    */
    static void Out(const std::string& i_log);
};

template <typename... Args>
void Log::Info(Args&&... i_logs)
{
    Out("[INFO] " + StringFormat::GetFormat(std::forward<Args>(i_logs)...));
}

template <typename... Args>
void Log::Warning(Args&&... i_logs)
{
    Out("[WARNING] " + StringFormat::GetFormat(std::forward<Args>(i_logs)...));
}

template <typename... Args>
void Log::Error(Args&&... i_logs)
{
    Out("[ERROR] " + StringFormat::GetFormat(std::forward<Args>(i_logs)...));
}

template <typename... Args>
void Log::Debug(Args&&... i_logs)
{
    Out("[DEBUG] " + StringFormat::GetFormat(std::forward<Args>(i_logs)...));
}