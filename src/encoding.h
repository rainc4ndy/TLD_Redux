//
// Created by Administrator on 2024/10/9.
//

#ifndef ENCODING_H
#define ENCODING_H

#include <string>
#include <BitStream.h>

class EncodingUtil {
public:
    
    static char *u2g(const char *utf8);
    static char *g2u(const char *gb2312);
    static std::string readAndConvertUTF8(RakNet::BitStream *bs, size_t size);
};

#endif //ENCODING_H
