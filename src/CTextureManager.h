#ifndef B9475861_5BB8_48B0_8EFC_D5A656F2AE9B
#define B9475861_5BB8_48B0_8EFC_D5A656F2AE9B

#include <unordered_map>
#include <string>
#include "CTexture.h"
#include "types.h"

class CTextureManager
{
private:
    std::unordered_map<std::string, CTexture> mp;
public:
    CTextureManager(/* args */) = default;
    ~CTextureManager() = default;

    CTexture* getTexture(std::string name);
    void loadFromBin(std::string file);
    void loadFromFile(std::string name, std::string file);

    void clear();
};

#endif /* B9475861_5BB8_48B0_8EFC_D5A656F2AE9B */
