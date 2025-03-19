#include "CCheatManager.h"

std::unordered_set<CCheatModule*> CCheatManager::allInstnace;
std::unordered_map<std::string, CCheatModule*> CCheatManager::nameMap;
void CCheatManager::add(CCheatModule* module) {
    allInstnace.emplace(module);
    nameMap[module->getNameEn()] = module;
}

const std::unordered_set<CCheatModule*> CCheatManager::getAll() {
    return allInstnace;
}

CCheatModule * CCheatManager::get(std::string englishName) {
    return nameMap[englishName];
}
