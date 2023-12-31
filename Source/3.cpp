#include "pch.h"

#define _GTA_III

#define Memory IIIMemory
#include "../ModUtils/ScopedUnprotect.hpp"
#include "../ModUtils/MemoryMgr.GTA.h"

CVector& CamPosIII = *AddressByVersion<CVector*>(0x6FAD2C, 0x6FAD2C, 0x70AE6C);
auto CalcScreenCoorsIII = AddressByVersion<bool (*)(CVector*, CVector*, float*, float*, bool)>(0x51C3A0, 0x51C5D0, 0x51C560);
auto RenderBufferedOneXLUSpriteIII = AddressByVersion<void(*)(CVector, float, float, uint8_t, uint8_t, uint8_t, short, float, uint8_t)>(0x51C960, 0x51CB90, 0x51CB20);
uint8_t& ms_nGameClockMinutesIII = *AddressByVersion<uint8_t*>(0x95CDC8, 0x95CF80, 0x96D0C0);
uint8_t& ms_nGameClockHoursIII = *AddressByVersion<uint8_t*>(0x95CDA6, 0x95CF5F, 0x96D09F);
float& FoggynessIII = *AddressByVersion<float*>(0x885AF4, 0x885AA4, 0x895BE4);
float& CloudCoverageIII = *AddressByVersion<float*>(0x8E2818, 0x8E27C8, 0x8F2908);
float& ms_fAspectRatioIII = *AddressByVersion<float*>(0x5F53C0, 0x5F52B0, 0x6022A8);
bool& m_PrefsUseWideScreenIII = *AddressByVersion<bool*>(0x95CD23, 0x95CEDB, 0x96D01B);

static void StarrySkies_Patch()
{
    if (bDisableStars) return;

    CVector ScreenPos, WorldPos, WorldStarPos;
    float SZ, SZX, SZY;

    float intensity = 255.0f - 255.0f * fmaxf(CloudCoverageIII, FoggynessIII);
    if (intensity == 0) return;

    if (ms_nGameClockHoursIII == nStarsHourStart) intensity *= 0.01666666666f * ms_nGameClockMinutesIII;
    else if (ms_nGameClockHoursIII == nStarsHourLast) intensity *= 0.01666666666f * (60 - ms_nGameClockMinutesIII);

    for (int side = 0; side < SSidesCount; ++side)
    {
        WorldPos = PositionsTable[side] + CamPosIII;
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

            if (CalcScreenCoorsIII(&WorldStarPos, &ScreenPos, &SZX, &SZY, false))
            {
                if (bWideFix) SZX /= ms_fAspectRatioIII;

                uint8_t brightness = (uint8_t)((1.0f - 0.015f * (rand() % 32)) * intensity);
                RenderBufferedOneXLUSpriteIII(ScreenPos, SZX * SZ, SZY * SZ, brightness, brightness, brightness, 255, 1.0f / ScreenPos.z, 255);
            }
        }
    }
}

bool DoStarrySkiesIII()
{
    ScopedUnprotect::Section Protect(GetModuleHandle(nullptr), ".text");

    const int8_t version = IIIMemory::GetVersion().version;
    switch (version)
    {
    case 0:
        #define MEMBASE_P 0x4F7100
        IIIMemory::InjectHook(MEMBASE_P + 0x0, StarrySkies_Patch, IIIMemory::HookType::Call);
        IIIMemory::InjectHook(MEMBASE_P + 0x5, 0x4F73E2, IIIMemory::HookType::Jump);

        // Custom stars time
        *(uint8_t*)(0x4F701E) = nStarsHourStart;
        *(uint8_t*)(0x4F7036) = nStarsHourStart;
        *(uint8_t*)(0x4F7056) = nStarsHourStart;
        *(uint8_t*)(0x4F7027) = nStarsHourLast;
        *(uint8_t*)(0x4F703F) = nStarsHourLast;
        break;

    case 1:
        #undef MEMBASE_P
        #define MEMBASE_P 0x4F71E0
        IIIMemory::InjectHook(MEMBASE_P + 0x0, StarrySkies_Patch, IIIMemory::HookType::Call);
        IIIMemory::InjectHook(MEMBASE_P + 0x5, 0x4F74C2, IIIMemory::HookType::Jump);

        // Custom stars time
        *(uint8_t*)(0x4F70FE) = nStarsHourStart;
        *(uint8_t*)(0x4F7116) = nStarsHourStart;
        *(uint8_t*)(0x4F7136) = nStarsHourStart;
        *(uint8_t*)(0x4F7107) = nStarsHourLast;
        *(uint8_t*)(0x4F711F) = nStarsHourLast;
        break;

    case 2:
        #undef MEMBASE_P
        #define MEMBASE_P 0x4F7170
        IIIMemory::InjectHook(MEMBASE_P + 0x0, StarrySkies_Patch, IIIMemory::HookType::Call);
        IIIMemory::InjectHook(MEMBASE_P + 0x5, 0x4F7452, IIIMemory::HookType::Jump);

        // Custom stars time
        *(uint8_t*)(0x4F708E) = nStarsHourStart;
        *(uint8_t*)(0x4F70A6) = nStarsHourStart;
        *(uint8_t*)(0x4F70C6) = nStarsHourStart;
        *(uint8_t*)(0x4F7097) = nStarsHourLast;
        *(uint8_t*)(0x4F70AF) = nStarsHourLast;
        break;

    default: return false;
    }

    return true;
}