#include "pch.h"

#define _GTA_SA

#define Memory SAMemory
#include "../ModUtils/ScopedUnprotect.hpp"
#include "../ModUtils/MemoryMgr.GTA.h"

char* TheCameraSA = AddressByVersion<char*>(0xB6F028, 0xB716A8, 0);
auto CalcScreenCoorsSA = AddressByVersion<bool (*)(CVector*, CVector*, float*, float*, bool, bool)>(0x70CE30, 0, 0);
auto RenderBufferedOneXLUSpriteSA = AddressByVersion<void(*)(CVector, float, float, uint8_t, uint8_t, uint8_t, short, float, uint8_t)>(0x70E4A0, 0, 0);
auto RwIm3DRenderIndexedPrimitiveSA = AddressByVersion<int(*)(int, int16_t*, int)>(0x7EF550, 0, 0);
auto RwRenderStateSetSA = AddressByVersion<int(*)(int, void*)>(0x7FE420, 0, 0);
uint8_t& ms_nGameClockMinutesSA = *AddressByVersion<uint8_t*>(0xB70152, 0, 0);
uint8_t& ms_nGameClockHoursSA = *AddressByVersion<uint8_t*>(0xB70153, 0, 0);
float& FoggynessSA = *AddressByVersion<float*>(0xC81300, 0, 0);
float& CloudCoverageSA = *AddressByVersion<float*>(0xC81304, 0, 0);
float& ms_fAspectRatioSA = *AddressByVersion<float*>(0xC3EFA4, 0, 0);

static void StarrySkies_Patch()
{
    if (bDisableStars) return;

    CVector ScreenPos, WorldPos, WorldStarPos;
    CVector& CamPosSA = (*(CMatrix**)(TheCameraSA + 20))->pos;
    float SZ, SZX, SZY;

    float intensity = 255.0f - 255.0f * fmaxf(CloudCoverageSA, FoggynessSA);
    if (intensity <= 0) return;

    if (ms_nGameClockHoursSA == nStarsHourStart) intensity *= 0.01666666666f * ms_nGameClockMinutesSA;
    else if(ms_nGameClockHoursSA == nStarsHourLast) intensity *= 0.01666666666f * (60 - ms_nGameClockMinutesSA);

    for (int side = 0; side < SSidesCount; ++side)
    {
        WorldPos = PositionsTable[side] + CamPosSA;
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
                if (bWideFix) SZX /= ms_fAspectRatioSA;

                uint8_t brightness = (uint8_t)((1.0f - 0.015f * (rand() % 32)) * intensity);
                RenderBufferedOneXLUSpriteSA(ScreenPos, SZX * SZ, SZY * SZ, brightness, brightness, brightness, 255, 1.0f / ScreenPos.z, 255);
            }
        }
    }

    if (bDrawEasterEgg)
    {
        for (int i = 0; i < 9; ++i)
        {
            SZ = RockStar_StarSize[i];
            WorldPos = CamPosSA;
            WorldPos.x += 100.0f;
            WorldPos.y -= RockStar_StarX[i];
            WorldPos.z += RockStar_StarY[i];

            if (CalcScreenCoorsSA(&WorldPos, &ScreenPos, &SZX, &SZY, false, true))
            {
                if (bWideFix) SZX /= ms_fAspectRatioSA;

                uint8_t brightness = (uint8_t)((1.0f - 0.015f * (rand() % 32)) * intensity);
                RenderBufferedOneXLUSpriteSA(ScreenPos, SZX * SZ, SZY * SZ, brightness, brightness, brightness, 255, 1.0f / ScreenPos.z, 255);
            }
        }

        SZ = 5.0f;
        WorldPos = CamPosSA + RockStar_MainStarOff;
        if (CalcScreenCoorsSA(&WorldPos, &ScreenPos, &SZX, &SZY, false, true))
        {
            if (bWideFix) SZX /= ms_fAspectRatioSA;

            uint8_t brightness = (uint8_t)((1.5f - 0.00156f * (rand() % 128)) * intensity);
            RenderBufferedOneXLUSpriteSA(ScreenPos, SZX * SZ, SZY * SZ, brightness, brightness, brightness, 255, 1.0f / ScreenPos.z, 255);
        }
    }
}

static void ShootingStars_v10_Patch(int primType, int16_t* indices, int numIndices)
{
    RwRenderStateSetSA(1, NULL);
    RwIm3DRenderIndexedPrimitiveSA(primType, indices, numIndices);
}

bool DoStarrySkiesSA()
{
    ScopedUnprotect::Section Protect(GetModuleHandle(nullptr), ".text");

    const int8_t version = SAMemory::GetVersion().version;
    switch (version)
    {
    case 0:
        #define MEMBASE_P 0x713DDB
        SAMemory::InjectHook(MEMBASE_P + 0x0, StarrySkies_Patch, SAMemory::HookType::Call);
        SAMemory::InjectHook(MEMBASE_P + 0x5, 0x714019, SAMemory::HookType::Jump);

        // Custom stars time
        *(uint8_t*)(0x713D2C) = nStarsHourStart;
        *(uint8_t*)(0x713D3A) = nStarsHourStart;
        *(uint8_t*)(0x713D44) = nStarsHourStart;
        *(uint8_t*)(0x713D31) = nStarsHourLast;
        *(uint8_t*)(0x713D3F) = nStarsHourLast;

        if (bDrawFallingStar)
        {
            // Fix shooting stars if we dont have a SilentPatch
            SAMemory::InjectHook(0x714625, ShootingStars_v10_Patch, SAMemory::HookType::Call);
        }
        else
        {
            // As we have falling stars already, give an ability to turn them off
            SAMemory::InjectHook(0x71438C, 0x714639, SAMemory::HookType::Jump);
        }

        break;

    default: return false;
    }

    return true;
}