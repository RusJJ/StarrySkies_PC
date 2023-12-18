#include "pch.h"
#include <stdio.h>

bool DoStarrySkiesSA();
bool DoStarrySkiesVC();
bool DoStarrySkiesIII();

__declspec(dllexport) int GetMyVersion()
{
    return 0x010201; // 1.2.1
}

void InitializeThoseStars()
{
    // WideFix
    bWideFix = (!GetModuleHandleA("GTAVC.WidescreenFix.asi") && !GetModuleHandleA("GTA3.WidescreenFix.asi"));

    // Config Moment
    int beefSeed = 0xBEEF;
    char szConfigVar[32];
    if (GetPrivateProfileStringA("Preferences", "SmallestStarsSize", nullptr, szConfigVar, sizeof(szConfigVar), ".\\StarrySkies.ini") > 0)
    {
        fSmallStars = (float)atof(szConfigVar);
    }
    if (GetPrivateProfileStringA("Preferences", "MiddleStarsSize", nullptr, szConfigVar, sizeof(szConfigVar), ".\\StarrySkies.ini") > 0)
    {
        fMiddleStars = (float)atof(szConfigVar);
    }
    if (GetPrivateProfileStringA("Preferences", "BiggestStarsSize", nullptr, szConfigVar, sizeof(szConfigVar), ".\\StarrySkies.ini") > 0)
    {
        fBiggestStars = (float)atof(szConfigVar);
    }
    if (GetPrivateProfileStringA("Preferences", "BiggestStarsChance", nullptr, szConfigVar, sizeof(szConfigVar), ".\\StarrySkies.ini") > 0)
    {
        fBiggestStarsSpawnChance = (float)atof(szConfigVar);
    }
    beefSeed = GetPrivateProfileIntA("Preferences", "StarsSeed", beefSeed, ".\\StarrySkies.ini");
    if (GetPrivateProfileIntA("Preferences", "ForceOffWideStarsFix", 0, ".\\StarrySkies.ini") != 0) bWideFix = false;
    bDisableStars = (bool)GetPrivateProfileIntA("Preferences", "DisableStars", bDisableStars, ".\\StarrySkies.ini");

    nStarsHourStart = GetPrivateProfileIntA("Preferences", "StarsStartHour", nStarsHourStart, ".\\StarrySkies.ini");
    nStarsHourLast = GetPrivateProfileIntA("Preferences", "StarsLastHour", nStarsHourLast, ".\\StarrySkies.ini");
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