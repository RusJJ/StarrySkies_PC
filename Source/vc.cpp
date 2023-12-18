#include "pch.h"

#define _GTA_VC

#define Memory VCMemory
#include "../ModUtils/ScopedUnprotect.hpp"
#include "../ModUtils/MemoryMgr.GTA.h"

CVector& CamPosVC = *AddressByVersion<CVector*>(0x7E46B8, 0x7E46C0, 0x7E36C0);
auto CalcScreenCoorsVC = AddressByVersion<bool (*)(CVector*, CVector*, float*, float*, bool)>(0x5778B0, 0x5778D0, 0x5777A0);
auto RenderBufferedOneXLUSpriteVC = AddressByVersion<void(*)(CVector, float, float, uint8_t, uint8_t, uint8_t, short, float, uint8_t)>(0x577350, 0x577370, 0x577240);
uint8_t& ms_nGameClockMinutesVC = *AddressByVersion<uint8_t*>(0xA10B92, 0xA10B9B, 0xA0FB9C);
uint8_t& ms_nGameClockHoursVC = *AddressByVersion<uint8_t*>(0xA10B6B, 0xA10B74, 0xA0FB75);
float& FoggynessVC = *AddressByVersion<float*>(0x94DDC0, 0x94DDC8, 0x94CDC8);
float& CloudCoverageVC = *AddressByVersion<float*>(0x974BE8, 0x974BF0, 0x973BF0);
float& ms_fAspectRatioVC = *AddressByVersion<float*>(0x94DD38, 0x94DD40, 0x94CD40);

static void StarrySkies_Patch()
{
    if (bDisableStars) return;

    CVector ScreenPos, WorldPos, WorldStarPos;
    float SZ, SZX, SZY;

    float intensity = 255.0f - 255.0f * fmaxf(CloudCoverageVC, FoggynessVC);
    if (intensity == 0) return;

    if (ms_nGameClockHoursVC == 22) intensity *= 0.01666666666f * ms_nGameClockMinutesVC;
    else if (ms_nGameClockHoursVC == 5) intensity *= 0.01666666666f * (60 - ms_nGameClockMinutesVC);

    for (int side = 0; side < SSidesCount; ++side)
    {
        WorldPos = PositionsTable[side] + CamPosVC;
        for (int i = 0; i < AMOUNT_OF_SIDESTARS; ++i)
        {
            WorldStarPos = WorldPos;
            SZ = StarSizes[side][i];
            switch (side)
            {
            case SSide_Left:
            case SSide_Right:
                WorldStarPos.y -= StarCoorsX[side][i];
                WorldStarPos.z += StarCoorsY[side][i];
                break;

            case SSide_Front:
            case SSide_Back:
                WorldStarPos.x -= StarCoorsX[side][i];
                WorldStarPos.z += StarCoorsY[side][i];
                break;

            default:
                WorldStarPos.x += StarCoorsX[side][i];
                WorldStarPos.y += StarCoorsY[side][i];
                break;
            }

            if (CalcScreenCoorsVC(&WorldStarPos, &ScreenPos, &SZX, &SZY, false))
            {
                if (bWideFix) SZX /= ms_fAspectRatioVC;

                uint8_t brightness = (uint8_t)((1.0f - 0.015f * (rand() % 32)) * intensity);
                RenderBufferedOneXLUSpriteVC(ScreenPos, SZX * SZ, SZY * SZ, brightness, brightness, brightness, 255, 1.0f / ScreenPos.z, 255);
            }
        }
    }
}

bool DoStarrySkiesVC()
{
    ScopedUnprotect::Section Protect(GetModuleHandle(nullptr), ".text");

    const int8_t version = VCMemory::GetVersion().version;
    switch (version)
    {
    case 0:
        #define MEMBASE_P 0x540000
        VCMemory::InjectHook(MEMBASE_P + 0x0, StarrySkies_Patch, VCMemory::HookType::Call);
        VCMemory::InjectHook(MEMBASE_P + 0x5, 0x54033F, VCMemory::HookType::Jump);

        // Custom stars time
        *(uint8_t*)(0x53FF09) = nStarsHourStart;
        *(uint8_t*)(0x53FF26) = nStarsHourStart;
        *(uint8_t*)(0x53FF46) = nStarsHourStart;
        *(uint8_t*)(0x53FF12) = nStarsHourLast;
        *(uint8_t*)(0x53FF2F) = nStarsHourLast;
        break;

    case 1:
        #undef MEMBASE_P
        #define MEMBASE_P 0x540020
        VCMemory::InjectHook(MEMBASE_P + 0x0, StarrySkies_Patch, VCMemory::HookType::Call);
        VCMemory::InjectHook(MEMBASE_P + 0x5, 0x54035F, VCMemory::HookType::Jump);

        // Custom stars time
        *(uint8_t*)(0x53FF29) = nStarsHourStart;
        *(uint8_t*)(0x53FF46) = nStarsHourStart;
        *(uint8_t*)(0x53FF66) = nStarsHourStart;
        *(uint8_t*)(0x53FF32) = nStarsHourLast;
        *(uint8_t*)(0x53FF4F) = nStarsHourLast;
        break;

    case 2:
        #undef MEMBASE_P
        #define MEMBASE_P 0x53FEF0
        VCMemory::InjectHook(MEMBASE_P + 0x0, StarrySkies_Patch, VCMemory::HookType::Call);
        VCMemory::InjectHook(MEMBASE_P + 0x5, 0x54022F, VCMemory::HookType::Jump);

        // Custom stars time
        *(uint8_t*)(0x53FDF9) = nStarsHourStart;
        *(uint8_t*)(0x53FE16) = nStarsHourStart;
        *(uint8_t*)(0x53FE36) = nStarsHourStart;
        *(uint8_t*)(0x53FE02) = nStarsHourLast;
        *(uint8_t*)(0x53FE2F) = nStarsHourLast;
        break;

    default: return false;
    }

    return true;
}