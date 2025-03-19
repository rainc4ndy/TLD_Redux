//
// Created by Administrator on 2024/10/6.
//


#ifndef GOCMODULE_H
#define GOCMODULE_H


#include "../../CCheatModule.h"
namespace CheatModule::Vehicle {
    class GocModule : public CCheatModule {
    protected:
        unsigned int tick{};

        bool ok_with_control();
        void update_tick();

    public:
        GocModule(std::string name, std::string nameEn);
        float speed{};
        unsigned int delay{};

        void onSave(nlohmann::json &j) override;
        void onLoad(const nlohmann::json &j) override;
    };
}


#endif //GOCMODULE_H
