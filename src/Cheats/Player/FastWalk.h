#ifndef F96EB00F_B174_4CE7_94D0_85375F6258CD
#define F96EB00F_B174_4CE7_94D0_85375F6258CD


#include <RakHook/rakhook.hpp>

#include "../../CCheatModule.h"
namespace CheatModule
{
namespace Player 
{

class FastWalk : public CCheatModule
{
private:
    unsigned int tick;
public:
    float speed;

    FastWalk(/* args */);
    void onProcess() override;

    void onSave(nlohmann::json &j) override;
    void onLoad(const nlohmann::json &j) override;
};

}
}

#endif /* F96EB00F_B174_4CE7_94D0_85375F6258CD */
