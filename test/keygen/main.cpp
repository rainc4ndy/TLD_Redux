#include <iostream>
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
// Function to get CPU serial number
std::string GetCPUSerialNumber() {
    std::string result;
    int cpuInfo[4] = {-1};
    __cpuid(cpuInfo, 1);
    unsigned int lowPart = cpuInfo[3];
    unsigned int highPart = cpuInfo[0];
    std::stringstream stream;
    stream << std::hex << highPart << std::hex << lowPart;
    result = stream.str();
    return result;
}

// Function to get hard disk serial number
std::string GetHardDiskSerialNumber() {
    std::string result;
    DWORD dwVolumeSerialNumber;
    GetVolumeInformationA("C:\\", NULL, 0, &dwVolumeSerialNumber, NULL, NULL, NULL, 0);
    std::stringstream stream;
    stream << std::hex << std::setw(8) << std::setfill('0') << dwVolumeSerialNumber;
    result = stream.str();
    return result;
}

std::string SHA256HashString(const std::string &aString) {
    std::string digest;
    CryptoPP::SHA256 hash;
    CryptoPP::StringSource foo(aString, true,
                               new CryptoPP::HashFilter(hash,
                                                        new CryptoPP::HexEncoder(
                                                            new CryptoPP::StringSink(digest)
                                                        )
                               )
    );
    return digest;
}

// thanks to raksamp devs!
int gen_gpci(char *buf, unsigned long factor) /* by bartekdvd */
{
    srand(time(0));
    unsigned char out[(6 * 4)] = {0};

    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = 0; i < 6 * 4; ++i)
        out[i] = alphanum[rand() % (sizeof(alphanum) - 1)];

    BIG_NUM_MUL((unsigned long *) out, (unsigned long *) out, factor);
    unsigned int notzero = 0;
    buf[0] = '0';
    buf[1] = '\0';

    if (factor == 0) return 1;

    int pos = 0;
    for (int i = 0; i < 24; i++) {
        unsigned char tmp = out[i] >> 4;
        unsigned char tmp2 = out[i] & 0x0F;

        if (notzero || tmp) {
            buf[pos++] = (char) ((tmp > 9) ? (tmp + 55) : (tmp + 48));
            if (!notzero) notzero = 1;
        }

        if (notzero || tmp2) {
            buf[pos++] = (char) ((tmp2 > 9) ? (tmp2 + 55) : (tmp2 + 48));
            if (!notzero) notzero = 1;
        }
    }
    buf[pos] = 0;

    return pos;
}

int main() {
    char auth_bs[4 * 16];
    auto my_version = std::string("0.3.7-R5");
    gen_gpci(auth_bs, 0x3e9);
    std::cout << "use gpci: " << auth_bs << std::endl;
    return 0;
}
