#include <fstream>
#include <iomanip>
#include <cstdarg>  // 用于 va_start 和 va_end
#include <sstream>
#include <chrono>
#include <ctime>

#include "CLog.h"

CLog::CLog(const char* workingDirectory)
    : g_szWorkingDirectory(workingDirectory)
{
    std::string logFilename = g_szWorkingDirectory + "\\" + "latest.log";
    logFile.open(logFilename, std::ios::out);

    std::string logFilenameAll = g_szWorkingDirectory + "\\" + "all.log";
    logFileAll.open(logFilenameAll, std::ios::out | std::ios::app);
}

CLog::~CLog() {
    if (logFile.is_open()) logFile.close();
    if (logFileAll.is_open()) logFileAll.close();
}

void CLog::log(const char* fmt, ...) {
    if (g_szWorkingDirectory.empty()) return;

    va_list ap;
    va_start(ap, fmt);

    // Get current time
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm localTime;
    localtime_s(&localTime, &time);

    // Log to g_flLog
    logToFile(logFile, fmt, ap, false);

    // Log to g_flLogAll
    va_start(ap, fmt); // Restart argument list
    logToFile(logFileAll, fmt, ap, true);
    va_end(ap);
}

void CLog::logToFile(std::ofstream& file, const char* fmt, va_list ap, bool isAll) {
    if (!file.is_open()) return;

    // Get current time
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm localTime;
    localtime_s(&localTime, &time);

    // Format log entry
    std::ostringstream logEntry;
    if (isAll) {
        logEntry << "[" << std::setw(2) << std::setfill('0') << localTime.tm_mday << "-"
                  << std::setw(2) << std::setfill('0') << (localTime.tm_mon + 1) << "-"
                  << (localTime.tm_year + 1900) << " || "
                  << std::setw(2) << std::setfill('0') << localTime.tm_hour << ":"
                  << std::setw(2) << std::setfill('0') << localTime.tm_min << ":"
                  << std::setw(2) << std::setfill('0') << localTime.tm_sec << "."
                  << std::setw(3) << std::setfill('0') << (std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000)
                  << "] ";
    } else {
        logEntry << "[" << std::setw(2) << std::setfill('0') << localTime.tm_hour << ":"
                  << std::setw(2) << std::setfill('0') << localTime.tm_min << ":"
                  << std::setw(2) << std::setfill('0') << localTime.tm_sec << "."
                  << std::setw(3) << std::setfill('0') << (std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count() % 1000)
                  << "] ";
    }

    // Print formatted message
    char buffer[512];
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    logEntry << buffer;

    // Write to file
    file << logEntry.str() << std::endl;
    file.flush();
}