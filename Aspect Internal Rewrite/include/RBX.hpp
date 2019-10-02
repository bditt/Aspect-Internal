#pragma once
#include <Includes.hpp>
#include <Classes.hpp>
#include <Retcheck.hpp>

bool showdebug = false;

void dprint(const char* format, uint32_t address = NULL)
{
	if (showdebug)
		printf(format, address);
}

using GetDataModelFn = void* (__cdecl*)(void* dm);
GetDataModelFn GetDataModel;

DWORD base = (DWORD)GetModuleHandle(0);
DWORD getaddy(int address) {
	return (address - 0x01130000 + base);
}

DWORD reverseaddy(int address) {
	return (address + 0x01130000 - base);
}

namespace RBX
{
	bool ESPEnabled = true;
	bool ESPNames = true;
	bool ESPDistance = true;
	bool ESPBox = true;
	bool ESPLine = true;
	bool ESPHealth = true;
	bool ESPHealthBar = true;
	bool ESPWalkspeed = true;
	bool ESPJumppower = true;
	bool ESPTeamcheck = true;
	bool AimEnabled = true;
	bool AimHead = true;
	bool AimTeamcheck = true;
	bool AimDrawFOV = true;
	bool AimPrediction = false;
	bool TeleKill = true;
	bool TeleMove = true;
	bool Elevator = true;
	bool NoClipEnabled = false;

	int s_height;
	int s_width;
	int AimSmoothAmount = 3;
	int AimFOV = 100;
	int PredictionOffset = 50;
	int TeleMoveAmount = 5;
	int ESPDistanceAmount = 1000;
	int ElevatorHeight = 0;
	int LocalWalkspeed = 16;
	int LocalJumpPower = 50;
	int TeleKillDistance = 25;

	float AimYOffset = 0;
	float AimXOffset = 0;
	float AimPredictionGravity = 196.2f;
	float AimPredictionDivider = 50;
	float AimPredictionDrop = 0.5f;
	float LocalGravity = 196.2;

	const char* AimTypes[] = { "New Aimbot", "Old Aimbot", "3rd Person" };
	const char* AimType = AimTypes[0];

	sol::state lua;

	ImVec4 ESPNameColor = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
	ImVec4 ESPFriendNameColor = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
	ImVec4 ESPDistanceColor = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
	ImVec4 ESPBoxColor = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
	ImVec4 ESPBoxFriendColor = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
	ImVec4 ESPLineColor = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
	ImVec4 ESPHealthColor = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
	ImVec4 ESPHealthBarColor = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
	ImVec4 ESPHealthBarOutlineColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	ImVec4 ESPWalkSpeedColor = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
	ImVec4 ESPJumpPowerColor = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
	ImVec4 AimFOVColor = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);

	uint32_t DataModel, Workspace, Players, LocalPlayer, Camera, LocalCharacter, LocalHumanoid, LocalHead, LocalTorso, LocalHumanoidRootPart;

	RBXDataModel* CDataModel;
	RBXPlayers* CPlayers;
	RBXService* CWorkspace;
	RBXService* CScriptContext;
	RBXPlayer* CLocalPlayer;
	RBXCharacter* CLocalCharacter;
	RBXInstance* CLocalHumanoid;
	RBXInstance* CLocalHead;
	RBXInstance* CLocalTorso;
	RBXInstance* CLocalHumanoidRootPart;

	const float GravityAcceleration = 500.0f, BulletVelocity = 30000.0f;

	float GetBulletDrop(float height, float DepthPlayerTarget)
	{
		float pitch = (atan2(height, DepthPlayerTarget));
		float BulletVelocityXY = BulletVelocity * cos(pitch);
		float Time = DepthPlayerTarget / BulletVelocityXY;
		float TotalVerticalDrop = (0.5f * GravityAcceleration * Time * Time);
		return TotalVerticalDrop;
	}

	float ViewMatrix[4][4];

	std::vector<std::string> FriendList;

	HANDLE RobloxHandle;
	HANDLE UpdateThread;
	HANDLE AimbotThread;
	HANDLE TeleMoveThread;
	HANDLE TeleKillThread;
	HANDLE NoClipThread;

	typedef __int16(__thiscall* Humanoid_SetHipHeight)(uintptr_t Humanoid, float value);
	typedef __int16(__thiscall* Humanoid_SetWalkSpeed)(uintptr_t Humanoid, float value);
	typedef __int16(__thiscall* Humanoid_SetJumpPower)(uintptr_t Humanoid, float value);
	typedef int(__thiscall* Workspace_SetGravity)(uintptr_t Workspace, float value);
	typedef int(__thiscall* Humanoid_ChangeState)(uintptr_t Humanoid, int State);
	typedef int(__thiscall* Humanoid_GetHealth)(uintptr_t Humanoid);
	typedef int(__thiscall* Humanoid_GetMaxHealth)(uintptr_t Humanoid);
	Humanoid_SetHipHeight sethipheight = (Humanoid_SetHipHeight)Memory::Scan("55 8B EC F3 0F 10 45 ? 56 51 8B F1 F3 0F 11 04 24 E8 ? ? ? ? 83 C4 04");//55 8B EC F3 0F 10 45 ? 56 51 8B F1 F3 0F 11 04 24 E8 ? ? ? ? 83 C4 04 
	Humanoid_SetWalkSpeed setwalkspeed = (Humanoid_SetWalkSpeed)Memory::Scan("55 8B EC F3 0F 10 45 ? F3 0F 5F 05 ? ? ? ? 56 8B F1 8B 96 ? ? ? ?");
	Humanoid_SetJumpPower setjumppower = (Humanoid_SetJumpPower)getaddy(0x0000);//(Humanoid_SetJumpPower)Memory::Scan("E8 ? ? ? ? 0F B6 87 ? ? ? ? 8B CE 50 E8 ? ? ? ? F3 0F 10 87 ? ? ? ?");
	Workspace_SetGravity setgravity = (Workspace_SetGravity)Memory::Scan("55 8B EC 80 3D ? ? ? ? ? F3 0F 10 4D ? 56 8B F1 75 1F 0F 57 C0");
	//ChangeStateFunc.SetFunc(Memory::Scan("55 8B EC 64 A1 ? ? ? ? 6A FF 68 ? ? ? ? 50 64 89 25 ? ? ? ? 83 EC 08 56 6A 00 8B F1 E9 ? ? ? ? 8D BF ? ? ? ? "), false, true, CallingConventions::CallingConvention::Thiscall);
	Humanoid_ChangeState ChangeState = (reinterpret_cast<Humanoid_ChangeState>(getaddy(0x01742930)));//Memory::Scan("55 8B EC 64 A1 ? ? ? ? 6A FF 68 ? ? ? ? 50 64 89 25 ? ? ? ? 83 EC 08 56 6A 00 8B F1 E9 ? ? ? ? 8D BF ? ? ? ? "));
	Humanoid_GetHealth GetCurrentHealth = (Humanoid_GetHealth)getaddy(0x17499F0);//01749FF0 Memory::Scan("55 8B EC 51 8B 81 C8 01  00 00 8B 08 33 C8 89 4D");
	Humanoid_GetMaxHealth GetMaxHealth = (Humanoid_GetMaxHealth)getaddy(0x01749FF0); //Memory::Scan("55 8B EC 51 8B 81 ? ? ? ? 8B 08 33 C8 89 4D FC D9 45 FC 8B E5 5D C3 A7");
	void ConsoleTxtCol(std::string col)
	{
		if (col == "black") {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 0);
		}
		else if (col == "blue") {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 1);
		}
		else if (col == "green") {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 2);
		}
		else if (col == "cyan") {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 3);
		}
		else if (col == "red") {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 4);
		}
		else if (col == "magenta") {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 5);
		}
		else if (col == "brown") {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 6);
		}
		else if (col == "lightgray") {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 7);
		}
		else if (col == "darkgray") {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 8);
		}
		else if (col == "lightblue") {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 9);
		}
		else if (col == "lightgreen") {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 10);
		}
		else if (col == "lightcyan") {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 11);
		}
		else if (col == "lightred") {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 12);
		}
		else if (col == "lightmagenta") {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 13);
		}
		else if (col == "yellow") {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 14);
		}
		else if (col == "white") {
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 15);
		}
	}

	bool CheckFriend(const std::string& name)
	{
		for (int i = 0; i < FriendList.size(); i++)
		{
			if (name.compare(FriendList[i].c_str()) == 0)
				return true;
		}

		return false;
	}


	bool AddFriend(const std::string& name)
	{
		// let's check if he's not in the list 
		for (auto it = FriendList.begin(); it != FriendList.end(); it++)
		{
			if (!name.compare((*it).c_str()))
				return true; // already in the list, don't add him
		}

		FriendList.push_back(name);

		return false;
	}

	bool RemoveFriend(const std::string& name)
	{
		for (auto it = FriendList.begin(); it != FriendList.end(); it++)
		{
			if (!name.compare((*it).c_str()))
			{
				FriendList.erase(it);
				return true; // we found someone, remove him
			}
		}

		// sorry, we haven't found him
		return false;
	}

	uint32_t getParent(uint32_t Instance)
	{
		return *(uint32_t*)(Instance + 0x38);
	}

	vec2 GetViewPortSize()
	{
		vec2 ViewPort;
		ViewPort.x = Workspace + 0xF0;
		ViewPort.y = Workspace + 0xF4;
		return ViewPort;
	}

	auto RetcheckBypass(LPVOID Addr)
	{
		*(BYTE*)(Addr) = 0xEB;
		//Mem.Write<BYTE>(Addr, 0xEB);
		return [&] {
			//Mem.Write<BYTE>(Addr, 0x72);
			*(BYTE*)(Addr) = 0x72;
		};
	}

	static std::string GetName(uintptr_t instance)
	{
		return **reinterpret_cast<std::string * *>(instance + 0x2C);
	}

	static std::string GetClassBroke(uintptr_t instance)
	{
		return **reinterpret_cast<std::string * *>(instance + 16);
	}

	static std::string* GetClass2(uint32_t Instance) {
		if (Instance) {
			__try {
				return (std::string*)(*(uint32_t*)(Instance + 16));
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {}
		}
		return (std::string*)"0";
	}

	std::string* GetClass(uint32_t self) {
		if (self) {
			__try {
				return (std::string*)(*(uint32_t(**)(void))(*(uint32_t*)self + 16))();
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {}
		}
		return (std::string*)"0";
	}

	std::vector<uint32_t> GetChildren(uint32_t Instance) {
		std::vector<uint32_t> Children;
		uint32_t StartOfChildren = *(uint32_t*)(Instance + 0x30);
		if (!StartOfChildren)
			return Children;
		uint32_t EndOfChildren = *(uint32_t*)(StartOfChildren + 4);
		for (uint32_t i = *(uint32_t*)StartOfChildren; i != EndOfChildren; i += 8)
		{
			Children.push_back(*(uint32_t*)i);
		}
		return Children;
	}

	uint32_t FindFirstChild(uint32_t Instance, std::string Name) {
		try {
			uint32_t StartOfChildren = *(uint32_t*)(Instance + 0x30);
			if (!StartOfChildren)
				return 0;
			uint32_t EndOfChildren = *(uint32_t*)(StartOfChildren + 4);
			for (uint32_t i = *(uint32_t*)StartOfChildren; i != EndOfChildren; i += 8)
			{
				if (strstr(GetName(*(uint32_t*)i).c_str(), Name.c_str()))
				{
					return *(uint32_t*)i;
				}
			}
			return 0;
		}
		catch (...) {};
	}

	uint32_t FindFirstChildOfClass(uint32_t Instance, std::string ClassName) {
		try {
			uint32_t StartOfChildren = *(uint32_t*)(Instance + 0x30);
			if (!StartOfChildren)
				return 0;
			uint32_t EndOfChildren = *(uint32_t*)(StartOfChildren + 4);
			for (uint32_t i = *(uint32_t*)StartOfChildren; i != EndOfChildren; i += 8)
			{
				//std::cout << "Class Name: " << GetClass(*(uint32_t*)i).c_str() << std::endl;
				if (strstr(GetClass(*(uint32_t*)i)->c_str(), ClassName.c_str()))
				{
					return *(uint32_t*)i;
				}
			}
			return 0;
		}
		catch (...) {};
	}

	uint32_t GetLocalPlayerLua()
	{
		return *(uint32_t*)(Players + 0xCC);
	}

	uint32_t GetLocalPlayer(uint32_t Players)
	{
		return *(uint32_t*)(Players + 0xCC);
	}

	uint32_t GetCharacter(uint32_t Player)
	{
		return *(uint32_t*)(Player + 0x5C);
	}

	int GetHumanoidState(uint32_t Humanoid)
	{
		uint32_t Pointer1 = *(uint32_t*)(Humanoid + 0x6A4);
		return *(int*)(Pointer1 + 0x28);
	}

	int GetUserID(uint32_t Player)
	{
		return *(int*)(Player + 0x158);
	}

	int GetTeamID(DWORD Player)
	{
		return *(int*)(Player + 0x8C);
	}

	float GetXPosition(DWORD Instance)
	{
		uint32_t Pointer1 = *(uint32_t*)(Instance + 0x98);
		uint32_t Pointer2 = *(uint32_t*)(Pointer1 + 0x80);
		float XCord = *(float*)(Pointer2 + 0xD4);
		return XCord;
	}

	float GetYPosition(DWORD Instance)
	{
		uint32_t Pointer1 = *(uint32_t*)(Instance + 0x98);
		uint32_t Pointer2 = *(uint32_t*)(Pointer1 + 0x80);
		float YCord = *(float*)(Pointer2 + 0xD8);
		return YCord;
	}

	float GetZPosition(DWORD Instance)
	{
		uint32_t Pointer1 = *(uint32_t*)(Instance + 0x98);
		uint32_t Pointer2 = *(uint32_t*)(Pointer1 + 0x80);
		float ZCord = *(float*)(Pointer2 + 0xDC);
		return ZCord;
	}

	float GetXPosition(DWORD Instance, float value)
	{
		uint32_t Pointer1 = *(uint32_t*)(Instance + 0x98);
		uint32_t Pointer2 = *(uint32_t*)(Pointer1 + 0x80);
		return *(float*)(Pointer2 + 0xD4);
	}

	float GetYPosition(DWORD Instance, float value)
	{
		uint32_t Pointer1 = *(uint32_t*)(Instance + 0x98);
		uint32_t Pointer2 = *(uint32_t*)(Pointer1 + 0x80);
		return *(float*)(Pointer2 + 0xD8);
	}

	float GetZPosition(DWORD Instance, float value)
	{
		uint32_t Pointer1 = *(uint32_t*)(Instance + 0x98);
		uint32_t Pointer2 = *(uint32_t*)(Pointer1 + 0x80);
		return *(float*)(Pointer2 + 0xDC);
	}

	void SetXPosition(DWORD Instance, float value)
	{
		uint32_t Pointer1 = *(uint32_t*)(Instance + 0x98);
		uint32_t Pointer2 = *(uint32_t*)(Pointer1 + 0x80);
		*(float*)(Pointer2 + 0xD4) = value;
	}

	void SetYPosition(DWORD Instance, float value)
	{
		uint32_t Pointer1 = *(uint32_t*)(Instance + 0x98);
		uint32_t Pointer2 = *(uint32_t*)(Pointer1 + 0x80);
		*(float*)(Pointer2 + 0xD8) = value;
	}

	void SetZPosition(DWORD Instance, float value)
	{
		uint32_t Pointer1 = *(uint32_t*)(Instance + 0x98);
		uint32_t Pointer2 = *(uint32_t*)(Pointer1 + 0x80);
		*(float*)(Pointer2 + 0xDC) = value;
	}

	
	float GetCurrentHealthOld(DWORD Humanoid)
	{
		try
		{
			uint32_t PointerBase = *(uint32_t*)(Humanoid + 0x1C8);
			uint32_t Pointer2 = *(uint32_t*)(PointerBase);
			uint32_t DecryptedPtr = PointerBase ^ Pointer2;
			return *reinterpret_cast<float*>(&DecryptedPtr);
		}
		catch (std::exception e)
		{
			std::cout << "Health Error: " << e.what() << std::endl;
		}
	}

	float GetMaxHealthOld(DWORD Humanoid)
	{
		try
		{
			uint32_t PointerBase = *(uint32_t*)(Humanoid + 0x1CC);
			uint32_t Pointer2 = *(uint32_t*)(PointerBase);
			uint32_t DecryptedPtr = PointerBase ^ Pointer2;
			return *reinterpret_cast<float*>(&DecryptedPtr);
		}
		catch (std::exception e)
		{
			std::cout << "Health Error: " << e.what() << std::endl;
		}
	}
	

	float GetWalkSpeed(DWORD Humanoid)
	{
		try
		{
			uint32_t PointerBase = *(uint32_t*)(Humanoid + 440);
			uint32_t Pointer2 = *(uint32_t*)(PointerBase);
			uint32_t DecryptedPtr = Pointer2 - PointerBase;
			return *reinterpret_cast<float*>(&DecryptedPtr);
		}
		catch (std::exception e)
		{
			std::cout << "Health Error: " << e.what() << std::endl;
		}
	}

	float GetJumpPower(DWORD Humanoid)
	{
		try
		{
			uint32_t PointerBase = *(uint32_t*)(Humanoid + 464);
			uint32_t Pointer2 = *(uint32_t*)(PointerBase);
			uint32_t DecryptedPtr = PointerBase ^ Pointer2;
			return *reinterpret_cast<float*>(&DecryptedPtr);
		}
		catch (std::exception e)
		{
			std::cout << "Health Error: " << e.what() << std::endl;
		}
	}

	vec3 GetPartPosition(uint32_t Part)
	{
		uint32_t Pointer1 = *(uint32_t*)(Part + 0x98);
		uint32_t Pointer2 = *(uint32_t*)(Pointer1 + 0x80);
		return *(vec3*)(Pointer2 + 0xD4);
	}

	vec3 GetPlayerVelocity(uint32_t Player)
	{
		uint32_t Pointer1 = *(uint32_t*)(Player + 0x98);
		uint32_t Pointer2 = *(uint32_t*)(Pointer1 + 0x80);
		return *(vec3*)(Pointer2 + 0xE0);
	}

	void SetLocalPlayerPosition(float x, float y, float z)
	{
		RBX::SetXPosition(RBX::LocalHead, x);
		RBX::SetYPosition(RBX::LocalHead, y);
		RBX::SetZPosition(RBX::LocalHead, z);
	}

	void GetViewMatrix()
	{
		uint32_t Link2RenderView = *(uint32_t*)(reinterpret_cast<uintptr_t>(CDataModel) + 0x74);
		//dprint("Link2RenderView: 0x%X\n", Link2RenderView);
		uint32_t RenderView = *(uint32_t*)(Link2RenderView + 0x1C);
		//dprint("RenderView: 0x%X\n", RenderView);
		uint32_t AvEngine = *(uint32_t*)(RenderView + 0x8);
		//dprint("AvEngine: 0x%X\n", AvEngine);
		ViewMatrix[0][0] = *(float*)(AvEngine + 0xA0);
		ViewMatrix[0][1] = *(float*)(AvEngine + 0xA4);
		ViewMatrix[0][2] = *(float*)(AvEngine + 0xA8);
		ViewMatrix[0][3] = *(float*)(AvEngine + 0xAC);
		ViewMatrix[1][0] = *(float*)(AvEngine + 0xB0);
		ViewMatrix[1][1] = *(float*)(AvEngine + 0xB4);
		ViewMatrix[1][2] = *(float*)(AvEngine + 0xB8);
		ViewMatrix[1][3] = *(float*)(AvEngine + 0xBC);
		ViewMatrix[2][0] = *(float*)(AvEngine + 0xC0);
		ViewMatrix[2][1] = *(float*)(AvEngine + 0xC4);
		ViewMatrix[2][2] = *(float*)(AvEngine + 0xC8);
		ViewMatrix[2][3] = *(float*)(AvEngine + 0xCC);
		ViewMatrix[3][0] = *(float*)(AvEngine + 0xD0);
		ViewMatrix[3][1] = *(float*)(AvEngine + 0xD4);
		ViewMatrix[3][2] = *(float*)(AvEngine + 0xD8);
		ViewMatrix[3][3] = *(float*)(AvEngine + 0xDC);
	}

	void LoadAddresses()
	{
		dprint("Loading Aspect\n");
		GetDataModel = reinterpret_cast<GetDataModelFn>(Memory::Scan("8D 45 E0 C7 45 ? ? ? ? ? 50 E8 ? ? ? ? 83 7D E0 00", CALL_REL_32, 11));
		unsigned char dm[8];
		DataModel = *reinterpret_cast<std::uintptr_t*>(GetDataModel(dm)) + 0x44;
		dprint("DataModel: 0x%X\n", DataModel);
		std::cout << "Casting DM!" << std::endl;
		CDataModel = reinterpret_cast<RBXDataModel*>(DataModel);
		std::cout << "DM Name: " << CDataModel->Name << std::endl;
		std::cout << "DM ClassName: " << CDataModel->ClassDescriptor->ClassName << std::endl;
		CScriptContext = CDataModel->FindFirstChildOfClass<RBXService>("ScriptContext");
		std::cout << "CScriptContext: " << CScriptContext->Name << std::endl;
		dprint("ScriptContext: 0x%X\n", reinterpret_cast<uintptr_t>(CScriptContext));
		CPlayers = CDataModel->FindFirstChildOfClass<RBXPlayers>("Players");
		printf("CPlayers: 0x%X\n", reinterpret_cast<uintptr_t>(CPlayers));
		std::cout << "CPlayers: " << CPlayers->Name << std::endl;
		CLocalPlayer = CPlayers->GetLocalPlayer();
		printf("CLocalPlayer: 0x%X\n", reinterpret_cast<uintptr_t>(CLocalPlayer));
		std::cout << "CLocalPlayer: " << CLocalPlayer->Name.c_str() << std::endl;
		Players = reinterpret_cast<uintptr_t>(CPlayers);
		/*
		CLocalCharacter = CLocalPlayer->Character;
		std::cout << "CLocalCharacter: " << CLocalCharacter->Name << std::endl;
		CLocalHead = CLocalCharacter->FindFirstChild<RBXInstance>("Head");
		std::cout << "CLocalHead: " << CLocalHead->Name << std::endl;
		*/
		GetViewMatrix();
	}

	bool WorldToScreen(vec3 PlayerPosition, vec2& ScreenPosition, float Matrix[4][4])
	{
		unsigned int screenWidth, screenHeight;
		RECT rc;
		HWND newhwnd = FindWindowW(NULL, L"ROBLOX");
		if (newhwnd != NULL) {
			GetWindowRect(newhwnd, &rc);
			screenWidth = rc.right - rc.left;
			screenHeight = rc.bottom - rc.top;
		}
		else {
			ExitProcess(0);
		}
		vec4 clipCoords;
		clipCoords.x = PlayerPosition.x * Matrix[0][0] + PlayerPosition.y * Matrix[0][1] + PlayerPosition.z * Matrix[0][2] + Matrix[0][3];
		clipCoords.y = PlayerPosition.x * Matrix[1][0] + PlayerPosition.y * Matrix[1][1] + PlayerPosition.z * Matrix[1][2] + Matrix[1][3];
		clipCoords.z = PlayerPosition.x * Matrix[2][0] + PlayerPosition.y * Matrix[2][1] + PlayerPosition.z * Matrix[2][2] + Matrix[2][3];
		clipCoords.w = PlayerPosition.x * Matrix[3][0] + PlayerPosition.y * Matrix[3][1] + PlayerPosition.z * Matrix[3][2] + Matrix[3][3];
		if (clipCoords.w < 0.1f)
		{
			return false;
		}

		vec3 NDC;
		NDC.x = clipCoords.x / clipCoords.w;
		NDC.y = clipCoords.y / clipCoords.w;
		NDC.z = clipCoords.z / clipCoords.w;

		ScreenPosition.x = (screenWidth / 2 * NDC.x) + (NDC.x + screenWidth / 2);
		ScreenPosition.y = -(screenHeight / 2 * NDC.y) + (NDC.y + screenHeight / 2);
		return true;
	}

	float distanceto(vec3 from, vec3 to) {
		return sqrt(((from.x - to.x) * (from.x - to.x)) + ((from.y - to.y) * (from.y - to.y)) + ((from.z - to.z) * (from.z - to.z)));
	}
}