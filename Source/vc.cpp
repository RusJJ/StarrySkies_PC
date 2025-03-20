#include "pch.h"

#define _GTA_VC

#define Memory VCMemory
#include "../ModUtils/ScopedUnprotect.hpp"
#include "../ModUtils/MemoryMgr.GTA.h"

CVector& CamPosVC = *AddressByVersion<CVector*>(0x7E46B8, 0x7E46C0, 0x7E36C0);
auto CalcScreenCoorsVC = AddressByVersion<bool (*)(CVector*, CVector*, float*, float*, bool)>(0x5778B0, 0x5778D0, 0x5777A0);
auto RenderBufferedOneXLUSpriteVC = AddressByVersion<void(*)(CVector, float, float, uint8_t, uint8_t, uint8_t, short, float, uint8_t)>(0x577350, 0x577370, 0x577240);
auto RwIm3DTransformVC = *AddressByVersion<void* (*)(RwIm3DVertex*, uint32_t, CMatrix*, uint32_t)>(0x65AE90, 0x65AEE0, 0x659E40);
auto RwIm3DRenderIndexedPrimitiveVC = *AddressByVersion<void* (*)(int, int16_t*, int)>(0x65AF90, 0x65AFE0, 0x659F40);
auto RwIm3DEndVC = *AddressByVersion<int (*)()>(0x65AF60, 0x65AFB0, 0x659F10);
auto RwRenderStateSetVC = AddressByVersion<int(*)(int, void*)>(0x649BA0, 0x649BF0, 0x648B50);
auto FlushSpriteBufferVC = AddressByVersion<void(*)()>(0x577790, 0x5777B0, 0x577680);
uint8_t& ms_nGameClockMinutesVC = *AddressByVersion<uint8_t*>(0xA10B92, 0xA10B9B, 0xA0FB9C);
uint8_t& ms_nGameClockHoursVC = *AddressByVersion<uint8_t*>(0xA10B6B, 0xA10B74, 0xA0FB75);
float& FoggynessVC = *AddressByVersion<float*>(0x94DDC0, 0x94DDC8, 0x94CDC8);
float& CloudCoverageVC = *AddressByVersion<float*>(0x974BE8, 0x974BF0, 0x973BF0);
float& ms_fAspectRatioVC = *AddressByVersion<float*>(0x94DD38, 0x94DD40, 0x94CD40);
uint32_t& m_snTimeInMillisecondsVC = *AddressByVersion<uint32_t*>(0x974B2C, 0x974B34, 0x973B34);
int16_t& NewWeatherTypeVC = *AddressByVersion<int16_t*>(0xA10A2E, 0xA10A36, 0xA0FA36);

static void StarrySkies_Patch()
{
    if (bDisableStars) return;

    CVector ScreenPos, WorldPos, WorldStarPos;
    float SZ, SZX, SZY;

    float intensity = 255.0f - 255.0f * fmaxf(CloudCoverageVC, FoggynessVC);
    if (intensity <= 0) return;

    if (ms_nGameClockHoursVC == nStarsHourStart) intensity *= 0.01666666666f * ms_nGameClockMinutesVC;
    else if (ms_nGameClockHoursVC == nStarsHourLast) intensity *= 0.01666666666f * (60 - ms_nGameClockMinutesVC);

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
    FlushSpriteBufferVC();

    if (bDrawEasterEgg)
    {
        for (int i = 0; i < 9; ++i)
        {
            SZ = RockStar_StarSize[i];
            WorldPos = CamPosVC;
            WorldPos.x += 100.0f;
            WorldPos.y -= RockStar_StarX[i];
            WorldPos.z += RockStar_StarY[i];

            if (CalcScreenCoorsVC(&WorldPos, &ScreenPos, &SZX, &SZY, false))
            {
                if (bWideFix) SZX /= ms_fAspectRatioVC;

                uint8_t brightness = (uint8_t)((1.0f - 0.015f * (rand() % 32)) * intensity);
                RenderBufferedOneXLUSpriteVC(ScreenPos, SZX * SZ, SZY * SZ, brightness, brightness, brightness, 255, 1.0f / ScreenPos.z, 255);
            }
        }

        SZ = 5.0f;
        WorldPos = CamPosVC + RockStar_MainStarOff;
        if (CalcScreenCoorsVC(&WorldPos, &ScreenPos, &SZX, &SZY, false))
        {
            if (bWideFix) SZX /= ms_fAspectRatioVC;

            uint8_t brightness = (uint8_t)((1.5f - 0.00156f * (rand() % 128)) * intensity);
            RenderBufferedOneXLUSpriteVC(ScreenPos, SZX * SZ, SZY * SZ, brightness, brightness, brightness, 255, 1.0f / ScreenPos.z, 255);
        }
        FlushSpriteBufferVC();
    }

    if (bDrawFallingStar && (NewWeatherTypeVC == 0 || NewWeatherTypeVC == 4))
    {
        uint32_t v36 = (m_snTimeInMillisecondsVC & 0x1FFF);
        if (v36 < 800)
        {
            float v43 = (400 - v36) + (400 - v36);
            Skies_TempBufferRenderVertices[0].color = 0xE1FFFFFF;
            Skies_TempBufferRenderVertices[1].color = 0x00FFFFFF;

            uint32_t v37 = (m_snTimeInMillisecondsVC >> 13) & 0x3F;
            CVector starScale{ 0.1f * (v37 % 7 - 3), 0.1f * ((m_snTimeInMillisecondsVC >> 13) - 4), 1.0f };
            starScale.Normalise();
            CVector starDir{ (float)(v37 % 9 - 5), (float)(v37 % 10 - 5), 0.1f };
            starDir.Normalise();

            WorldPos = CamPosVC + starDir * 1000.0f;

            Skies_TempBufferRenderVertices[0].pos = WorldPos + starScale * v43;
            Skies_TempBufferRenderVertices[1].pos = WorldPos + starScale * (v43 + 50.0f);

            RwRenderStateSetVC(1, (void*)0); // FIX_BUGS
            RwRenderStateSetVC(10, (void*)5);
            RwRenderStateSetVC(11, (void*)6);

            if (RwIm3DTransformVC(Skies_TempBufferRenderVertices, 2, NULL, 0x10 | 0x8))
            {
                RwIm3DRenderIndexedPrimitiveVC(2, pShootingStarIndices, 2);
                RwIm3DEndVC();
            }
        }

        // Original render states that are used for clouds
        RwRenderStateSetVC(10, (void*)2);
        RwRenderStateSetVC(11, (void*)2);
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

        // Fixing a rainbow!
        *(uint8_t*)(0x540C4A + 1) = 0x85;

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

        // Fixing a rainbow!
        *(uint8_t*)(0x540C6A + 1) = 0x85;

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

        // Fixing a rainbow!
        *(uint8_t*)(0x540B3A + 1) = 0x85;

        break;

    default: return false;
    }

    return true;
}