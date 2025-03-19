//
// Created by Administrator on 2025/2/15.
//

#ifndef TLD_REDUX_CPATCHMODULE_H
#define TLD_REDUX_CPATCHMODULE_H

#include "CCheatModule.h"
class CPatchModule : public CCheatModule {
protected:
    virtual void work() = 0;
    void patchAddress(void* address, const char* patchData, size_t size);
public:
    CPatchModule(std::string name, std::string nameEn);
    void onEnable() override;
    void onDisable() override;
};



#endif //TLD_REDUX_CPATCHMODULE_H
