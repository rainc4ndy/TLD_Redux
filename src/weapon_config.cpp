#include "weapon_config.h"

// Define the static arrays
float WeaponConfig::s_WeaponDamage[] = {
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    82.5f, 0.0f, 1.0f, 9.9f, 46.2f, 82.5f, 8.25f, 13.2f,
    46.2f, 3.3f, 3.3f, 4.95f, 6.6f, 8.25f, 9.9f, 9.9f,
    6.6f, 24.75f, 41.25f, 82.5f, 82.5f, 1.0f, 46.2f, 82.5f,
    0.0f, 0.33f, 0.33f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    2.64f, 9.9f, 330.0f, 82.5f, 1.0f, 1.0f, 165.0f
};

int WeaponConfig::s_MaxWeaponShootRate[] = {
    250, 250, 250, 250, 250, 250, 250, 250,
    250, 30, 250, 250, 250, 250, 250, 250,
    0, 0, 0, 90, 20, 0, 160, 120,
    120, 800, 120, 120, 50, 90, 90, 90,
    90, 50, 800, 800, 0, 0, 0, 20,
    0, 0, 10, 10, 0, 0, 0, 0,
    0, 400
};

float WeaponConfig::s_WeaponRange[] = {
    1.76f, 1.76f, 1.76f, 1.76f, 1.76f, 1.76f, 1.6f, 1.76f,
    1.76f, 1.76f, 1.76f, 1.76f, 1.76f, 1.76f, 1.76f, 1.76f,
    40.0f, 40.0f, 40.0f, 90.0f, 75.0f, 0.0f, 35.0f, 35.0f,
    35.0f, 40.0f, 35.0f, 40.0f, 35.0f, 45.0f, 70.0f, 90.0f,
    35.0f, 100.0f, 320.0f, 55.0f, 55.0f, 5.1f, 75.0f, 40.0f,
    25.0f, 6.1f, 10.1f, 100.0f, 100.0f, 100.0f, 1.76f
};

// Implementation of the methods
float WeaponConfig::GetWeaponDamage(int id) {
    if (id < 0 || id >= sizeof(s_WeaponDamage) / sizeof(s_WeaponDamage[0])) {
        return -1.0f; // Invalid ID
    }
    return s_WeaponDamage[id];
}

int WeaponConfig::GetMaxWeaponShootRate(int id) {
    if (id < 0 || id >= sizeof(s_MaxWeaponShootRate) / sizeof(s_MaxWeaponShootRate[0])) {
        return -1; // Invalid ID
    }
    return s_MaxWeaponShootRate[id];
}

float WeaponConfig::GetWeaponRange(int id) {
    if (id < 0 || id >= sizeof(s_WeaponRange) / sizeof(s_WeaponRange[0])) {
        return -1.0f; // Invalid ID
    }
    return s_WeaponRange[id];
}
