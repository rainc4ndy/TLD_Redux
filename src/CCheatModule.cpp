#include "CCheatModule.h"

#include "CCheatManager.h"
#include "CLog.h"
#include "Plugin.h"

CCheatModule::CCheatModule(std::string name, std::string nameEn)
    : name(std::move(name)), nameEn(std::move(nameEn)), enabled(false), pending_for_enable(false) {
    CCheatManager::add(this);
    used_by_bot = false;
}

CCheatModule::CCheatModule(CheatModule::Bot::CBot *bot) : enabled(false), pending_for_enable(false) {
    this->bot = bot;
    used_by_bot = true;
}

bool CCheatModule::isPending() {
    return pending_for_enable;
}

bool CCheatModule::isEnabled() {
    return enabled;
}

void CCheatModule::setEnabled(bool enabled) {
    this->enabled = enabled;
    if (enabled) {
        onEnable();
    } else {
        onDisable();
    }
}

std::string CCheatModule::getName() {
    return name;
}

std::string CCheatModule::getNameEn() {
    return nameEn;
}

void CCheatModule::saveJson(nlohmann::json &j) {
    j = {
        {"enabled", enabled},
        {"data", {}}
    };
    onSave(j["data"]);
}

void CCheatModule::fromJson(const nlohmann::json &j) {
    onLoad(j["data"]);
    pending_for_enable = j["enabled"].get<bool>();
}

void CCheatModule::onSave(nlohmann::json &j) {
}

void CCheatModule::onLoad(const nlohmann::json &j) {
}

template<typename T>
void CCheatModule::saveData(nlohmann::json &j, const std::string& key, T value) {
    j[key] = value; // Save the value to JSON
}

template<typename T>
void CCheatModule::loadData(const nlohmann::json &j, const std::string& key, T& value) {
    if (j.contains(key)) {
        value = j[key].get<T>(); // Load the value from JSON
    }
}

// Explicit instantiation for commonly used types
template void CCheatModule::saveData<int>(nlohmann::json&, const std::string&, int);
template void CCheatModule::loadData<int>(const nlohmann::json&, const std::string&, int&);
template void CCheatModule::saveData<bool>(nlohmann::json&, const std::string&, bool);
template void CCheatModule::loadData<bool>(const nlohmann::json&, const std::string&, bool&);
template void CCheatModule::saveData<float>(nlohmann::json&, const std::string&, float);
template void CCheatModule::loadData<float>(const nlohmann::json&, const std::string&, float&);
template void CCheatModule::saveData<std::string>(nlohmann::json&, const std::string&, std::string);
template void CCheatModule::loadData<std::string>(const nlohmann::json&, const std::string&, std::string&);

template void CCheatModule::saveData<unsigned long>(nlohmann::json&, const std::string&, unsigned long);
template void CCheatModule::loadData<unsigned long>(const nlohmann::json&, const std::string&, unsigned long&);
template void CCheatModule::saveData<unsigned int>(nlohmann::json&, const std::string&, unsigned int);
template void CCheatModule::loadData<unsigned int>(const nlohmann::json&, const std::string&, unsigned int&);