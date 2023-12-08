#include "pch.h"

bool DoStarrySkiesSA();
bool DoStarrySkiesVC();

void InitializeThoseStars()
{
    // Keeps stars always the same
    srand(0xBEEF);

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
    srand(time(NULL));
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        if (!DoStarrySkiesSA())
        {
            if (!DoStarrySkiesVC())
            {
                MessageBoxA(NULL, "StarrySkies is not supported in this game!\n \nList of supported games:\n- GTA:SA v1.0 US", "StarrySkies PC", MB_ICONERROR);
                return FALSE;
            }
        }
        InitializeThoseStars();
        // MessageBoxA(NULL, "StarrySkies has been loaded! :)", "StarrySkies PC", MB_OK);
        break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}