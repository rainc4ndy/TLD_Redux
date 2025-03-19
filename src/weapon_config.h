#ifndef WEAPON_CONFIG_H
#define WEAPON_CONFIG_H

class WeaponConfig {
public:
    static float GetWeaponDamage(int id);
    static int GetMaxWeaponShootRate(int id);
    static float GetWeaponRange(int id);

private:
    static float s_WeaponDamage[];
    static int s_MaxWeaponShootRate[];
    static float s_WeaponRange[];
};

#endif // WEAPON_CONFIG_H
