#pragma once
#include <iostream>
#include <numbers>
#include "Offsets.h"
#include "Vector3.h"
#include "mem.h"

uintptr_t GetPlayers(uintptr_t Client, int i)
{
	uintptr_t EntityEntry = rpm<uintptr_t>(Client + offsets::dwEntityList + i * 8);

	return EntityEntry;
}

int GetPlayerHealth(uintptr_t Client, uintptr_t PlayerNumber)
{
	int Health = rpm<int>(PlayerNumber + offsets::m_iHealth);

	return Health;
}

int GetPlayerTeam(uintptr_t Client, uintptr_t PlayerNumber)
{
	int Team = rpm<int>(PlayerNumber + offsets::TeamNum);

	return Team;
}

Vector3 GetPlayerOrigin(uintptr_t Client, uintptr_t PlayerNumber)
{
	Vector3 LocalOrigin = rpm<Vector3>(PlayerNumber + offsets::VecOrigin);

	return LocalOrigin;
}


Vector3 GetPlayerHeadPos(uintptr_t Client, uintptr_t PlayerNumber)
{

	uintptr_t Playergamescene = rpm<uintptr_t>(PlayerNumber + 0x310);
	uintptr_t bonearray = rpm<uintptr_t>(Playergamescene + 0x160 + 0x80);
	Vector3 PlayerHead = rpm<Vector3>(bonearray + 6 * 32);

	return PlayerHead;
}


