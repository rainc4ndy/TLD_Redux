//
// Created by Administrator on 2024/10/11.
//

#ifndef AUTHKEY_H
#define AUTHKEY_H

namespace CheatModule {
namespace Bot {
    int gen_gpci(char* buffer, unsigned long factor);
    extern char AuthKeyTable[512][2][128];
} // Bot
} // CheatModule

#endif //AUTHKEY_H
