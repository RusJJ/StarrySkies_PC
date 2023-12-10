#include "pch.h"

#define _GTA_SA

#include "../ModUtils/ScopedUnprotect.hpp"
#include "../ModUtils/MemoryMgr.h"
#include "../ModUtils/MemoryMgr.GTA.h"

char* TheCameraSA = AddressByVersion<char*>(0xB6F028, 0, 0);
auto CalcScreenCoorsSA = AddressByVersion<bool (*)(CVector*, CVector*, float*, float*, bool, bool)>(0x70CE30, 0, 0);
auto RenderBufferedOneXLUSpriteSA = AddressByVersion<void(*)(CVector, float, float, uint8_t, uint8_t, uint8_t, short, float, uint8_t)>(0x70E4A0, 0, 0);

void StarrySkies_Patch(float intensity)
{
    CVector ScreenPos, WorldPos, WorldStarPos;
    CVector& CamPos = (*(CMatrix**)(TheCameraSA + 20))->pos;
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

            if (CalcScreenCoorsSA(&WorldStarPos, &ScreenPos, &SZX, &SZY, false, true))
            {
                uint8_t brightness = (1.0f - 0.015f * (rand() % 32)) * 255;
                RenderBufferedOneXLUSpriteSA(ScreenPos, SZX * SZ, SZY * SZ, brightness, brightness, brightness, 255, 1.2f / ScreenPos.z, 255);
            }
        }
    }
}

bool DoStarrySkiesSA()
{
    ScopedUnprotect::Section Protect(GetModuleHandle(nullptr), ".text");

    const int8_t version = Memory::GetVersion().version;
    switch (version)
    {
    case 0:
        Memory::Patch(0x713DDB + 0x0, (uint32_t)0x2024448B); // 8B 44 24 20 (0x2024448B) // unused yet (wrong)
        Memory::Patch(0x713DDB + 0x4, (uint8_t)0x50);
        Memory::InjectHook(0x713DDB + 0x5, StarrySkies_Patch, Memory::HookType::Call);
        Memory::Patch(0x713DDB + 0xA, (uint8_t)0x83);
        Memory::Patch(0x713DDB + 0xB, (uint16_t)0x04C4); // 83 C4 08
        Memory::InjectHook(0x713DDB + 0xD, 0x714019, Memory::HookType::Jump);
        break;

    default: return false;
    }

    return true;
}