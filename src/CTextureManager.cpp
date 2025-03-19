#include "CTextureManager.h"
#include "CBinaryPacker.h"
#include "CTexture.h"
#include "Plugin.h"
#include "CLog.h"

#include <fstream>

CTexture* CTextureManager::getTexture(std::string name) { return &mp[name]; }

void CTextureManager::loadFromBin(std::string file) {
    CBinaryPacker pck;
    pck.open(file.c_str(), true);
    
    stBinaryHeader header;
    while (pck.nextHeader(header)) {
        auto data = new unsigned char[header.size];
        pck.dump(data, header.size);

        mp[header.fileNameStr] = CTexture();
        auto r = &mp[header.fileNameStr];
        r->fromRaw(data, header.size);
        delete[] data;
    }
}

void CTextureManager::loadFromFile(std::string name, std::string file) {

    std::fstream fs;
    fs.open(file, std::ios::binary | std::ios::in);
    if (fs.is_open()) {
        fs.seekg(0, std::ios::end);
        size_t sz = fs.tellg();
        fs.seekg(0);

        //Plugin::getInstance().getLog()->log("bg file size: %d",sz);

        auto data = new unsigned char[sz];
        fs.read((char*)data, sz);

        mp[name] = CTexture();
        auto r = &mp[name];
        r->fromRaw(data, sz);

        delete[] data;
        fs.close();
    }
}

void CTextureManager::clear() {
    for (auto& it : mp){
        it.second.release();
    }
    mp.clear();
}
