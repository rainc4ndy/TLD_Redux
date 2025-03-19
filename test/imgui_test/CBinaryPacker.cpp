#include "CBinaryPacker.h"
#include <iostream>
std::string CBinaryPacker::XorKey = "LwAex3wck1";

CBinaryPacker::CBinaryPacker() {}
CBinaryPacker::~CBinaryPacker() {}

void CBinaryPacker::xorEncryptDecrypt(const unsigned char* raw,
                                      unsigned char* out, size_t sz) {
    size_t keyLength = XorKey.size();
    for (size_t i = 0; i < sz; ++i) out[i] = raw[i] ^ XorKey[i % keyLength];
}

void CBinaryPacker::encodeData(const unsigned char* raw, unsigned char* out,
                               size_t sz) {
    xorEncryptDecrypt(raw, out, sz);
    for (int i = 0; i < sz / 2; i++) {
        char tmp = out[i];
        out[i] = out[sz-1-i];
        out[sz-1-i] = tmp;
    }
}

void CBinaryPacker::decodeData(const unsigned char* raw, unsigned char* out,
                               size_t sz) {
    for (int i = 0; i < sz / 2; i++) {
        char tmp = out[i];
        out[i] = out[sz-1-i];
        out[sz-1-i] = tmp;
    }
    xorEncryptDecrypt(raw, out, sz);
}

void CBinaryPacker::appendBinary(std::string name, const unsigned char* raw, size_t sz) {
    stBinaryHeader header;
    header.size = sz;
    header.fileName_sz = name.size();

    auto fileName = new char[name.size()];
    encodeData((unsigned char*)(name.c_str()), (unsigned char*)fileName, name.size());

    auto encrypted_raw = new unsigned char[sz];
    encodeData(raw, encrypted_raw, sz);

    buffer.write(reinterpret_cast<const char*>(&header.size), sizeof(header.size));
    buffer.write(reinterpret_cast<const char*>(&header.fileName_sz), sizeof(header.fileName_sz));

    buffer.write(fileName, header.fileName_sz);
    buffer.write(reinterpret_cast<const char*>(encrypted_raw), header.size);

    delete encrypted_raw;
    delete fileName;
}

void CBinaryPacker::open(const char *path, bool read) {
    buffer.open(path, read ? (std::ios_base::in | std::ios::binary) : (std::ios_base::out | std::ios::binary));
    if (buffer.is_open()) {
        // pass
    } else {
        throw std::runtime_error("File not found");
    }
}

void CBinaryPacker::save() { 
    buffer.close();
    buffer.flush();
}

bool CBinaryPacker::nextHeader(stBinaryHeader& header) { 
    if (!buffer.good()) {
        return false;
    }

    // Debugging output for tellg()
    std::streampos currentPos = buffer.tellg();
    if (currentPos == -1) {
        return false;
    }

    // Read the header information
    buffer.read(reinterpret_cast<char*>(&header.size), sizeof(header.size));
    buffer.read(reinterpret_cast<char*>(&header.fileName_sz), sizeof(header.fileName_sz));

    if (buffer.fail()) {
        return false;
    }
    
    // Allocate memory for the file name and read it
    auto tmp = new char[header.fileName_sz + 1];
    buffer.read(tmp, header.fileName_sz);

    tmp[header.fileName_sz] = '\0';
    decodeData((unsigned char*)tmp, (unsigned char*)tmp, header.fileName_sz);
    
    header.fileNameStr = tmp;
    delete[] tmp;
    return true;
}


void CBinaryPacker::dump(unsigned char* out, size_t sz) {
    buffer.read((char*)out, sz);
    decodeData(out, out, sz);
}
