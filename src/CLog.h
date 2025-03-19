#ifndef CB900ABC_612E_4F3F_B8D3_5FF60CD2E944
#define CB900ABC_612E_4F3F_B8D3_5FF60CD2E944

#include <fstream>
#include <sstream>

class CLog {
public:
    CLog(const char* workingDirectory);
    ~CLog();

    void log(const char* fmt, ...);

private:
    std::ofstream logFile;
    std::ofstream logFileAll;
    std::string g_szWorkingDirectory; // Assuming this is initialized somewhere

    void logToFile(std::ofstream& file, const char* fmt, va_list ap, bool isAll);
};


#endif /* CB900ABC_612E_4F3F_B8D3_5FF60CD2E944 */
