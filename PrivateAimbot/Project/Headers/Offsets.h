#pragma once
#include <cstddef>

namespace offsets
{
	//Client
	constexpr std::ptrdiff_t dwEntityList = 0x17C1960;
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x16C8F48;
	constexpr std::ptrdiff_t dwForceJump = 0x16C2330; //2
	constexpr auto ViewAngles = 0x1880DD0;
	constexpr auto dwViewMatrix = 0x1820160;


	//Entity
	constexpr std::ptrdiff_t m_iHealth = 0x32C;
	constexpr std::ptrdiff_t m_fFlags = 0x3C8;
	constexpr auto VecOrigin = 0x1224;
	constexpr auto TeamNum = 0x3BF;
	constexpr std::ptrdiff_t m_lifeState = 0x330;
	constexpr std::ptrdiff_t m_vecViewOffset = 0xC48; // Vector
	constexpr std::ptrdiff_t m_aimPunchAngle = 0x171C;
	constexpr std::ptrdiff_t m_iShotsFired = 0x1420;
	constexpr std::ptrdiff_t m_hPlayerPawn = 0x7EC;
}