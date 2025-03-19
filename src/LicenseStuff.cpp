//
// Created by Administrator on 2025/2/14.
//

#define WIN32_LEAN_AND_MEAN

#include "LicenseStuff.h"
#include "httplib.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <Windows.h>
#include <iphlpapi.h> // 包含这个头文件以获取 PIP_ADAPTER_INFO
#include <intrin.h> // 包含这个头文件以使用 __cpuid

#include <sha.h>
#include <filters.h>
#include <hex.h>
#include <base64.h>
#include <aes.h>
#include <ccm.h>

#include <sampapi/CChat.h>

#include <iostream>
#include <nlohmann/json.hpp>

#include "CLog.h"
#include "encoding.h"
#include "LoginPatterns.h"
#include "Plugin.h"
#include "samp.h"
#include "Utils.h"

const std::string CLicenseChecker::AES_KEY = "58A8991E895865CC88D9C1AD903F0E31",
        CLicenseChecker::AES_IV = "36006FA136AF1273",
        CLicenseChecker::KEY_FILE = "/HEX";

std::string CMachineIdentifier::GetCPUSerialNumber() {
    std::string result;
    int cpuInfo[4] = {-1};
    __cpuid(cpuInfo, 1);
    unsigned int lowPart = cpuInfo[3];
    unsigned int highPart = cpuInfo[0];
    std::stringstream stream;
    stream << std::hex << highPart << std::hex << lowPart;
    return stream.str();
}

std::string CMachineIdentifier::GetHardDiskSerialNumber() {
    std::string result;
    DWORD dwVolumeSerialNumber;
    GetVolumeInformationA("C:\\", NULL, 0, &dwVolumeSerialNumber, NULL, NULL, NULL, 0);
    std::stringstream stream;
    stream << std::hex << std::setw(8) << std::setfill('0') << dwVolumeSerialNumber;
    return stream.str();
}

std::string CMachineIdentifier::GetHash() {
    std::string machineID = GetCPUSerialNumber() + GetHardDiskSerialNumber();
    std::string digest;
    CryptoPP::SHA256 hash;
    CryptoPP::StringSource foo(machineID, true,
                               new CryptoPP::HashFilter(hash,
                                                        new CryptoPP::HexEncoder(
                                                            new CryptoPP::StringSink(digest)
                                                        )
                               )
    );
    return digest;
}


CLicenseChecker::CLicenseChecker() {
    MachineID = CMachineIdentifier::GetHash();
    waitForMSG = false;
}

CLicenseChecker::~CLicenseChecker() {
    updateThread->join();
}

void CLicenseChecker::ProcessLicenseCheck() {
    // using namespace std;
    //
    // vector<string> pending_messages;
    // string my_license = LoadLicenseFromFile(Plugin::getInstance().getDirectory() + KEY_FILE);
    // if (my_license.size()) {
    //     switch (authenticate(my_license)) {
    //         case PiratedKey: {
    //             pending_messages.emplace_back("密钥无效，信息和此计算机不一致");
    //             break;
    //         }
    //         case CorruptedKey: {
    //             pending_messages.emplace_back("你的旧密钥已经损坏无法读取!");
    //             break;
    //         }
    //         case ExpiredKey: {
    //             pending_messages.emplace_back("你的时长已经耗尽无法继续使用TLD，及时更换新密钥.");
    //             break;
    //         }
    //         case LegalKey: {
    //             // 合法，不需要打开终端
    //             return;
    //         }
    //     }
    // }
    //
    // AllocConsole();
    // SetConsoleOutputCP(CP_UTF8);
    // SetConsoleCP(CP_UTF8);
    // SetConsoleTitle("*** The Last Dancer ***");
    // freopen("CONOUT$", "w", stdout);
    // freopen("CONIN$", "r", stdin); // For input
    //
    // HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    //
    // SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
    //
    // cout << LoginPattern << endl;
    // cout << "当前版本: " << Plugin::CurrentVersion << endl;
    //
    // for (auto &it: pending_messages) {
    //     cout << it << endl;
    // }
    //
    // cout << "使用TLD需要密钥，请在下方输入你从blume手上获取的密钥:" << endl;
    //
    // while (true) {
    //     std::string input_key;
    //     cout << "-> ";
    //     cin >> input_key;
    //     cout << "验证中..." << endl;
    //     httplib::Client cli("http://" + Plugin::MasterServer);
    //     httplib::Params params;
    //     params.emplace("licenseKey", input_key);
    //     params.emplace("machine", MachineID);
    //     if (auto res = cli.Post("/api/key", params)) {
    //         if (res->status == httplib::StatusCode::OK_200) {
    //             switch (authenticate(res->body)) {
    //                 case PiratedKey: {
    //                     cout << "你并非密钥的持有者，密钥无效" << endl;
    //                     break;
    //                 }
    //                 case CorruptedKey: {
    //                     cout << "该密钥无效，可能已经损坏" << endl;
    //                     break;
    //                 }
    //                 case ExpiredKey: {
    //                     cout << "时长已经耗尽，该密钥已经作废了." << endl;
    //                     break;
    //                 }
    //                 case LegalKey: {
    //                     SaveLicenseToFile(res->body, Plugin::getInstance().getDirectory() + KEY_FILE);
    //                     cout << "激活成功" << endl;
    //                     cout << "Redux VIP 时长 " << RemainTime / 86400 << " 天" << endl;
    //                     cout << "3秒后自动进入游戏" << endl;
    //                     std::this_thread::sleep_for(3000ms);
    //                     return;
    //                 }
    //             }
    //         } else {
    //             std::cout << "出错: " << res->body << std::endl;
    //         }
    //     } else {
    //         auto err = res.error();
    //         std::cout << "HTTP 错误: " << httplib::to_string(err) << std::endl;
    //         std::this_thread::sleep_for(1000ms);
    //     }
    // }
}

int CLicenseChecker::authenticate(std::string license) {
    using namespace CryptoPP;
    try {
        // Decode base64
        std::string decoded;
        StringSource ss(license, true,
                        new Base64Decoder(
                            new StringSink(decoded)
                        )
        );

        auto key = std::vector<byte>(AES_KEY.begin(), AES_KEY.end()),
                iv = std::vector<byte>(AES_IV.begin(), AES_IV.end());

        CBC_Mode<AES>::Decryption d;
        d.SetKeyWithIV(key.data(), key.size(), iv.data(), iv.size());

        // AES CBC 解密
        std::string decryptedData;
        StringSource s(decoded, true,
                       new StreamTransformationFilter(d,
                                                      new StringSink(decryptedData)
                       ) // StreamTransformationFilter
        ); // StringSource

        //std::cout << "decryptedData: " << decryptedData << std::endl;
        //std::fstream fout("key.key", std::ios::out | std::ios::binary);
        //fout.write(decryptedData.data(), decryptedData.size());
        // 从JSON中导出密钥信息
        try {
            nlohmann::json j = nlohmann::json::parse(decryptedData);
            auto key_machine = j.at("machine").get<std::string>();

            if (key_machine != MachineID) {
                return PiratedKey;
            }
            Key = j.at("key").get<std::string>();
            ExpireTime = j.at("expireTime").get<unsigned int>();
            Duration = j.at("duration").get<unsigned int>();

            RemainTime = static_cast<unsigned int>(std::max(0LL, ExpireTime - (time(0) + 3600 * 8)));
            if (RemainTime <= 0) {
                return ExpiredKey;
            }
        } catch (nlohmann::detail::parse_error) {
            return CorruptedKey;
        }
        return LegalKey;
    } catch (Exception) {
        return CorruptedKey;
    }
}

unsigned int CLicenseChecker::GetExpireTime() {
    return ExpireTime;
}

unsigned int CLicenseChecker::GetDuration() {
    return Duration;
}

unsigned int CLicenseChecker::GetRemainTime() {
    return RemainTime;
}

void CLicenseChecker::GetDayMessage() {
    using namespace std::chrono;
    auto worker = Plugin::getInstance().getAsyncHTTPWorker();
    worker->emplace({
        Plugin::MasterServer,
        "/api/msg",
        METHOD_GET,
        [this](bool success, HTTPResponse result) {
            if (success) {
                try {
                    auto j = nlohmann::json::parse(result.body);
                    if (j.is_array()) {
                        for (auto it = j.begin(); it != j.end(); ++it) {
                            auto msg = it->at("message").get<std::string>();
                            auto color = it->at("color").get<DWORD>();
                            auto con = std::unique_ptr<char[]>(EncodingUtil::u2g(("[TLD] " + msg).c_str())).get();
                            samp::RefChat()->AddMessage(color, con);
                        }
                    }
                } catch (std::exception &e) {
                    samp::RefChat()->AddMessage(0xcf0000AA, "[x] Unable to get TLD update message");
                    samp::RefChat()->AddMessage(0xcf0000AA, e.what());
                }
            } else {
                samp::RefChat()->AddMessage(0xcf0000AA, std::unique_ptr<char[]>(EncodingUtil::u2g("[x] 获取TLD公告失败")).get());
            }
        }});
}

void CLicenseChecker::GetUpdate() {
    using namespace std::chrono;
    auto worker = Plugin::getInstance().getAsyncHTTPWorker();
    worker->emplace({
        Plugin::MasterServer,
        "/api/update",
        METHOD_GET,
        [this](bool success, HTTPResponse result) {
            if (success) {
                try {
                    auto j = nlohmann::json::parse(result.body);
                    auto version_s = j.at("tld_version").get<std::string>();
                    auto version_info_s = j.at("tld_update_info").get<std::string>();
                    float version = std::stof(version_s);
                    if (version > std::stof(Plugin::CurrentVersion)) {
                        samp::RefChat()->AddMessage(0x00c728AA ,
                            std::unique_ptr<char[]>(EncodingUtil::u2g(("* 有版本更新! 最新TLD版本: " + version_s).c_str())).get());
                        samp::RefChat()->AddMessage(0x00c728AA ,
                            std::unique_ptr<char[]>(EncodingUtil::u2g(("* 更新内容: " + version_info_s).c_str())).get());
                    } else {
                        samp::RefChat()->AddMessage(0x00c728AA ,
                            std::unique_ptr<char[]>(EncodingUtil::u2g("* 当前已是最新版")).get());
                    }
                } catch (std::exception &e) {
                    samp::RefChat()->AddMessage(0xcf0000AA, e.what());
                }
            } else {
                samp::RefChat()->AddMessage(0xcf0000AA,
                            std::unique_ptr<char[]>(EncodingUtil::u2g("[x] 获取更新失败")).get());
            }
        }});
}

std::string CLicenseChecker::LoadLicenseFromFile(std::string licenseFile) {
    std::ifstream file(licenseFile, std::ios::in | std::ios::binary);
    if (file.is_open()) {
        using namespace CryptoPP;

        std::stringstream buffer;
        buffer << file.rdbuf();

        std::string digest;
        StringSource ss(buffer.str(), true, new HexDecoder(new StringSink(digest)));
        return digest;
    }
    return "";
}

void CLicenseChecker::SaveLicenseToFile(std::string license, std::string fileName) {
    std::ofstream file(fileName, std::ios::out | std::ios::binary);
    if (file.is_open()) {
        using namespace CryptoPP;
        std::string digest;
        StringSource ss(license, true, new HexEncoder(new StringSink(digest)));

        file.write(digest.c_str(), digest.length());
        file.close();
    }
}
