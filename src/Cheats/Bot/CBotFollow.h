//
// Created by Administrator on 2024/10/12.
//

#ifndef CBOTFOLLOW_H
#define CBOTFOLLOW_H
#include "CCheatModule.h"

namespace CheatModule {
namespace Bot {

class CBotFollow : public CCheatModule {
private:
    void adjustPosition(float* position, float heading, int id, int total);
public:
    CBotFollow();

    enum stFollowStrategy {
        FS_ALIGN, //Align with player's left and right
        FS_ROUND, //Form a circle of bot
        FS_SQAURE,
        FS_FRONT
    };

    enum stAnimStrategy {
        AS_FOLLOW, //Use origin anim
        AS_DANCE,  //Use dance anim
        AS_ARMY, //Use origin anim
    };

    int followStrategy;
    float follow_range;
    bool shoot_with_me;
    bool drive_with_me;
    bool aim_with_me;
    bool chat_with_me;
    bool all_rpc_follow;
    bool onProcess(stOnFootData *data) override;
    bool onProcess(stBulletData *data) override;
    bool onProcess(stAimData *data) override;
    bool onProcess(stInCarData *data) override;
    bool onProcess(bool fromServer, int rpc, RakNet::BitStream* bs) override;

    void onSave(nlohmann::json &j);
    void onLoad(const nlohmann::json &j);
};

} // Bot
} // CheatModule

#endif //CBOTFOLLOW_H
