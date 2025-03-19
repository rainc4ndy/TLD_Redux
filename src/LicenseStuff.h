//
// Created by Administrator on 2025/2/14.
//

#ifndef LICENSESTUFF_H
#define LICENSESTUFF_H

#include <mutex>
#include <string>
#include <thread>
#include <vector>

/**
 * 用于获取机器码 sha256
 */
class CMachineIdentifier {
private:
    static std::string GetCPUSerialNumber();
    static std::string GetHardDiskSerialNumber();
public:
    static std::string GetHash();
};

/**
 * key凭证检查器
 */
class CLicenseChecker {
private:
    const static std::string LicenseServers;
    const static std::string AES_KEY, AES_IV;
    const static std::string KEY_FILE;

    enum eLicenseStatus {
        PiratedKey,
        CorruptedKey,
        ExpiredKey,
        LegalKey
    };

    std::string Key;
    std::string MachineID;
    unsigned int ExpireTime;
    unsigned int Duration;
    unsigned int RemainTime;

    std::atomic_bool waitForMSG;
    std::unique_ptr<std::thread> updateThread;
public:
    CLicenseChecker();
    ~CLicenseChecker();

    void ProcessLicenseCheck();
    int authenticate(std::string license);

    unsigned int GetExpireTime();
    unsigned int GetDuration();
    unsigned int GetRemainTime();

    /**
     * 从服务器获取每日信息
     */
    void GetDayMessage();
    void GetUpdate();

    std::string LoadLicenseFromFile(std::string licenseFile);
    void SaveLicenseToFile(std::string license, std::string fileName);
};

#endif //LICENSESTUFF_H
