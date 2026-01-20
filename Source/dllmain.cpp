#include "pch.h"
#include <stdio.h>

bool DoStarrySkiesSA();
bool DoStarrySkiesVC();
bool DoStarrySkiesIII();

__declspec(dllexport) int GetMyVersion()
{
    return 0x010301; // 1.3.1
}

void InitializeThoseStars()
{
    // WideFix
    hWideFix = GetModuleHandleA("GTAVC.WidescreenFix.asi");
    if (!hWideFix) hWideFix = GetModuleHandleA("GTA3.WidescreenFix.asi");
    if (!hWideFix) hWideFix = GetModuleHandleA("GTASA.WidescreenFix.asi");
    if (!hWideFix) bWideFix = false;

    // Config Moment
    int beefSeed = 0xBEEF;
    char szConfigVar[32] { 0 };
    if (GetPrivateProfileStringA("Preferences", "SmallestStarsSize", nullptr, szConfigVar, sizeof(szConfigVar), ".\\" CONFIG_FILENAME ".ini") > 0)
    {
        fSmallStars = (float)atof(szConfigVar);
    }
    if (GetPrivateProfileStringA("Preferences", "MiddleStarsSize", nullptr, szConfigVar, sizeof(szConfigVar), ".\\" CONFIG_FILENAME ".ini") > 0)
    {
        fMiddleStars = (float)atof(szConfigVar);
    }
    if (GetPrivateProfileStringA("Preferences", "BiggestStarsSize", nullptr, szConfigVar, sizeof(szConfigVar), ".\\" CONFIG_FILENAME ".ini") > 0)
    {
        fBiggestStars = (float)atof(szConfigVar);
    }
    if (GetPrivateProfileStringA("Preferences", "BiggestStarsChance", nullptr, szConfigVar, sizeof(szConfigVar), ".\\" CONFIG_FILENAME ".ini") > 0)
    {
        fBiggestStarsSpawnChance = (float)atof(szConfigVar);
    }
    beefSeed = GetPrivateProfileIntA("Preferences", "StarsSeed", beefSeed, ".\\" CONFIG_FILENAME ".ini");
    if (GetPrivateProfileIntA("Preferences", "ForceOffWideStarsFix", 0, ".\\" CONFIG_FILENAME ".ini") != 0) bWideFix = false;
    bDisableStars = (bool)GetPrivateProfileIntA("Preferences", "DisableStars", bDisableStars, ".\\" CONFIG_FILENAME ".ini");
    bDrawEasterEgg = (bool)GetPrivateProfileIntA("Preferences", "ShowEasterEgg", bDrawEasterEgg, ".\\" CONFIG_FILENAME ".ini");
    if (GetPrivateProfileStringA("Preferences", "EasterEggStarsScale", nullptr, szConfigVar, sizeof(szConfigVar), ".\\" CONFIG_FILENAME ".ini") > 0)
    {
        fRockStarEasterScale = (float)atof(szConfigVar);
    }
    bDrawFallingStar = (bool)GetPrivateProfileIntA("Preferences", "ShowFallingStars", bDrawFallingStar, ".\\" CONFIG_FILENAME ".ini");
    bForceDisableFallingStar = (bool)GetPrivateProfileIntA("Preferences", "ForceOffFallingStars", bForceDisableFallingStar, ".\\" CONFIG_FILENAME ".ini");
    bForceInteriorStars = (bool)GetPrivateProfileIntA("Preferences", "StarsInInteriors", bForceInteriorStars, ".\\" CONFIG_FILENAME ".ini");

    nStarsHourStart = GetPrivateProfileIntA("Preferences", "StarsStartHour", nStarsHourStart, ".\\" CONFIG_FILENAME ".ini");
    nStarsHourLast = GetPrivateProfileIntA("Preferences", "StarsLastHour", nStarsHourLast, ".\\" CONFIG_FILENAME ".ini");
    if (nStarsHourStart <= nStarsHourLast || nStarsHourStart > 23 || nStarsHourLast > 23)
    {
        nStarsHourStart = 22;
        nStarsHourLast = 5;
    }

    // Keeps stars always the same
    srand(++beefSeed);

    for (int side = 0; side < SSidesCount; ++side)
    {
        for (int i = 0; i < AMOUNT_OF_SIDESTARS; ++i)
        {
            StarCoorsX[side][i] = 95.0f * RandomIt(-1.0f, 1.0f);

            // Side=4 is when rendering stars directly ABOVE us
            if (side == SSide_Up) StarCoorsY[side][i] = 95.0f * RandomIt(-1.0f, 1.0f);
            else StarCoorsY[side][i] = 95.0f * RandomIt(-0.35f, 1.0f);

            // Smaller chances for a bigger star (this is more life-like)
            if (RandomIt(0.0f, 1.0f) > fBiggestStarsSpawnChance) StarSizes[side][i] = 0.8f * RandomIt(fSmallStars, fBiggestStars);
            else StarSizes[side][i] = 0.8f * RandomIt(fSmallStars, fMiddleStars);
        }
    }

    // Useless calculations in a rendering queue
    for (int i = 0; i < 9; ++i)
    {
        RockStar_StarX[i] *= 90.0f;
        RockStar_StarY[i] *= 80.0f;
        RockStar_StarY[i] += 10.0f;
        RockStar_StarSize[i] *= fRockStarEasterScale;
    }

    // Makes other rand() calls "more random"
    srand((unsigned int)time(NULL));
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        InitializeThoseStars();
        if (!DoStarrySkiesSA() && !DoStarrySkiesVC() && !DoStarrySkiesIII())
        {
            MessageBoxA(NULL, "StarrySkies is not supported in this game!\n \nList of supported games:\n- GTA:SA v1.0 US\n- GTA:VC (any version)\n- GTA:III (any version)", "StarrySkies PC", MB_ICONERROR);
            return FALSE;
        }
      #ifdef _DEBUG
        MessageBoxA(NULL, "StarrySkies has been loaded! :)", "StarrySkies PC", MB_OK);
      #endif
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}