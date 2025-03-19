//
// Created by Administrator on 2024/10/11.
//

#ifndef IMTYPES_H
#define IMTYPES_H

#include <imgui.h>

static const char s8_zero = 0, s8_one = 1, s8_fifty = 50, s8_min = -128, s8_max = 127;
static const ImU8 u8_zero = 0, u8_one = 1, u8_fifty = 50, u8_min = 0, u8_max = 255;
static const short s16_zero = 0, s16_one = 1, s16_fifty = 50, s16_min = -32768, s16_max = 32767;
static const ImU16 u16_zero = 0, u16_one = 1, u16_fifty = 50, u16_min = 0, u16_max = 65535;
static const ImS32 s32_zero = 0, s32_one = 1, s32_fifty = 50, s32_min = INT_MIN / 2, s32_max = INT_MAX / 2, s32_hi_a =
                INT_MAX / 2 - 100, s32_hi_b = INT_MAX / 2;
static const ImU32 u32_zero = 0, u32_one = 1, u32_fifty = 50, u32_min = 0, u32_max = UINT_MAX / 2, u32_hi_a =
                UINT_MAX / 2 - 100, u32_hi_b = UINT_MAX / 2;
static const ImS64 s64_zero = 0, s64_one = 1, s64_fifty = 50, s64_min = LLONG_MIN / 2, s64_max = LLONG_MAX / 2, s64_hi_a
                = LLONG_MAX / 2 - 100, s64_hi_b = LLONG_MAX / 2;
static const ImU64 u64_zero = 0, u64_one = 1, u64_fifty = 50, u64_min = 0, u64_max = ULLONG_MAX / 2, u64_hi_a =
                ULLONG_MAX / 2 - 100, u64_hi_b = ULLONG_MAX / 2;
static const float f32_zero = 0.f, f32_one = 1.f, f32_lo_a = -10000000000.0f, f32_hi_a = +10000000000.0f, f32_fifty = 50.0f;
static const double f64_zero = 0., f64_one = 1., f64_lo_a = -1000000000000000.0, f64_hi_a = +1000000000000000.0;

#endif //IMTYPES_H
