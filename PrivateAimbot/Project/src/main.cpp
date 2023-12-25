#include "../Headers/Vector3.h"
#include "../Headers/LocalPlayer.h"
#include "../Headers/mem.h"
#include "../Headers/Player.h"




int main()
{

	AttachProcess(L"cs2.exe");
	uintptr_t Client = ModuleAddress(L"client.dll");

	

	
	while (true)
	{
		if (!GetAsyncKeyState(VK_LSHIFT))
			continue;
		
		uintptr_t LocalPlayer = GetLocalPlayer(Client             );
		Vector3 LocalAngles   = GetLocalAngles(Client, LocalPlayer);
		int LocalPlayerHealth = GetLocalHealth(Client, LocalPlayer);
		int LocalPlayerteam   = GetTeam       (Client, LocalPlayer);
		float aimPunch = GetAimPunch(LocalPlayer);
		int IfShooting = rpm<int>(LocalPlayer + offsets::m_iShotsFired);
	
		
		auto Fov = 15.f;
		Vector3 BestAngles;


		uintptr_t EntityList = rpm<uintptr_t>(Client + offsets::dwEntityList);


		for (int i = 1; i < 32; i++)
		{
		
			uintptr_t listEntry = rpm<uintptr_t>(EntityList + (8 * (i & 0x7FFF) >> 9) + 0x10);   // = EntityList + 10;



			if (!listEntry)
			continue;

			uintptr_t player = rpm<uintptr_t>(listEntry + 0x78 * (i & 0x1FF)); //الاعب رقم 2     


		if (!player)
		 continue;

			uint32_t playerPawn = rpm<uint32_t>(player + offsets::m_hPlayerPawn);   //اللاعب ال2 زائد البون



			uintptr_t listEntry2 = rpm<uintptr_t>(EntityList + 0x8 * ((playerPawn & 0x7FFF) >> 9) + 0x10); //10 SAME THING AS BEFORE LOL (ListEntry)


			uintptr_t pCSPPlayerPawn = rpm<uintptr_t>(listEntry2 + 0x78 * (playerPawn & 0x1FF));

		

			
			int PlayersHealth = GetPlayerHealth(Client, pCSPPlayerPawn);

			if (PlayersHealth < 0.1 || PlayersHealth > 100)
				continue;

			int PlayerTeam = GetPlayerTeam(Client, pCSPPlayerPawn);

		  if (PlayerTeam == LocalPlayerteam)
				continue;

			
			Vector3 LocalHeadPos = LocalEyePos(Client, LocalPlayer);


			Vector3 PlayerHead = GetPlayerHeadPos(Client, pCSPPlayerPawn);
			
			const auto Deltalul = GetLocalFov(PlayerHead, LocalHeadPos, Client);


	         auto LocalFov = hypot(Deltalul.x, Deltalul.y);

			



		    if (LocalFov < Fov)
	        {
			  Fov = LocalFov;
			  BestAngles = CalculateAngle(LocalHeadPos, PlayerHead, Client, LocalPlayer);
	        }
			
		}
		if (!BestAngles.isZero())
		{	
			wpm<Vector3>(Client + offsets::ViewAngles, LocalAngles + BestAngles);
			if (IfShooting == 0)
			{
				wpm<Vector3>(LocalPlayer + offsets::m_aimPunchAngle, aimPunch / 2);
			}
			
		}
		

	}

}