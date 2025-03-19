//
// Created by Administrator on 2024/10/7.
//

#ifndef TARGETSELECTOR_H
#define TARGETSELECTOR_H

#include <CPed.h>
class CTargetSelector {
private:
    unsigned int tick;
    void updateTick();
public:
    CTargetSelector();
    enum eTargetMode {
        TM_NEAREST,
        TM_RANDOM,
        TM_RANDOM_SERVER,
        TM_SINGLE
    };

    int delay;
    int mode;
    bool NPCIncluded;
    int specifiedPlayerID;
    int targetPlayerID;
    CPed* targetPlayerPed;

    bool get();
};

#endif //TARGETSELECTOR_H
