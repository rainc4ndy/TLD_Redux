//
// Created by Administrator on 2024/10/8.
//

#ifndef TARGETRENDERER_H
#define TARGETRENDERER_H

#include "CCheatModule.h"

namespace CheatModule::Target {
    class TargetRenderer : public CCheatModule {
    private:
    public:
        TargetRenderer();
        static void drawTargetOnPed(CPed* pPed);
        void onProcessVisual() override;
    };
}




#endif //TARGETRENDERER_H
