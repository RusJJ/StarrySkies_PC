#include "pch.h"

#define _GTA_VC

#define Memory VCMemory
#include "../ModUtils/ScopedUnprotect.hpp"
#include "../ModUtils/MemoryMgr.GTA.h"

CVector& CamPos = *AddressByVersion<CVector*>(0x7E46B8, 0x7E46C0, 0x7E36C0);
auto CalcScreenCoorsVC = AddressByVersion<bool (*)(CVector*, CVector*, float*, float*, bool)>(0x5778B0, 0x5778D0, 0x5777A0);
auto RenderBufferedOneXLUSpriteVC = AddressByVersion<void(*)(CVector, float, float, uint8_t, uint8_t, uint8_t, short, float, uint8_t)>(0x577350, 0x577370, 0x577240);

static void StarrySkies_Patch(float intensity)
{
    CVector ScreenPos, WorldPos, WorldStarPos;
    float SZ, SZX, SZY;

    for (int side = 0; side < SSidesCount; ++side)
    {
        WorldPos = PositionsTable[side] + CamPos;
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
                uint8_t brightness = (1.0f - 0.015f * (rand() % 32)) * 255;
                RenderBufferedOneXLUSpriteVC(ScreenPos, SZX * SZ, SZY * SZ, brightness, brightness, brightness, 255, 1.2f / ScreenPos.z, 255);
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
        VCMemory::Patch(MEMBASE_P + 0x0, (uint32_t)0x1824448B); // 8B 44 24 18 (0x1824448B) // mov eax, [esp+0x18]
        VCMemory::Patch(MEMBASE_P + 0x4, (uint8_t)0x50);
        VCMemory::InjectHook(MEMBASE_P + 0x5, StarrySkies_Patch, VCMemory::HookType::Call);
        VCMemory::Patch(MEMBASE_P + 0xA, (uint8_t)0x83);
        VCMemory::Patch(MEMBASE_P + 0xB, (uint16_t)0x04C4); // 83 C4 08
        VCMemory::InjectHook(MEMBASE_P + 0xD, 0x54033F, VCMemory::HookType::Jump);
        break;

    case 1:
        #undef MEMBASE_P
        #define MEMBASE_P 0x540020
        VCMemory::Patch(MEMBASE_P + 0x0, (uint32_t)0x1824448B); // 8B 44 24 18 (0x1824448B) // mov eax, [esp+0x18]
        VCMemory::Patch(MEMBASE_P + 0x4, (uint8_t)0x50);
        VCMemory::InjectHook(MEMBASE_P + 0x5, StarrySkies_Patch, VCMemory::HookType::Call);
        VCMemory::Patch(MEMBASE_P + 0xA, (uint8_t)0x83);
        VCMemory::Patch(MEMBASE_P + 0xB, (uint16_t)0x04C4); // 83 C4 08
        VCMemory::InjectHook(MEMBASE_P + 0xD, 0x54035F, VCMemory::HookType::Jump);
        break;

    case 2:
        #undef MEMBASE_P
        #define MEMBASE_P 0x53FEF0
        VCMemory::Patch(MEMBASE_P + 0x0, (uint32_t)0x1824448B); // 8B 44 24 18 (0x1824448B) // mov eax, [esp+0x18]
        VCMemory::Patch(MEMBASE_P + 0x4, (uint8_t)0x50);
        VCMemory::InjectHook(MEMBASE_P + 0x5, StarrySkies_Patch, VCMemory::HookType::Call);
        VCMemory::Patch(MEMBASE_P + 0xA, (uint8_t)0x83);
        VCMemory::Patch(MEMBASE_P + 0xB, (uint16_t)0x04C4); // 83 C4 08
        VCMemory::InjectHook(MEMBASE_P + 0xD, 0x54022F, VCMemory::HookType::Jump);
        break;

    default: return false;
    }

    return true;
}