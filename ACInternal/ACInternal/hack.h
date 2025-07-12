#pragma once
#include <Windows.h>
#include <vector>
#include "entity.h"
#include "w2s.h"

#define local_player_pointer  0x17E0A8
#define local_player_pointer1 0x18AC00
#define local_player_pointer2 0x195404
#define player_count_pointer 0x18AC0C
#define entity_list_pointer 0x18AC04
#define player_viewmatrix 0x17DFD0


extern bool bAimbot;
extern bool bEnableSmoothing;
extern float aimbotSmoothing;
extern bool bEsp;
extern bool bName;
extern bool bHealth;
extern bool bArmor;
extern bool bShowTeam;
extern bool bDrawHead;
extern float aimbotFOV;
extern int targetSelectionIndex;
extern const char* targetSelectionItems[3];
extern bool bTriggerbot;
extern float triggerbotdelay;
extern bool bNoRecoil;
extern bool bBunnyHop;
extern float boxEspColor[4];
extern float boxTeamEspColor[4];
extern bool bAutomatic;
extern std::vector<entity*> gEntityList;
void UpdateEntityList();
DWORD WINAPI UpdateEntityListThread(LPVOID lpParam);
void RunHack();