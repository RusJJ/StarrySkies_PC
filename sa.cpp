#include "pch.h"

#define _GTA_SA

#include "ModUtils/MemoryMgr.h"
#include "ModUtils/MemoryMgr.GTA.h"

static char* TheCamera = (char*) *AddressByVersion<int32_t**>(0xB6F028, 0, 0);

extern "C" void StarrySkies_Patch(float intensity)
{
    CVector ScreenPos, WorldPos, WorldStarPos;
    CVector& CamPos = *CamPosPtr;
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

            if (CalcScreenCoors(&WorldStarPos, &ScreenPos, &SZX, &SZY, false, true))
            {
                uint8_t brightness = (1.0 - 0.015f * (rand() & 0x1F)) * 1.0f; //intensity;
                RenderBufferedOneXLUSprite(ScreenPos, SZX * SZ, SZY * SZ,
                    brightness, brightness, brightness, 255, 1.0f / ScreenPos.z, 255);
            }
        }
    }
}

bool DoStarrySkiesSA()
{
	const int8_t version = Memory::GetVersion().version;
	switch (version)
	{
	case 0:
        CamPosPtr = *(CVector**)(TheCamera + 68);
		Memory::InjectHook(0x713DDB, StarrySkies_Patch, Memory::HookType::Call);
		Memory::InjectHook(0x713DDB + 0x5, 0x714019, Memory::HookType::Jump);
		break;

	default: return false;
	}

    *(void**)&CalcScreenCoors = (void*) *AddressByVersion<int32_t**>(0x70CE30, 0, 0);
    *(void**)&RenderBufferedOneXLUSprite = (void*)*AddressByVersion<int32_t**>(0x70E4A0, 0, 0);

	return true;
}