#include "hack.h"
#include "mem.h"
#include "math.h"
#include "hook.h"
#include <imgui.h>
#include <iostream>
bool bAimbot = false;
bool bEnableSmoothing = false;
float aimbotSmoothing = 0.0f;
bool bEsp = false;
bool bName = false;
bool bHealth = false;
bool bArmor = false;
bool bShowTeam = false;
bool bDrawHead = false;
float aimbotFOV = 45.0f;
int targetSelectionIndex = 0;
const char* targetSelectionItems[3] = { "Head", "Neck", "Chest" };
bool bTriggerbot = false;
float triggerbotdelay = 0.0f;
bool bNoRecoil = false;
bool bBunnyHop = false;
float boxEspColor[4] = { 0.9137f, 0.3294f, 0.1255f, 1.0f };
float boxTeamEspColor[4] = { 0.66f, 0.78f, 0.93f, 1.0f };
bool bAutomatic = false;
std::vector<entity*> gEntityList;

void UpdateEntityList() {
	uintptr_t module_base = (uintptr_t)GetModuleHandle(L"ac_client.exe");
	int playerCount = *(int*)(module_base + player_count_pointer);
	uintptr_t entListStructAddr = module_base + entity_list_pointer;
	entity** pEntityList = *reinterpret_cast<entity***>(entListStructAddr);;


	if (!pEntityList)
	{
		return;
	}

	gEntityList.clear();

	for (int i = 1; i < playerCount; i++) {
		entity* ent = pEntityList[i];
		if (ent) {
			gEntityList.push_back(ent);
		}
	}
}


DWORD WINAPI UpdateEntityListThread(LPVOID lpParam)
{
	while (true)
	{
		UpdateEntityList();
		Sleep(16);
	}
	return 0;
}

void Aimbot()
{
	if (!bAimbot)
		return;

}

typedef entity* (__cdecl* tGetCrossHairEnt)();
tGetCrossHairEnt GetCrossHairEnt = nullptr;

void Triggerbot(entity* local_player, uintptr_t module_handle)
{
	if (!bTriggerbot)
		return;

	GetCrossHairEnt = (tGetCrossHairEnt)(module_handle + 0xCC890);

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
	{
		entity* crossHairEnt = GetCrossHairEnt();
		if (crossHairEnt)
		{
			if (crossHairEnt->team != local_player->team && crossHairEnt->entity_health >= 0)
			{
				local_player->entity_attack = true;
			}
		}
		else
		{
			local_player->entity_attack = false;
		}
	}
}

void automatic(entity* local_player)
{
	if (!bAutomatic)
		return;

	static DWORD lastShotTime = 0;
	DWORD now = GetTickCount();
	const DWORD shotDelay = 3;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (now - lastShotTime >= shotDelay)
		{
			local_player->entity_attack = true;
			lastShotTime = now;
		}
		else
		{
			local_player->entity_attack = false;
		}
	}
	else
	{
		local_player->entity_attack = false;
	}
}

void BunnyHop(entity* local_player, uintptr_t module_base)
{
	if (!bBunnyHop)
		return;

	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (local_player->enity_on_floor)
		{
			local_player->enity_jump_next = true;
		}
		else
		{
			local_player->enity_jump_next = false;
		}
	}
	else
	{
		local_player->enity_jump_next = false;
	}
}

void NoRecoil()
{
	if (!bNoRecoil)
		return;

	// C8C51 change to jmp for no spread.


}

void Esp(entity* local_player, float viewmatrix[16], uintptr_t module_base)
{
	if (!bEsp)
		return;

	ImGuiIO& io = ImGui::GetIO();
	int windowWidth = (int)io.DisplaySize.x;
	int windowHeight = (int)io.DisplaySize.y;


	ImU32 color = ImGui::ColorConvertFloat4ToU32(ImVec4(boxEspColor[0], boxEspColor[1], boxEspColor[2], boxEspColor[3]));
	UpdateEntityList();
	for (entity* ent : gEntityList)
	{
		if (!ent)
			continue;

		// Skip the local player.
		if (ent == local_player)
			continue;

		Vector3 headPos = ent->entity_origin;
		Vector3 feetPos = ent->entity_headpos;

		if (ent->entity_health <= 0)
		{
			continue;
		}

		if (!bShowTeam && ent->team == local_player->team)
			continue;


		if (ent->team == local_player->team)
		{
			color = ImGui::ColorConvertFloat4ToU32(ImVec4(boxTeamEspColor[0], boxTeamEspColor[1], boxTeamEspColor[2], boxTeamEspColor[3]));
		}
		else
		{
			color = ImGui::ColorConvertFloat4ToU32(ImVec4(boxEspColor[0], boxEspColor[1], boxEspColor[2], boxEspColor[3]));
		}

		Vector3 screenHead, screenFeet;
		if (!WorldToScreen(headPos, screenHead, viewmatrix, windowWidth, windowHeight))
			continue;
		if (!WorldToScreen(feetPos, screenFeet, viewmatrix, windowWidth, windowHeight))
			continue;

		float dynamicBoxHeight = screenHead.y - screenFeet.y;
		float dynamicBoxWidth = dynamicBoxHeight * 0.5f;

		// Define the corners of the bounding box.
		ImVec2 bottomLeft(screenFeet.x - dynamicBoxWidth / 2, screenHead.y);
		ImVec2 bottomRight(screenFeet.x + dynamicBoxWidth / 2, screenHead.y);
		ImVec2 topLeft(screenFeet.x - dynamicBoxWidth / 2, screenFeet.y + 2);
		ImVec2 topRight(screenFeet.x + dynamicBoxWidth / 2, screenFeet.y + 2);

		const float cornerLength = 10.0f;

		ImDrawList* drawList = ImGui::GetForegroundDrawList();

		drawList->AddLine(ImVec2(topLeft.x - cornerLength, topLeft.y), topLeft, color, 2.0f);
		drawList->AddLine(ImVec2(topLeft.x, topLeft.y - cornerLength), topLeft, color, 2.0f);

		drawList->AddLine(topRight, ImVec2(topRight.x + cornerLength, topRight.y), color, 2.0f);
		drawList->AddLine(ImVec2(topRight.x, topRight.y - cornerLength), topRight, color, 2.0f);

		drawList->AddLine(ImVec2(bottomLeft.x - cornerLength, bottomLeft.y), bottomLeft, color, 2.0f);
		drawList->AddLine(bottomLeft, ImVec2(bottomLeft.x, bottomLeft.y + cornerLength), color, 2.0f);

		drawList->AddLine(bottomRight, ImVec2(bottomRight.x + cornerLength, bottomRight.y), color, 2.0f);
		drawList->AddLine(bottomRight, ImVec2(bottomRight.x, bottomRight.y + cornerLength), color, 2.0f);


		const float barSpacing = 3.0f;
		const float barWidth = 2.0f;

		float healthBarRight = topLeft.x + 1.0f;
		float healthBarLeft = healthBarRight - barWidth;

		float armorBarRight = healthBarLeft - barSpacing;
		float armorBarLeft = armorBarRight - barWidth;

		float barTop = topLeft.y;
		float barBottom = bottomLeft.y;

		ImU32 bgColor = IM_COL32(20, 20, 20, 255);

		float maxVal = 100.0f;
		float healthPerc = (ent->entity_health > maxVal ? maxVal : ent->entity_health) / maxVal;
		float armorPerc = (ent->entity_armor > maxVal ? maxVal : ent->entity_armor) / maxVal;

		float filledHealthHeight = dynamicBoxHeight * healthPerc;
		float filledArmorHeight = dynamicBoxHeight * armorPerc;

		float healthFillTop = barBottom - filledHealthHeight;
		float armorFillTop = barBottom - filledArmorHeight;

		ImU32 healthColor = IM_COL32(144, 238, 144, 255);
		ImU32 armorColor = IM_COL32(100, 149, 237, 255);

		if (bHealth)
		{
			drawList->AddRectFilled(ImVec2(healthBarLeft + 2, barTop), ImVec2(healthBarRight + 2, barBottom), bgColor);
			drawList->AddRectFilled(ImVec2(healthBarLeft + 2, healthFillTop), ImVec2(healthBarRight + 2, barBottom), healthColor);
		}

		if (bArmor)
		{
			drawList->AddRectFilled(ImVec2(healthBarLeft + 8, barTop), ImVec2(healthBarRight + 8, barBottom), bgColor);
			drawList->AddRectFilled(ImVec2(healthBarLeft + 8, armorFillTop), ImVec2(healthBarRight + 8, barBottom), armorColor);
		}

		if (bDrawHead)
		{
			float headCircleRadius = (dynamicBoxWidth / 7.0f) * -1;
			if (local_player->enity_scoping)
			{
				headCircleRadius /= 2;
			}
			drawList->AddCircle(ImVec2(screenHead.x - 2, screenHead.y - 3), headCircleRadius, IM_COL32(255, 255, 255, 255), 0, 1.5f);
		}

		if (bName)
		{
			if (bName)
			{
				char name[17];
				sprintf_s(name, "%s", ent->entity_name);
				ImVec2 textSize = ImGui::CalcTextSize(name);
				float centerX = (bottomLeft.x + bottomRight.x) / 2.0f;
				ImVec2 textPos(centerX - textSize.x / 2.0f, screenFeet.y);
				drawList->AddText(textPos, IM_COL32(255, 255, 255, 255), name);
			}

		}
	}
}


void RunHack()
{
	static bool init = false;
	uintptr_t module_base = (uintptr_t)GetModuleHandle(L"ac_client.exe");
	entity* local_player = *reinterpret_cast<entity**>(module_base + local_player_pointer1);
	float* viewmatrix = reinterpret_cast<float*>(module_base + player_viewmatrix);
	Aimbot();
	Triggerbot(local_player, module_base);
	BunnyHop(local_player, module_base);
	NoRecoil();
	automatic(local_player);
	Esp(local_player, viewmatrix, module_base);
}
