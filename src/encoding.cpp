//
// Created by Administrator on 2024/10/9.
//

#include "encoding.h"

#include <memory>
#include <vector>
#include <Windows.h>

char* EncodingUtil::u2g(const char* utf8) {
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
    len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
    if (wstr) delete[] wstr;
    return str;
}

//GB2312到UTF-8的转换
char* EncodingUtil::g2u(const char* gb2312) {
    int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len + 1];
    memset(wstr, 0, len + 1);
    MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len + 1];
    memset(str, 0, len + 1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
    if (wstr) delete[] wstr;
    return str;
}

std::string EncodingUtil::readAndConvertUTF8(RakNet::BitStream *bs, size_t size) {
    std::vector<char> buffer(size + 1);
    bs->Read(buffer.data(), static_cast<int>(size));
    buffer[size] = '\0'; // Null-terminate the buffer

    std::unique_ptr<char[]> converted(g2u(buffer.data()));
    std::string result(converted.get());

    // Return the std::string
    return std::move(result); // RVO or move semantics will be used here (explictly)
}