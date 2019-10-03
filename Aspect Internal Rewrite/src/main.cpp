#include <RBX.hpp>
#include "Security.hpp"

time_t starttime;
const char* keyboard_keys[] = { "RMB", "LMB", "Mouse 4", "Mouse 5", "Left Shift", "Left Shift", "Left CTRL", "Left ALT", "Right CTRL", "Right ALT", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" };
const char* AimbotKey = keyboard_keys[0];
const char* TeleKillKey = keyboard_keys[11];

constexpr unsigned int Command(const char* str, int h = 0) {
	return !str[h] ? 5381 : (Command(str, h + 1) * 33) ^ str[h];
}

DWORD GetAimKey(const char* key)
{
	switch (Command(key))
	{
	case Command("RMB"):
		return VK_RBUTTON;
	case Command("LMB"):
		return VK_LBUTTON;
	case Command("Mouse 4"):
		return VK_XBUTTON1;
	case Command("Mouse 5"):
		return VK_XBUTTON2;
	case Command("Left CTRL"):
		return VK_LCONTROL;
	case Command("Left ALT"):
		return VK_LMENU;
	case Command("Left Shift"):
		return VK_LSHIFT;
	case Command("Right Shift"):
		return VK_RSHIFT;
	case Command("Right CTRL"):
		return VK_RCONTROL;
	case Command("Right ALT"):
		return VK_RMENU;
	case Command("A"):
		return 0x41;
	case Command("B"):
		return 0x42;
	case Command("C"):
		return 0x43;
	case Command("D"):
		return 0x44;
	case Command("E"):
		return 0x45;
	case Command("F"):
		return 0x46;
	case Command("G"):
		return 0x47;
	case Command("H"):
		return 0x48;
	case Command("I"):
		return 0x49;
	case Command("J"):
		return 0x4A;
	case Command("K"):
		return 0x4B;
	case Command("L"):
		return 0x4C;
	case Command("M"):
		return 0x4D;
	case Command("N"):
		return 0x4E;
	case Command("O"):
		return 0x4F;
	case Command("P"):
		return 0x50;
	case Command("Q"):
		return 0x51;
	case Command("R"):
		return 0x52;
	case Command("S"):
		return 0x53;
	case Command("T"):
		return 0x54;
	case Command("U"):
		return 0x55;
	case Command("V"):
		return 0x56;
	case Command("W"):
		return 0x57;
	case Command("X"):
		return 0x58;
	case Command("Y"):
		return 0x59;
	case Command("Z"):
		return 0x5A;
	}
}

void InitDiscord()
{
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	Discord_Initialize("612035845532418073", &handlers, 1, "76561198816001423");
}

void UpdatePresence()
{
	char buffer[256];
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	discordPresence.state = "https://discord.me/bdittnew";
	discordPresence.details = "Made by bditt!";
	discordPresence.instance = 1;
	discordPresence.startTimestamp = starttime;
	discordPresence.largeImageKey = "aspect";
	Discord_UpdatePresence(&discordPresence);
}

LONG WINAPI ExceptionFilter(EXCEPTION_POINTERS* excpInfo)
{
	HANDLE hFile = CreateFile((std::to_string(time(nullptr)) + ".dmp").c_str(), GENERIC_WRITE, FILE_SHARE_READ, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	MINIDUMP_EXCEPTION_INFORMATION eInfo;
	eInfo.ThreadId = GetCurrentThreadId();
	eInfo.ExceptionPointers = excpInfo;
	eInfo.ClientPointers = FALSE;

	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &eInfo, nullptr, nullptr);
	CloseHandle(hFile);

	MessageBox(nullptr, "Aspect has crashed", "Crash", 0);
	exit(0);
}

void DisableCrashReports()
{
	using RtlSetUnhandledExceptionFilterFn = std::decay_t<decltype(SetUnhandledExceptionFilter)>;
	reinterpret_cast<RtlSetUnhandledExceptionFilterFn>(GetProcAddress(GetModuleHandle("ntdll.dll"), "RtlSetUnhandledExceptionFilter"))(ExceptionFilter);
}

#pragma warning(disable:4005)
#pragma warning(disable:4458)

#define WRITABLE (PAGE_READWRITE | PAGE_WRITECOPY |PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)

std::wstring s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

HWND hWnd;
ID3D11Device* pDevice;
ID3D11DeviceContext* pContext;
ID3D11Texture2D* pRenderTargetTexture;
ID3D11RenderTargetView* pRenderTargetView;

using PresentFn = HRESULT(__stdcall*)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
PresentFn originalPresent;

bool IsWhitelisted = false;
bool IsGuiOpen = true;
bool IsColorPickerOpen = false;

#define SAFE_RELEASE( x ) if( x ){ x->Release( ); x = 0; }


void TeleKill()
{
	if (GetAsyncKeyState(GetAimKey(TeleKillKey)) & 0x8000)
	{
		try
		{
			for (auto Player : *RBX::CPlayers->Children)
			{
				if (Player->UserId != RBX::CLocalPlayer->UserId)
				{
					std::cout << "Player->UserId != RBX::CLocalPlayer->UserId!" << std::endl;
					if (Player->TeamID != RBX::CLocalPlayer->TeamID)
					{
						auto EnemyHead = Player->Character->FindFirstChild<RBXInstance>("Head");
						std::cout << "EnemyHead!" << std::endl;
						vec3 vpos1 = EnemyHead->GetPrimitive()->GetBody()->GetPosition();
						std::cout << "vpos1!" << std::endl;
						vec3 LocalHeadPosition = RBX::CPlayers->GetLocalPlayer()->Character->FindFirstChild<RBXInstance>("Head")->GetPrimitive()->GetBody()->GetPosition();
						std::cout << "LocalHeadPosition!" << std::endl;
						float fdistance = RBX::distanceto(LocalHeadPosition, vpos1);
						std::cout << "fdistance!" << std::endl;
						if (fdistance <= RBX::TeleKillDistance)
						{
							std::cout << "Teleporting!" << std::endl;
							RBX::CPlayers->GetLocalPlayer()->Character->FindFirstChild<RBXInstance>("Head")->GetPrimitive()->GetBody()->SetXPosition(EnemyHead->GetPrimitive()->GetBody()->GetXPosition());
							RBX::CPlayers->GetLocalPlayer()->Character->FindFirstChild<RBXInstance>("Head")->GetPrimitive()->GetBody()->SetYPosition(EnemyHead->GetPrimitive()->GetBody()->GetYPosition());
							RBX::CPlayers->GetLocalPlayer()->Character->FindFirstChild<RBXInstance>("Head")->GetPrimitive()->GetBody()->SetZPosition(EnemyHead->GetPrimitive()->GetBody()->GetZPosition());
							std::cout << "Telteported!" << std::endl;
						}
					}
				}
			}
		}
		catch (...)
		{
			std::cout << "Error boi!" << std::endl;
		}
	}
}

void TeleMove()
{
	if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000)
	{
		RBXInstance* LocalHead = RBX::CPlayers->GetLocalPlayer()->Character->FindFirstChild<RBXInstance>("Head");
		if (LocalHead != NULL)
		{
			LocalHead->GetPrimitive()->GetBody()->SetXPosition(LocalHead->GetPrimitive()->GetBody()->GetXPosition() + RBX::TeleMoveAmount);
		}
		float currentx = RBX::CLocalHead->GetPrimitive()->GetBody()->GetPosition().x;
		//RBX::CLocalHead->GetPrimitive()->GetBody()->GetPosition().x = currentx + RBX::TeleMoveAmount;
		//RBX::SetXPosition(RBX::LocalHead, RBX::GetXPosition(RBX::LocalHead) + RBX::TeleMoveAmount);
	}
	if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000)
	{
		RBXInstance* LocalHead = RBX::CPlayers->GetLocalPlayer()->Character->FindFirstChild<RBXInstance>("Head");
		if (LocalHead != NULL)
		{
			LocalHead->GetPrimitive()->GetBody()->SetZPosition(LocalHead->GetPrimitive()->GetBody()->GetZPosition() - RBX::TeleMoveAmount);
		}
	}
	if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000)
	{
		RBXInstance* LocalHead = RBX::CPlayers->GetLocalPlayer()->Character->FindFirstChild<RBXInstance>("Head");
		if (LocalHead != NULL)
		{
			LocalHead->GetPrimitive()->GetBody()->SetXPosition(LocalHead->GetPrimitive()->GetBody()->GetXPosition() - RBX::TeleMoveAmount);
		}
	}
	if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
	{
		RBXInstance* LocalHead = RBX::CPlayers->GetLocalPlayer()->Character->FindFirstChild<RBXInstance>("Head");
		if (LocalHead != NULL)
		{
			LocalHead->GetPrimitive()->GetBody()->SetZPosition(LocalHead->GetPrimitive()->GetBody()->GetZPosition() + RBX::TeleMoveAmount);
		}
	}
	if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000)
	{
		RBXInstance* LocalHead = RBX::CPlayers->GetLocalPlayer()->Character->FindFirstChild<RBXInstance>("Head");
		if (LocalHead != NULL)
		{
			LocalHead->GetPrimitive()->GetBody()->SetYPosition(LocalHead->GetPrimitive()->GetBody()->GetYPosition() - RBX::TeleMoveAmount);
		}
	}
	if (GetAsyncKeyState(VK_NUMPAD9) & 0x8000)
	{
		RBXInstance* LocalHead = RBX::CPlayers->GetLocalPlayer()->Character->FindFirstChild<RBXInstance>("Head");
		if (LocalHead != NULL)
		{
			LocalHead->GetPrimitive()->GetBody()->SetYPosition(LocalHead->GetPrimitive()->GetBody()->GetYPosition() + RBX::TeleMoveAmount);
		}
	}
}

std::string getexepath()
{
	char result[MAX_PATH];
	return std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
}

void SaveConfig()
{
	char path[MAX_PATH];
	get_file("Aspect.dll", "", path, MAX_PATH);
	std::cout << "Saving Config to: " << path << std::endl;
	std::ofstream myfile;
	std::string file;
	file += path;
	file += "config.ini";
	myfile.open(file);
	myfile << "ESPEnabled=" << std::to_string(RBX::ESPNames) << "\n";
	myfile << "ESPNames=" << std::to_string(RBX::ESPNames) << "\n";
	myfile << "ESPDistance=" << std::to_string(RBX::ESPDistance) << "\n";
	myfile << "ESPBoxes=" << std::to_string(RBX::ESPBox) << "\n";
	myfile << "ESPLines=" << std::to_string(RBX::ESPLine) << "\n";
	myfile << "ESPTeamCheck=" << std::to_string(RBX::ESPTeamcheck) << "\n";

	myfile << "ESPNamesColorR=" << std::to_string(RBX::ESPNameColor.x) << "\n";
	myfile << "ESPNamesColorG=" << std::to_string(RBX::ESPNameColor.y) << "\n";
	myfile << "ESPNamesColorB=" << std::to_string(RBX::ESPNameColor.z) << "\n";
	myfile << "ESPNamesColorA=" << std::to_string(RBX::ESPNameColor.w) << "\n";

	myfile << "ESPDistanceColorR=" << std::to_string(RBX::ESPDistanceColor.x) << "\n";
	myfile << "ESPDistanceColorG=" << std::to_string(RBX::ESPDistanceColor.y) << "\n";
	myfile << "ESPDistanceColorB=" << std::to_string(RBX::ESPDistanceColor.z) << "\n";
	myfile << "ESPDistanceColorA=" << std::to_string(RBX::ESPDistanceColor.w) << "\n";

	myfile << "ESPBoxesColorR=" << std::to_string(RBX::ESPBoxColor.x) << "\n";
	myfile << "ESPBoxesColorG=" << std::to_string(RBX::ESPBoxColor.y) << "\n";
	myfile << "ESPBoxesColorB=" << std::to_string(RBX::ESPBoxColor.z) << "\n";
	myfile << "ESPBoxesColorA=" << std::to_string(RBX::ESPBoxColor.w) << "\n";

	myfile << "ESPLinesColorR=" << std::to_string(RBX::ESPLineColor.x) << "\n";
	myfile << "ESPLinesColorG=" << std::to_string(RBX::ESPLineColor.y) << "\n";
	myfile << "ESPLinesColorB=" << std::to_string(RBX::ESPLineColor.z) << "\n";
	myfile << "ESPLinesColorA=" << std::to_string(RBX::ESPLineColor.w) << "\n";

	myfile << "AimbotEnabled=" << std::to_string(RBX::AimEnabled) << "\n";
	myfile << "AimbotAimAtHead=" << std::to_string(RBX::AimHead) << "\n";
	myfile << "AimbotTeamCheck=" << std::to_string(RBX::AimTeamcheck) << "\n";
	myfile << "AimbotDrawFOV=" << std::to_string(RBX::AimDrawFOV) << "\n";
	myfile << "AimbotSmooth=" << std::to_string(RBX::AimSmoothAmount) << "\n";
	myfile << "AimbotFOV=" << std::to_string(RBX::AimFOV) << "\n";
	myfile << "AimbotYOffset=" << std::to_string(RBX::AimYOffset) << "\n";
	myfile.close();
	std::cout << "Finished Saving!" << std::endl;
}

void LoadConfig()
{
	char path[MAX_PATH];
	get_file("Aspect.dll", "", path, MAX_PATH);
	std::string file;
	file += path;
	//std::string name = "gui.ini";
	//std::string inifilename = file.c_str() + name;
	//ImGui::GetIO().IniFilename = inifilename.c_str();
	file += "config.ini";
	std::ifstream cFile(file);
	if (!cFile.good())
	{
		SaveConfig();
	}
	if (cFile.is_open())
	{
		std::string line;
		while (getline(cFile, line)) {
			line.erase(std::remove_if(line.begin(), line.end(), isspace),
				line.end());
			if (line[0] == '#' || line.empty())
				continue;
			auto delimiterPos = line.find("=");
			auto name = line.substr(0, delimiterPos);
			auto value = line.substr(delimiterPos + 1);
			if (name == "ESPEnabled")
			{
				if (value == "0")
				{
					RBX::ESPEnabled = false;
				}
				else
				{
					RBX::ESPEnabled = true;
				}
				//std::istringstream(value) >> std::boolalpha >> RBX::ESPEnabled;
			}
			else if (name == "ESPNames")
			{
				if (value == "0")
				{
					RBX::ESPNames = false;
				}
				else
				{
					RBX::ESPNames = true;
				}
				//std::istringstream(value) >> std::boolalpha >> RBX::ESPNames;
			}
			else if (name == "ESPDistance")
			{
				if (value == "0")
				{
					RBX::ESPDistance = false;
				}
				else
				{
					RBX::ESPDistance = true;
				}
				//std::istringstream(value) >> std::boolalpha >> RBX::ESPDistance;
			}
			else if (name == "ESPBoxes")
			{
				if (value == "0")
				{
					RBX::ESPBox = false;
				}
				else
				{
					RBX::ESPBox = true;
				}
				//std::istringstream(value) >> std::boolalpha >> RBX::ESPBox;
			}
			else if (name == "ESPLines")
			{
				if (value == "0")
				{
					RBX::ESPLine = false;
				}
				else
				{
					RBX::ESPLine = true;
				}
				//std::istringstream(value) >> std::boolalpha >> RBX::ESPLine;
			}
			else if (name == "ESPTeamCheck")
			{
				if (value == "0")
				{
					RBX::ESPTeamcheck = false;
				}
				else
				{
					RBX::ESPTeamcheck = true;
				}
				//std::istringstream(value) >> std::boolalpha >> RBX::ESPTeamcheck;
			}
			else if (name == "ESPNamesColorR")
			{
				RBX::ESPNameColor.x = ::atof(value.c_str());
			}
			else if (name == "ESPNamesColorG")
			{
				RBX::ESPNameColor.y = ::atof(value.c_str());
			}
			else if (name == "ESPNamesColorB")
			{
				RBX::ESPNameColor.z = ::atof(value.c_str());
			}
			else if (name == "ESPNamesColorA")
			{
				RBX::ESPNameColor.w = ::atof(value.c_str());
			}
			else if (name == "ESPDistanceColorR")
			{
				RBX::ESPDistanceColor.x = ::atof(value.c_str());
			}
			else if (name == "ESPDistanceColorG")
			{
				RBX::ESPDistanceColor.y = ::atof(value.c_str());
			}
			else if (name == "ESPDistanceColorB")
			{
				RBX::ESPDistanceColor.z = ::atof(value.c_str());
			}
			else if (name == "ESPDistanceColorA")
			{
				RBX::ESPDistanceColor.w = ::atof(value.c_str());
			}
			else if (name == "ESPBoxesColorR")
			{
				RBX::ESPBoxColor.x = ::atof(value.c_str());
			}
			else if (name == "ESPBoxesColorG")
			{
				RBX::ESPBoxColor.y = ::atof(value.c_str());
			}
			else if (name == "ESPBoxesColorB")
			{
				RBX::ESPBoxColor.z = ::atof(value.c_str());
			}
			else if (name == "ESPBoxesColorA")
			{
				RBX::ESPBoxColor.w = ::atof(value.c_str());
			}
			else if (name == "ESPLinesColorR")
			{
				RBX::ESPLineColor.x = ::atof(value.c_str());
			}
			else if (name == "ESPLinesColorG")
			{
				RBX::ESPLineColor.y = ::atof(value.c_str());
			}
			else if (name == "ESPLinesColorB")
			{
				RBX::ESPLineColor.z = ::atof(value.c_str());
			}
			else if (name == "ESPLinesColorA")
			{
				RBX::ESPLineColor.w = ::atof(value.c_str());
			}
			else if (name == "AimbotEnabled")
			{
				if (value == "0")
				{
					RBX::AimEnabled = false;
				}
				else
				{
					RBX::AimEnabled = true;
				}
				//std::istringstream(value) >> std::boolalpha >> RBX::AimEnabled;
			}
			else if (name == "AimbotAimAtHead")
			{
				if (value == "0")
				{
					RBX::AimHead = false;
				}
				else
				{
					RBX::AimHead = true;
				}
				//std::istringstream(value) >> std::boolalpha >> RBX::AimHead;
			}
			else if (name == "AimbotTeamCheck")
			{
				if (value == "0")
				{
					RBX::AimTeamcheck = false;
				}
				else
				{
					RBX::AimTeamcheck = true;
				}
				//std::istringstream(value) >> std::boolalpha >> RBX::AimTeamcheck;
			}
			else if (name == "AimbotDrawFOV")
			{
				if (value == "0")
				{
					RBX::AimDrawFOV = false;
				}
				else
				{
					RBX::AimDrawFOV = true;
				}
				//std::istringstream(value) >> std::boolalpha >> RBX::AimDrawFOV;
			}
			else if (name == "AimbotSmooth")
			{
				RBX::AimSmoothAmount = ::atof(value.c_str());
			}
			else if (name == "AimbotFOV")
			{
				RBX::AimFOV = ::atof(value.c_str());
			}
			else if (name == "AimbotYOffset")
			{
				RBX::AimYOffset = ::atof(value.c_str());
			}
			else
			{
				std::cout << "Unknown setting: " << name << std::endl;
			}
		}

	}
	else
	{
		std::cerr << "Couldn't open config file for reading.\n";
	}
}

void RenderESP(std::shared_ptr<RBXPlayer> Player, ImDrawList* list)
{
	try
	{
		//std::cout << "Found Enemy!" << std::endl;
		if (Player->TeamID != RBX::CPlayers->GetLocalPlayer()->TeamID || !RBX::ESPTeamcheck)
		{
			//std::cout << "Enemy not on team!" << std::endl;
			RBX::CLocalCharacter = RBX::CPlayers->GetLocalPlayer()->Character;//RBX::FindFirstChild(RBX::Workspace, RBX::GetName(RBX::LocalPlayer).c_str());
			if (!RBX::CLocalCharacter || reinterpret_cast<uintptr_t>(RBX::CLocalCharacter) == 0xF || RBX::CLocalCharacter == NULL)
				return;
			//printf("CLocalCharacter: 0x%X\n", reinterpret_cast<uintptr_t>(RBX::CLocalCharacter));
			RBX::CLocalHumanoid = RBX::CLocalCharacter->FindFirstChildOfClass<RBXInstance>("Humanoid");
			//printf("CLocalHumanoid: 0x%X\n", reinterpret_cast<uintptr_t>(RBX::CLocalHumanoid));
			if (!RBX::CLocalHumanoid || reinterpret_cast<uintptr_t>(RBX::CLocalHumanoid) == 0xF || RBX::CLocalHumanoid == NULL)
				return;
			//std::cout << "LocalHumanoid" << std::endl;
			RBX::CLocalHead = RBX::CLocalCharacter->FindFirstChild<RBXInstance>("Head");//RBX::FindFirstChild(RBX::LocalCharacter, "Head");
			if (!RBX::CLocalHead || reinterpret_cast<uintptr_t>(RBX::CLocalHead) == 0xF || RBX::CLocalHead == NULL)
				return;
			//printf("CLocalHead: 0x%X\n", reinterpret_cast<uintptr_t>(RBX::CLocalHead));
			//std::string playername = RBX::GetName(Player).c_str();
			RBXCharacter* Character = Player->Character;
			if (!Character || reinterpret_cast<uintptr_t>(Character) == 0xF || Character == NULL)
				return;
			//std::cout << "Character" << std::endl;
			RBXInstance* EnemyHumanoid = Character->FindFirstChildOfClass<RBXInstance>("Humanoid");
			if (!EnemyHumanoid || reinterpret_cast<uintptr_t>(EnemyHumanoid) == 0xF || EnemyHumanoid == NULL)
				return;
			//std::cout << "EnemyHumanoid" << std::endl;
			RBXInstance* Head = Character->FindFirstChild<RBXInstance>("Head");
			if (!Head || reinterpret_cast<uintptr_t>(Head) == 0xF || Head == NULL)
				return;
			//std::cout << "Head" << std::endl;
			RBXInstance* LeftLeg = Character->FindFirstChild<RBXInstance>("Left Leg");
			if (!LeftLeg || reinterpret_cast<uintptr_t>(LeftLeg) == 0xF || LeftLeg == NULL)
			{
				LeftLeg = Character->FindFirstChild<RBXInstance>("LeftLowerLeg");//RBX::FindFirstChild(Character, "LeftLowerLeg");
			}
			if (!LeftLeg || reinterpret_cast<uintptr_t>(LeftLeg) == 0xF || LeftLeg == NULL)
				return;
			//std::cout << "LeftLeg" << std::endl;
			RBXInstance* Torso = Player->Character->FindFirstChild<RBXInstance>("Torso");
			if (!Torso || reinterpret_cast<uintptr_t>(Torso) == 0xF || Torso == NULL)
			{
				Torso = Character->FindFirstChild<RBXInstance>("UpperTorso");
			}
			if (!Torso || reinterpret_cast<uintptr_t>(Torso) == 0xF || Torso == NULL)
				return;
			//std::cout << "Torso" << std::endl;
			//printf("Torso: 0x%X\n", reinterpret_cast<uintptr_t>(Torso));
			vec2 screenPos1;
			vec2 screenPos2;
			vec2 screenPos3;
			vec2 screenPos4;
			if (Head && Torso && LeftLeg)
			{
				//std::cout << "Getting Positions!" << std::endl;
				RBXPrimitive* CLocalHeadPrim = RBX::CLocalHead->GetPrimitive();
				//printf("CLocalHeadPrim: 0x%X\n", reinterpret_cast<uintptr_t>(CLocalHeadPrim));
				RBXBody* CLocalHeadBody = CLocalHeadPrim->GetBody();
				//std::cout << "CLocalHeadBody!" << std::endl;
				//printf("CLocalHeadBody: 0x%X\n", reinterpret_cast<uintptr_t>(CLocalHeadBody));
				vec3 LocalHeadPosition = CLocalHeadBody->GetPosition();//RBX::GetPartPosition(RBX::LocalHead);
				//std::cout << "LocalHeadPosition!" << std::endl;
				vec3 vpos1 = Head->GetPrimitive()->GetBody()->GetPosition();
				//std::cout << "vpos1!" << std::endl;
				vec3 vpos2 = Torso->GetPrimitive()->GetBody()->GetPosition();//RBX::GetPartPosition(Torso);
				//std::cout << "vpos2!" << std::endl;
				vec3 vpos3 = LeftLeg->GetPrimitive()->GetBody()->GetPosition();//RBX::GetPartPosition(LeftLeg);
				//std::cout << "vpos3!" << std::endl;
				float fdistance = RBX::distanceto(LocalHeadPosition, vpos1);
				int rdistance = roundf(fdistance * 100) / 100;
				//std::cout << "Got Positions!" << std::endl;
				if (rdistance < RBX::ESPDistanceAmount)
				{
					//std::cout << "Head Y: " << vpos1.y << std::endl;
					vpos1.y = vpos1.y + 2.5f;
					vpos3.y = vpos3.y - 3.0f;
					RBX::GetViewMatrix();
					//std::cout << "View Matrix Set!" << std::endl;
					if (RBX::WorldToScreen(vpos1, screenPos1, RBX::ViewMatrix) && RBX::WorldToScreen(vpos2, screenPos2, RBX::ViewMatrix) && RBX::WorldToScreen(vpos3, screenPos3, RBX::ViewMatrix) ) //&& RBX::WorldToScreen(vpos4, screenPos4, RBX::ViewMatrix))
					{
						//std::cout << "W2S Passed!" << std::endl;
						int textoffset = -15;
						if (RBX::ESPNames)
						{
							//std::cout << "Drawing ESPNames\n";// << RBX::GetName(i).c_str() << std::endl;
							textoffset += 15;
							const ImU32 col32 = ImColor(RBX::ESPNameColor);
							list->AddText(ImVec2(screenPos3.x - 50, screenPos3.y + textoffset), col32, Player->Name.c_str(), NULL);
							////std::cout << "Name Drawn!" << std::endl;
							//D3D11Font::DrawString((int)(screenPos3.x - 20), (int)(screenPos3.y + textoffset), RBX::GetName(i).c_str(), 224, 196, 13, 255, FontA);
							//std::cout << "Drew ESPNames\n";
						}
						if (RBX::ESPDistance)
						{
							//std::cout << "Drawing ESPDistance\n";
							textoffset += 15;
							std::stringstream ss;
							ss << "Distance: " << std::to_string(rdistance) << std::endl;
							std::string distance = ss.str();
							const ImU32 col32 = ImColor(RBX::ESPDistanceColor);
							list->AddText(ImVec2(screenPos3.x - 50, screenPos3.y + textoffset), col32, distance.c_str(), NULL);
							//D3D11Font::DrawString(distance.c_str(), screenPos3.x - 20, screenPos3.y + textoffset, 224, 196, 13, 255, pFontVisualsLarge);
							//std::cout << "Drew ESPDistance\n";
						}
						if (RBX::ESPBox)
						{
							//std::cout << "Drawing ESPBox\n";
							RECT rPosition;
							rPosition.left = screenPos2.x - (screenPos1.y - screenPos2.y) / 2.1f;
							rPosition.top = screenPos1.y;
							rPosition.bottom = screenPos3.y;
							rPosition.right = screenPos2.x + (screenPos1.y - screenPos2.y) / 2.1f;
							ImU32 col32 = ImColor(RBX::ESPBoxColor);
							if (RBX::CheckFriend(Player->Name))
							{
								col32 = ImColor(RBX::ESPBoxFriendColor);
							}
							list->AddRect(ImVec2(rPosition.left, rPosition.top), ImVec2(rPosition.right, rPosition.bottom), col32);
							//DrawRectOutlined(rPosition, 22, 163, 43, 255);
							//std::cout << "Drew ESPBox\n";
						}
						if (RBX::ESPLine)
						{
							//std::cout << "Drawing ESPLine\n";
							const ImU32 col32 = ImColor(RBX::ESPLineColor);
							list->AddLine(ImVec2(RBX::s_height, RBX::s_width / 2), ImVec2(screenPos2.x, screenPos2.y), col32);
							//DrawLine(RBX::s_height, RBX::s_width / 2, screenPos2.x, screenPos2.y, 22, 163, 43, 255, 3);
							//DrawLine(screenPos4.x, screenPos4.y, screenPos2.x, screenPos2.y, 22, 163, 43, 255, 3);
							//std::cout << "Drew ESPLine\n";
						}
						if (RBX::ESPHealth)
						{
							//std::cout << "Drawing ESPHealth\n";
							textoffset += 15;
							std::stringstream ss;
							ss << "Health: " << std::to_string(EnemyHumanoid->GetCurrentHealth()) + "/" + std::to_string(EnemyHumanoid->GetMaxHealth()) << std::endl;
							std::string distance = ss.str();
							const ImU32 col32 = ImColor(RBX::ESPHealthColor);
							list->AddText(ImVec2(screenPos3.x - 50, screenPos3.y + textoffset), col32, distance.c_str(), NULL);
							//std::cout << "Drew ESPHealth\n";
						}
						if (RBX::ESPHealthBar)
						{
							//std::cout << "Drawing ESPHealthBar\n";
							textoffset += 15;
							const ImU32 OutLineColor = ImColor(RBX::ESPHealthBarOutlineColor);
							const ImU32 BarColor = ImColor(RBX::ESPHealthBarColor);
							list->AddRectFilled(ImVec2(screenPos3.x - 50, screenPos3.y + textoffset), ImVec2(screenPos3.x + 80, screenPos3.y + textoffset + 10), OutLineColor);
							list->AddRectFilled(ImVec2(screenPos3.x - 49, screenPos3.y + textoffset + 1), ImVec2(screenPos3.x + EnemyHumanoid->GetCurrentHealth() - 20, screenPos3.y + textoffset + 9), BarColor);
							//std::cout << "Drew ESPHealthBar\n";
						}
					}
				}
				return;
			}
		}
	}
	catch (std::exception e)
	{
		std::cout << "ESP Error #2: " << e.what() << std::endl;
	}
}

void AimAtPosOld(float x, float y)
{
	if (GetAsyncKeyState(GetAimKey(AimbotKey)) & 0x8000)
	{
		int lowDistMax = 75;
		int AimSpeed = RBX::AimSmoothAmount;
		bool smooth = true;
		float ScreenCenterX = (RBX::s_width / 2);
		float ScreenCenterY = (RBX::s_height / 2);
		float TargetX = 0;
		float TargetY = 0;
		if (x != 0)
		{
			if (x > ScreenCenterX)
			{
				TargetX = -(ScreenCenterX - x);
				TargetX /= AimSpeed;
				if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
			}

			if (x < ScreenCenterX)
			{
				TargetX = x - ScreenCenterX;
				TargetX /= AimSpeed;
				if (TargetX + ScreenCenterX < 0) TargetX = 0;
			}
		}
		if (y != 0)
		{
			if (y > ScreenCenterY)
			{
				TargetY = -(ScreenCenterY - y);
				TargetY /= AimSpeed;
				if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
			}

			if (y < ScreenCenterY)
			{
				TargetY = y - ScreenCenterY;
				TargetY /= AimSpeed;
				if (TargetY + ScreenCenterY < 0) TargetY = 0;
			}
		}
		if (!smooth)
		{
			mouse_event(0x0001, (UINT)(TargetX), (UINT)(TargetY), NULL, NULL);
			return;
		}
		TargetX /= RBX::AimSmoothAmount;
		TargetY /= RBX::AimSmoothAmount;
		if (abs(TargetX) < 1)
		{
			if (TargetX > 0)
			{
				TargetX = 1;
			}
			if (TargetX < 0)
			{
				TargetX = -1;
			}
		}
		if (abs(TargetY) < 1)
		{
			if (TargetY > 0)
			{
				TargetY = 1;
			}
			if (TargetY < 0)
			{
				TargetY = -1;
			}
		}
		mouse_event(0x0001, (UINT)TargetX, (UINT)TargetY, NULL, NULL);
	}
}

float DistanceBetweenCross(float X, float Y)
{
	RECT rc;
	HWND newhwnd = FindWindowW(NULL, L"ROBLOX");
	if (newhwnd != NULL) {
		GetWindowRect(newhwnd, &rc);
	}
	else {
		ExitProcess(0);
	}
	float ydist = (Y - (rc.bottom - rc.top) / 2);
	float xdist = (X - (rc.right - rc.left) / 2);
	float Hypotenuse = sqrt(pow(ydist, 2) + pow(xdist, 2));
	return Hypotenuse;
}

bool CalculateBullet(vec3& vLocalOrigin, vec3& vAimSpot, vec3& vTargetVelocity, vec3& vLocalVelocity)
{
	float flTDistance = RBX::distanceto(vLocalOrigin, vAimSpot);

	FLOAT flGravityConstant = 196.2f;

	FLOAT flTTI = flTDistance / 50;

	float flDropAdjust = 0.5 * flTTI * flTTI;

	vAimSpot = vAimSpot + vTargetVelocity * flTTI;
	//vAimSpot = vAimSpot - vLocalVelocity * flTTI;
	//vAimSpot.y += flDropAdjust;
	return TRUE;
}

void mouse_move(int x, int y)
{
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.mouseData = 0;
	input.mi.time = 0;
	input.mi.dx = x;
	input.mi.dy = y;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	SendInput(1, &input, sizeof(input));
}

void AimAtPos(float x, float y)
{
	if (GetAsyncKeyState(GetAimKey(AimbotKey)) & 0x8000)
	{
		//By fredaikis
		int lowDistMax = 75;
		int AimSpeed = 1;
		bool smooth = true;
		float ScreenCenterX = (RBX::s_width / 2);
		float ScreenCenterY = (RBX::s_height / 2);
		float TargetX = 0;
		float TargetY = 0;
		if (x != 0)
		{
			if (x > ScreenCenterX)
			{
				TargetX = -(ScreenCenterX - x);
				TargetX /= AimSpeed;
				if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
			}

			if (x < ScreenCenterX)
			{
				TargetX = x - ScreenCenterX;
				TargetX /= AimSpeed;
				if (TargetX + ScreenCenterX < 0) TargetX = 0;
			}
		}
		if (y != 0)
		{
			if (y > ScreenCenterY)
			{
				TargetY = -(ScreenCenterY - y);
				TargetY /= AimSpeed;
				if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
			}

			if (y < ScreenCenterY)
			{
				TargetY = y - ScreenCenterY;
				TargetY /= AimSpeed;
				if (TargetY + ScreenCenterY < 0) TargetY = 0;
			}
		}
		if (!smooth)
		{
			mouse_event(0x0001, (UINT)(TargetX), (UINT)(TargetY), NULL, NULL);
			return;
		}
		TargetX /= RBX::AimSmoothAmount;
		TargetY /= RBX::AimSmoothAmount;
		if (abs(TargetX) < 1)
		{
			if (TargetX > 0)
			{
				TargetX = 1;
			}
			if (TargetX < 0)
			{
				TargetX = -1;
			}
		}
		if (abs(TargetY) < 1)
		{
			if (TargetY > 0)
			{
				TargetY = 1;
			}
			if (TargetY < 0)
			{
				TargetY = -1;
			}
		}
		//mouse_event(MOUSEEVENTF_ABSOLUTE, (UINT)TargetX, (UINT)TargetY, NULL, NULL);
		//mouse_event(0x0001, (UINT)TargetX, (UINT)TargetY, NULL, NULL);
		mouse_move(TargetX, TargetY);
	}
}

void AimAt(float screenx, float screeny)
{
	float aX, aY;
	float ScreenCenterX = (RBX::s_width / 2);
	float ScreenCenterY = (RBX::s_height / 2);

	float enemyX = screenx;
	float enemyY = screeny;

	aX = enemyX - ScreenCenterX;
	aY = enemyY - ScreenCenterY;

	
	if (true)
	{
		aX /= RBX::AimSmoothAmount;
		aY /= RBX::AimSmoothAmount;
	}

	mouse_event(MOUSEEVENTF_MOVE, aX, aY, 0, 0);
}

void Aimbot()
{
	//std::cout << GetAimKey(item_current) << std::endl;
	if (GetAsyncKeyState(GetAimKey(AimbotKey)) & 0x8000 && IsWhitelisted)
	{
		try
		{
			std::string ClosestPlayerName;
			float closest = 1000000.0f;
			float closestx = 1;
			float closesty = 1;
			//std::cout << "GetPlayers" << std::endl;
			for (auto Player : *RBX::CPlayers->Children)
			{
				if (Player->UserId != RBX::CLocalPlayer->UserId)
				{
					//std::cout << "Player != LocalPlayer" << std::endl;
					if (RBX::CLocalPlayer->TeamID != Player->TeamID || !RBX::AimTeamcheck)
					{
						//std::cout << "Player != LocalTeam" << std::endl;
						RBXCharacter* Character = Player->Character;
						if (!Character || reinterpret_cast<uintptr_t>(Character) == 0xF || Character == NULL)
						{
							continue;
						}
						RBXInstance* Head = Character->FindFirstChild<RBXInstance>("Head");
						if (!Head || reinterpret_cast<uintptr_t>(Head) == 0xF || Head == NULL)
						{
							std::cout << "Failed to find Head!" << std::endl;
							continue;
						}
						//std::cout << "Head" << std::endl;
						vec2 screenPos1;
						vec2 screenPos2;
						vec3 vpos1;
						vpos1 = Head->GetPrimitive()->GetBody()->GetPosition();
						//std::cout << "vpos1" << std::endl;
						//vpos2 = RBX::CPlayers->GetLocalPlayer()->Character->FindFirstChild<RBXInstance>("Head")->GetPrimitive()->GetBody()->GetPosition();
						if (RBX::AimHead)
						{
							vpos1.y = vpos1.y + 1.5f + (RBX::AimYOffset);
						}
						vpos1.x = vpos1.x + (RBX::AimXOffset);
						//std::cout << "vpos1.y" << std::endl;
						if (RBX::AimPrediction)
						{
							if (CalculateBullet(RBX::CLocalPlayer->Character->FindFirstChild<RBXInstance>("Head")->GetPrimitive()->GetBody()->GetPosition(), vpos1, RBX::CLocalPlayer->Character->FindFirstChild<RBXInstance>("Head")->GetPrimitive()->GetBody()->GetVelocity(), Head->GetPrimitive()->GetBody()->GetVelocity()))
							{

							}
							if (RBX::WorldToScreen(vpos1, screenPos1, RBX::ViewMatrix))
							{
								//std::cout << "WorldToScreen" << std::endl;
								float distance = DistanceBetweenCross(screenPos1.x, screenPos1.y);
								//std::cout << "distance: " << distance << std::endl;
								if (distance < closest && distance <= RBX::AimFOV)
								{
									//std::cout << "distance < closest" << std::endl;
									closest = distance;
									closestx = screenPos1.x;
									closesty = screenPos1.y;
									ClosestPlayerName = Player->Name;
									//std::cout << "returning" << std::endl;
								}
							}
						}
						else
						{
							//std::cout << "!AimPrediction" << std::endl;
							if (RBX::WorldToScreen(vpos1, screenPos1, RBX::ViewMatrix))
							{
								//std::cout << "WorldToScreen" << std::endl;
								float distance = RBX::distanceto(RBX::CLocalCharacter->FindFirstChild<RBXInstance>("Head")->GetPrimitive()->GetBody()->GetPosition(),vpos1);
								//std::cout << "distance: " << distance << std::endl;
								if (distance < closest && distance <= RBX::AimFOV)
								{
									//std::cout << "distance < closest" << std::endl;
									closest = distance;
									closestx = screenPos1.x;
									closesty = screenPos1.y;
									ClosestPlayerName = Player->Name;
									//std::cout << "returning" << std::endl;
								}
							}
						}
					}
				}
			}
			if (closestx != 1 && closesty != 1)
			{
				//AimAt(closestx, closesty);
				//mouse_event(MOUSEEVENTF_ABSOLUTE, (UINT)closestx, (UINT)closesty, NULL, NULL);
				if (RBX::AimType == "New Aimbot")
				{
					//std::cout << "adwadawdawd" << std::endl;
					if (GetAsyncKeyState(VK_MBUTTON))
					{
						if (RBX::CheckFriend(ClosestPlayerName))
						{
							RBX::RemoveFriend(ClosestPlayerName);
						}
						else
						{
							RBX::AddFriend(ClosestPlayerName);
						}
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}
					if (!RBX::CheckFriend(ClosestPlayerName))
					{
						AimAtPos(closestx, closesty);
					}
				}
				else if (RBX::AimType == "Old Aimbot")
				{
					//std::cout << "adwadawdawd" << std::endl;
					AimAtPosOld(closestx, closesty);
				}
				else if (RBX::AimType == "3rd Person")
				{
					//std::cout << "adwadawdawd" << std::endl;
					SetCursorPos(closestx, closesty);
				}
				else
				{
					std::cout << "Unknown Aim Type!" << std::endl;
				}
				//mouse_event(MOUSEEVENTF_ABSOLUTE, (UINT)closestx, (UINT)closesty, NULL, NULL);
			}
		}
		catch (std::exception e)
		{
			std::cout << "Aimbot Error #2: " << e.what() << std::endl;
		}
	}
}

void Update() {
	while (true)
	{
		if (GetAsyncKeyState(0x2D))
		{
			while (GetAsyncKeyState(0x2D)) {}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(32));
	}
}

void StartAimbot()
{
	while (true)
	{
		try
		{
			if (IsWhitelisted && RBX::AimEnabled)
			{
				Aimbot();
				Sleep(30);
			}
		}
		catch (std::exception e)
		{
			std::cout << "Aimbot Error #1: " << e.what() << std::endl;
		}
		//std::this_thread::sleep_for(std::chrono::microseconds(50));
	}
}

void StartTeleMove()
{
	while (true)
	{
		try
		{
			if (IsWhitelisted && RBX::TeleMove)
			{
				TeleMove();
				Sleep(50);
			}
		}
		catch (std::exception e)
		{
			std::cout << "Aimbot Error #1: " << e.what() << std::endl;
		}
	}
}

void StartTeleKill()
{
	while (true)
	{
		try
		{
			if (IsWhitelisted && RBX::TeleKill)
			{
				TeleKill();
			}
		}
		catch (std::exception e)
		{
			std::cout << "Aimbot Error #1: " << e.what() << std::endl;
		}
	}
}

void StartElevator()
{
	while (true)
	{
		try
		{
			if (IsWhitelisted && RBX::Elevator)
			{
				RBXInstance* CLH = RBX::CPlayers->GetLocalPlayer()->Character->FindFirstChildOfClass<RBXInstance>("Humanoid");
				uintptr_t LH = reinterpret_cast<uintptr_t>(CLH);
				RBX::sethipheight(LH, RBX::ElevatorHeight);
			}
		}
		catch (std::exception e)
		{
			std::cout << "Elevator Error: " << e.what() << std::endl;
		}
	}
}

void StartNoClip()
{
	while (true)
	{
		try
		{
			if (IsWhitelisted && RBX::NoClipEnabled)
			{
				/*
				std::cout << "Changing State!" << std::endl;
				RBXInstance* CLH = RBX::CPlayers->GetLocalPlayer()->Character->FindFirstChildOfClass<RBXInstance>("Humanoid");
				uintptr_t LH = reinterpret_cast<uintptr_t>(CLH);
				*/
				uintptr_t LP = RBX::GetLocalPlayer(RBX::Players);
				if (!LP)
				{
					continue;
				}
				uintptr_t LC = RBX::GetCharacter(LP);
				if (!LC)
				{
					continue;
				}
				uintptr_t LH = RBX::FindFirstChildOfClass(LC, "Humanoid");
				if (LH)
				{
					RBX::ChangeState(LH, 11);
					std::cout << "Changed State!" << std::endl;
				}
				Sleep(50);
			}
		}
		catch (const std::exception& ex) {
			std::cout << "NoClip Error #1: " << ex.what() << std::endl;
		}
		catch (const std::string& ex) {
			std::cout << "NoClip Error #2: " << ex << std::endl;
		}
		catch (...) {
			std::cout << "NoClip Error #3: Unknown!" << std::endl;
		}
	}
}

uintptr_t getSwapChainVT()
{
	//std::cout << "Creating Window" << std::endl;
	dprint("CWindow\n");
	HWND hWnd = CreateWindow("STATIC", "Dummy", 0, 0, 0, 0, 0, nullptr, nullptr, nullptr, nullptr);
	//std::cout << "Setting SCD" << std::endl;
	dprint("SCD\n");
	DXGI_SWAP_CHAIN_DESC scd = { 0 };
	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	dprint("SCDF\n");
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 1;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Windowed = TRUE;
	//std::cout << "Setting RLevels" << std::endl;
	dprint("RLevel\n");
	D3D_FEATURE_LEVEL requestedLevels[] = { D3D_FEATURE_LEVEL_11_0 };

	IDXGISwapChain* pSwapChain = nullptr;
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;
	dprint("CDDSC\n");
	//std::cout << "Creating Device/SwapChain" << std::endl;
	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, requestedLevels, 1, D3D11_SDK_VERSION, &scd, &pSwapChain,
		&pDevice, nullptr, &pContext);
	//std::cout << "Returning SwapChain" << std::endl;
	dprint("RSC\n");
	return *reinterpret_cast<uintptr_t*>(pSwapChain);
}

int my_exception_handler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description) {
	// L is the lua state, which you can wrap in a state_view if necessary
	// maybe_exception will contain exception, if it exists
	// description will either be the what() of the exception or a description saying that we hit the general-case catch(...)
	std::cout << "An exception occurred in a function, here's what it says ";
	if (maybe_exception) {
		std::cout << "(straight from the exception): ";
		const std::exception& ex = *maybe_exception;
		std::cout << ex.what() << std::endl;
	}
	else {
		std::cout << "(from the description parameter): ";
		std::cout.write(description.data(), description.size());
		std::cout << std::endl;
	}

	// you must push 1 element onto the stack to be 
	// transported through as the error object in Lua
	// note that Lua -- and 99.5% of all Lua users and libraries -- expects a string
	// so we push a single string (in our case, the description of the error)
	return sol::stack::push(L, description);
}

HRESULT WINAPI presentHook(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	static std::once_flag initGraphics;
	std::call_once(initGraphics, [&]()
		{
			pSwapChain->GetDevice(__uuidof(ID3D11Device), reinterpret_cast<void**>(&pDevice));
			pDevice->GetImmediateContext(&pContext);

			ImGui::CreateContext();
			ImGui::StyleColorsDark();

			ImGui_ImplWin32_Init(hWnd);
			ImGui_ImplDX11_Init(pDevice, pContext);
			ImGui_ImplDX11_CreateDeviceObjects();
			RBX::lua.open_libraries(sol::lib::base, sol::lib::table, sol::lib::package,
				sol::lib::debug, sol::lib::math, sol::lib::io, sol::lib::os,
				sol::lib::string, sol::lib::coroutine);
			//RBX::lua.set_exception_handler(&my_exception_handler);
			//RBX::lua.new_usertype<RBXPlayer>("RBXPlayer", "UserId", &RBXPlayer::UserId, "GetUserId", &RBXPlayer::GetUserId, "GetName", &RBXPlayer::GetName);
			//RBX::lua.new_usertype<RBXPlayers>("RBXPlayers", "FindFirstChild", &RBXPlayers::FindFirstChild<RBXPlayer>, "GetLocalPlayer", &RBXPlayers::GetLocalPlayer, "GetName", &RBXPlayers::GetName, "GetPlayers", &RBXPlayers::GetPlayers, "GetPlayer", &RBXPlayers::GetPlayer);
			//RBX::lua[sol::create_if_nil]["Aspect"]["Players"] = (RBXPlayers)* RBX::CPlayers;// (< -which is a RBXPlayers instance);
			//std::cout << "LocalPlayer: " << RBX::CPlayers->LocalPlayer->Name << std::endl;
			//RBX::lua["Aspect"]["Players"] = RBX::Players;
			//RBX::lua.set_function("GetLocalPlayer", RBX::GetLocalPlayerLua);
			//RBX::lua["Aspect"]["GetLP"] = RBX::GetLocalPlayer;
			//RBX::lua.set("test", sol::c_call<sol::wrap<decltype(&RBX::GetLocalPlayer));
			//LoadConfig();
		});

	if (!pRenderTargetView)
	{
		pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pRenderTargetTexture));
		pDevice->CreateRenderTargetView(pRenderTargetTexture, nullptr, &pRenderTargetView);
		pRenderTargetTexture->Release();

		ImGui::CreateContext();

		ImGui_ImplDX11_Init(pDevice, pContext);
		ImGui_ImplDX11_CreateDeviceObjects();
	}

	UpdatePresence();
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (!IsWhitelisted)
	{
		if (Aspect::Whitelist::AutoLogin())
		{
			IsWhitelisted = true;
		}
		if (!IsWhitelisted)
		{
			ImGui::Begin("Aspect Login", NULL, ImVec2(200, 100), 1.f, ImGuiWindowFlags_NoCollapse);
			static char buffusername[64] = "";
			static char buffpassword[64] = "";
			ImGui::PushID(1);
			ImGui::Text("Username: ");
			ImGui::SameLine();
			ImGui::InputText("", buffusername, 64, ImGuiInputTextFlags_CharsNoBlank);
			ImGui::PopID();
			ImGui::PushID(2);
			ImGui::Text("Password: ");
			ImGui::SameLine();
			ImGui::InputText("", buffpassword, 64, ImGuiInputTextFlags_Password | ImGuiInputTextFlags_CharsNoBlank);
			ImGui::PopID();
			if (ImGui::Button("Login"))
			{
				if (Aspect::Whitelist::CheckWhitelist(buffusername, buffpassword))
				{
					std::cout << "Logging In!" << std::endl;
					IsWhitelisted = true;
					IsGuiOpen = true;
					char path[MAX_PATH];
					get_file("Aspect.dll", "", path, MAX_PATH);
					std::cout << "Saving Credentials!" << std::endl;
					std::ofstream myfile;
					std::string file;
					file += path;
					file += "credentials.ini";
					myfile.open(file);
					myfile << buffusername << "\n";
					myfile << buffpassword;
				}
				else
				{
					std::cout << "Invalid login!" << std::endl;
				}
			}
			ImGui::End();
		}
	}
	else
	{
		if (IsGuiOpen)
		{
			//RBXPlayers::UpdatePlayersTable();
			//ImGui::GetIO().MouseDrawCursor = true;
			//ImGui::ShowDemoWindow();
			dprint("Loading GUI!\n");
			ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;
			ImVec4* colors = ImGui::GetStyle().Colors;
			colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
			colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
			colors[ImGuiCol_CheckMark] = ImVec4(0.62f, 0.93f, 0.06f, 1.00f);
			ImGui::Begin("Aspect Internal", NULL, ImVec2(100, 100), 1.f, ImGuiWindowFlags_NoCollapse | ImGuiConfigFlags_NoMouseCursorChange);
			ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
			if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
			{
				if (ImGui::BeginTabItem("ESP"))
				{
					ImGui::Checkbox("- ESP Enabled", &RBX::ESPEnabled);
					if (ImGui::CollapsingHeader("ESP Names"))
					{
						ImGui::Checkbox("- ESP Names Enabled", &RBX::ESPNames);
						
						ImGui::Text("- Enemy Name Color");
						ImGui::SameLine();
						ImGui::ColorEdit4("Enemy Name Color", &RBX::ESPNameColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
						
						ImGui::Text("- Team Name Color");
						ImGui::SameLine();
						ImGui::ColorEdit4("Team Name Color", &RBX::ESPFriendNameColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
						
						ImGui::Text("- Friend Name Color");
						ImGui::SameLine();
						ImGui::ColorEdit4("Friend Name Color", &RBX::ESPFriendNameColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					}
					if (ImGui::CollapsingHeader("ESP Distance"))
					{
						ImGui::Checkbox("- ESP Distance Enabled", &RBX::ESPDistance);
						ImGui::Text("- ESP Distance Amount");
						ImGui::SameLine();
						ImGui::SliderInt("", &RBX::ESPDistanceAmount, 1, 1000);

						ImGui::Text("- Enemy Distance Color");
						ImGui::SameLine();
						ImGui::ColorEdit4("Enemy Distance Color", &RBX::ESPDistanceColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
						
						ImGui::Text("- Team Distance Color");
						ImGui::SameLine();
						ImGui::ColorEdit4("Team Distance Color", &RBX::ESPDistanceColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
						
						ImGui::Text("- Friend Distance Color");
						ImGui::SameLine();
						ImGui::ColorEdit4("Friend Distance Color", &RBX::ESPDistanceColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					}
					if (ImGui::CollapsingHeader("ESP Boxes"))
					{
						ImGui::Checkbox("- ESP Boxes Enabled", &RBX::ESPBox);
						
						ImGui::Text("- Enemy Box Color");
						ImGui::SameLine();
						ImGui::ColorEdit4("Enemy Box Color", &RBX::ESPBoxColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
						
						ImGui::Text("- Team Name Color");
						ImGui::SameLine();
						ImGui::ColorEdit4("Team Name Color", &RBX::ESPBoxFriendColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
						
						ImGui::Text("- Friend Box Color");
						ImGui::SameLine();
						ImGui::ColorEdit4("Friend Box Color", &RBX::ESPBoxFriendColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					}
					if (ImGui::CollapsingHeader("ESP Tracers"))
					{
						ImGui::Checkbox("- ESP Tracer Enabled", &RBX::ESPLine);
						
						ImGui::Text("- Enemy Tracer Color");
						ImGui::SameLine();
						ImGui::ColorEdit4("Enemy Tracer Color", &RBX::ESPLineColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
						
						ImGui::Text("- Team Name Color");
						ImGui::SameLine();
						ImGui::ColorEdit4("Team Name Color", &RBX::ESPLineColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
						
						ImGui::Text("- Friend Tracer Color");
						ImGui::SameLine();
						ImGui::ColorEdit4("Friend Tracer Color", &RBX::ESPLineColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					}
					if (ImGui::CollapsingHeader("ESP Health"))
					{
						ImGui::Checkbox("- ESP Health Enabled", &RBX::ESPHealth);
						
						ImGui::Text("- Enemy Health Color");
						ImGui::SameLine();
						ImGui::ColorEdit4("Enemy Health Color", &RBX::ESPHealthColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
						
						ImGui::Text("- Team Name Color");
						ImGui::SameLine();
						ImGui::ColorEdit4("Team Name Color", &RBX::ESPHealthColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
						
						ImGui::Text("- Friend Health Color");
						ImGui::SameLine();
						ImGui::ColorEdit4("Friend Health Color", &RBX::ESPHealthColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					}
					if (ImGui::CollapsingHeader("ESP Health Bar"))
					{
						ImGui::Checkbox("- ESP Health Bar Enabled", &RBX::ESPHealthBar);
						
						ImGui::Text("- Enemy Health Bar Color");
						ImGui::SameLine();
						ImGui::ColorEdit4("Enemy Health Bar Color", &RBX::ESPHealthBarColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
						
						ImGui::Text("- Team Name Color");
						ImGui::SameLine();
						ImGui::ColorEdit4("Team Name Color", &RBX::ESPHealthBarColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);

						ImGui::Text("- Friend Health Bar Color");
						ImGui::SameLine();
						ImGui::ColorEdit4("Friend Health Bar Color", &RBX::ESPHealthBarColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					}
					if (ImGui::CollapsingHeader("ESP Teams"))
					{
						ImGui::Checkbox("- ESP Team Check Enabled", &RBX::ESPTeamcheck);
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Aimbot"))
				{
					ImGui::PushID(3);
					ImGui::Checkbox("Aimbot", &RBX::AimEnabled);
					ImGui::Checkbox("- Aim at Head", &RBX::AimHead);
					ImGui::Checkbox("- Aim Prediction", &RBX::AimPrediction);
					ImGui::Checkbox("- Team Check", &RBX::AimTeamcheck);
					ImGui::Checkbox("- Draw FOV", &RBX::AimDrawFOV);
					ImGui::SameLine();
					ImGui::ColorEdit4("FOV Color", &RBX::AimFOVColor.x, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					ImGui::PopID();
					ImGui::Text("- Smooth ");
					ImGui::SameLine();
					ImGui::PushID(4);
					ImGui::SliderInt("", &RBX::AimSmoothAmount, 2, 100);
					ImGui::PopID();
					ImGui::Text("- Fov ");
					ImGui::SameLine();
					ImGui::PushID(5);
					ImGui::SliderInt("", &RBX::AimFOV, 10, 1000);
					ImGui::PopID();
					ImGui::Text("- Y Offset ");
					ImGui::SameLine();
					ImGui::PushID(6);
					ImGui::SliderFloat("", &RBX::AimYOffset, -5, 5);
					ImGui::PopID();
					ImGui::Text("- Prediction Offset ");
					ImGui::SameLine();
					ImGui::PushID(7);
					ImGui::SliderInt("", &RBX::PredictionOffset, 0, 100);
					ImGui::PopID();
					ImGui::Text("- Aim Type");
					ImGui::SameLine();
					ImGui::PushID(8);
					if (ImGui::BeginCombo("", RBX::AimType)) // The second parameter is the label previewed before opening the combo.
					{
						for (int n = 0; n < IM_ARRAYSIZE(RBX::AimTypes); n++)
						{
							bool is_selected = (RBX::AimType == RBX::AimTypes[n]);
							if (ImGui::Selectable(RBX::AimTypes[n], is_selected))
								RBX::AimType = RBX::AimTypes[n];
							if (is_selected)
								ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
						}
						ImGui::EndCombo();
					}
					ImGui::PopID();
					ImGui::Text("- Aim Key");
					ImGui::SameLine();
					ImGui::PushID(9);
					if (ImGui::BeginCombo("", AimbotKey)) // The second parameter is the label previewed before opening the combo.
					{
						for (int n = 0; n < IM_ARRAYSIZE(keyboard_keys); n++)
						{
							bool is_selected = (AimbotKey == keyboard_keys[n]);
							if (ImGui::Selectable(keyboard_keys[n], is_selected))
								AimbotKey = keyboard_keys[n];
							if (is_selected)
								ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
						}
						ImGui::EndCombo();
					}
					ImGui::PopID();
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Config"))
				{
					if (ImGui::Button("Save Config"))
					{
						SaveConfig();
					}
					if (ImGui::Button("Load Config"))
					{
						LoadConfig();
					}
					ImGui::EndTabItem();
				}
				/*
				if (ImGui::BeginTabItem("Players"))
				{
					ImGui::BeginChild("PList", ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 260), false, 0);
					if (ImGui::TreeNode("Player List"))
					{
						std::vector<uint32_t> Children = RBX::GetChildren(RBX::Players);
						for (auto i : Children)
						{
							if (i != RBX::LocalPlayer)
							{
								if (ImGui::TreeNode(RBX::GetName(i).c_str()))
								{
									ImGui::TreePop();
								}
							}
						}
						ImGui::TreePop();
					}
					ImGui::EndChild();
					ImGui::EndTabItem();
				}
				*/
				if (ImGui::BeginTabItem("Exploits"))
				{
					
					ImGui::Checkbox("Telekill", &RBX::TeleKill);
					ImGui::Text("- Distance");
					ImGui::SameLine();
					ImGui::PushID(10);
					ImGui::SliderInt("", &RBX::TeleKillDistance, 1, 250);
					ImGui::PopID();
					ImGui::Text("- Telekill Key");
					ImGui::SameLine();
					ImGui::PushID(11);
					if (ImGui::BeginCombo("", TeleKillKey)) // The second parameter is the label previewed before opening the combo.
					{
						for (int n = 0; n < IM_ARRAYSIZE(keyboard_keys); n++)
						{
							bool is_selected = (TeleKillKey == keyboard_keys[n]);
							if (ImGui::Selectable(keyboard_keys[n], is_selected))
								TeleKillKey = keyboard_keys[n];
							if (is_selected)
								ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
						}
						ImGui::EndCombo();
					}
					ImGui::PopID();
					ImGui::Checkbox("TeleMover", &RBX::TeleMove);
					ImGui::Text("- Amount");
					ImGui::SameLine();
					ImGui::PushID(12);
					ImGui::SliderInt("", &RBX::TeleMoveAmount, 1, 50);
					ImGui::PopID();
					ImGui::PushID(13);
					ImGui::Text("- Elevator Height");
					ImGui::SameLine();
					if (ImGui::SliderInt("", &RBX::ElevatorHeight, 0, 50))
					{
						RBXInstance* CLH = RBX::CPlayers->GetLocalPlayer()->Character->FindFirstChildOfClass<RBXInstance>("Humanoid");
						if (CLH != NULL)
						{
							uintptr_t LH = reinterpret_cast<uintptr_t>(CLH);
							RBX::sethipheight(LH, RBX::ElevatorHeight);
						}
						else
						{
							std::cout << "CLH = NULL!" << std::endl;
						}
					}
					ImGui::PopID();
					ImGui::PushID(14);
					ImGui::Text("- Local WalkSpeed");
					ImGui::SameLine();
					if (ImGui::SliderInt("", &RBX::LocalWalkspeed, 0, 1000))
					{
						RBXInstance* CLH = RBX::CPlayers->GetLocalPlayer()->Character->FindFirstChildOfClass<RBXInstance>("Humanoid");
						if (CLH != NULL)
						{
							uintptr_t LH = reinterpret_cast<uintptr_t>(CLH);
							RBX::setwalkspeed(LH, RBX::LocalWalkspeed);
						}
					}
					ImGui::PopID();
					/*
					ImGui::PushID(15);
					ImGui::Text("- Local JumpPower");
					ImGui::SameLine();
					if (ImGui::SliderInt("", &RBX::LocalJumpPower, 0, 1000))
					{
						RBXInstance* CLH = RBX::CPlayers->GetLocalPlayer()->Character->FindFirstChildOfClass<RBXInstance>("Humanoid");
						if (CLH != NULL)
						{
							uintptr_t LH = reinterpret_cast<uintptr_t>(CLH);
							RBX::setjumppower(LH, RBX::LocalJumpPower);
						}
					}
					ImGui::PopID();
					ImGui::PushID(16);
					ImGui::Text("- Local Gravity");
					ImGui::SameLine();
					if (ImGui::SliderFloat("", &RBX::LocalGravity, 0, 1000))
					{
						uintptr_t WS = reinterpret_cast<uintptr_t>(RBX::CDataModel->FindFirstChildOfClass<RBXInstance>("Workspace"));
						RBX::setgravity(WS, RBX::LocalGravity);
					}
					ImGui::PopID();
					*/
					ImGui::Checkbox("NoClip", &RBX::NoClipEnabled);
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Misc"))
				{
					if (ImGui::Button("Reload"))
					{
						TerminateThread(RBX::UpdateThread, 0);
						CloseHandle(RBX::UpdateThread);
						TerminateThread(RBX::AimbotThread, 0);
						CloseHandle(RBX::AimbotThread);
						TerminateThread(RBX::TeleMoveThread, 0);
						CloseHandle(RBX::TeleMoveThread);
						TerminateThread(RBX::TeleKillThread , 0);
						CloseHandle(RBX::TeleKillThread);
						RBX::LoadAddresses();
						dprint("Checking Roblox vt!\n");
						uintptr_t vt = getSwapChainVT();
						dprint("vt: 0x%X\n", vt);
						dprint("vt + 32: 0x%X\n", vt + 32);
						dprint("Checking Roblox Present!\n");
						originalPresent = *reinterpret_cast<PresentFn*>(vt + 32);
						DWORD dwOldProtection;
						dprint("Setting Roblox Present!\n");
						VirtualProtect(reinterpret_cast<LPVOID>(vt), 64, PAGE_READWRITE, &dwOldProtection);
						*reinterpret_cast<void**>(vt + 32) = &presentHook;
						VirtualProtect(reinterpret_cast<LPVOID>(vt), 64, dwOldProtection, &dwOldProtection);
						dprint("Aspect Loaded!\n");
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Testing"))
				{
					if (ImGui::Button("Lua Test"))
					{
						char path[MAX_PATH];
						get_file("Aspect.dll", "", path, MAX_PATH);
						//std::cout << "Saving Config to: " << path << std::endl;
						std::ofstream myfile;
						std::string file;
						file += path;
						file += "Test.lua";
						sol::protected_function_result pfr = RBX::lua.safe_script_file(file, &sol::script_pass_on_error);
						std::cout << "PFR: " << pfr.valid() << std::endl;
						sol::error err = pfr;
						std::cout << err.what() << std::endl;
					}
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::End();
		}
		else
		{
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
		}
		ImDrawList* list = ImGui::GetOverlayDrawList();
		dprint("Drawing FOV!\n");
		if (RBX::AimDrawFOV)
		{
			const ImU32 col32 = ImColor(RBX::AimFOVColor);
			list->AddCircle(ImVec2(RBX::s_width / 2, RBX::s_height / 2), RBX::AimFOV, col32, 32);
		}
		dprint("Checking ESP!\n");
		if (RBX::ESPEnabled)
		{
			dprint("ESP Enabled!\n");
			RECT rc;
			HWND rbxhwnd = FindWindowW(NULL, L"ROBLOX");
			if (rbxhwnd != NULL) {
				GetWindowRect(rbxhwnd, &rc);
				RBX::s_width = rc.right - rc.left;
				RBX::s_height = rc.bottom - rc.top;
			}
			else {
				ExitProcess(0);
			}
			//dprint("Getting Players\n");
			//std::vector<uint32_t> Children = RBX::GetChildren(RBX::Players);
			//dprint("Looping Players\n");
			//std::vector<RBXPlayer*> Children = RBX::CPlayers->GetChildren<RBXPlayer*>();
			for (auto Child : *RBX::CPlayers->Children)
			{
				if (Child->UserId != RBX::CLocalPlayer->UserId)
				{
					//std::cout << "Rendering : " << Child->Name << std::endl;
					RenderESP(Child, list);
				}
				//
			}
		}
	}

	ImGui::Render();
	pContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	return originalPresent(pSwapChain, SyncInterval, Flags);
}

LONG originalWndProc;
LRESULT IMGUI_IMPL_API WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (auto result = ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		return result;
	}

	switch (msg)
	{
	case WM_SIZE:
		if (pRenderTargetView)
		{
			pRenderTargetView->Release();
			pRenderTargetView = nullptr;
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_INSERT:
			IsGuiOpen = !IsGuiOpen;
			break;
		}
	}

	return CallWindowProc(reinterpret_cast<WNDPROC>(originalWndProc), hWnd, msg, wParam, lParam);
}

void ConsoleHacks()
{
	DWORD nothing;
	VirtualProtect((PVOID)& FreeConsole, 1, PAGE_EXECUTE_READWRITE, &nothing);
	*(BYTE*)(&FreeConsole) = 0xC3;
}

void Console(const char* title) { //void console haxs
	AllocConsole();
	SetConsoleTitleA(title);
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	HWND ConsoleHandle = GetConsoleWindow(); //gets the console window
	::SetWindowPos(ConsoleHandle, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW); //make program topmost (above roblox)
																														 //topmost function: Look at http://www.cplusplus.com/forum/windows/125528/
	::ShowWindow(ConsoleHandle, SW_NORMAL); //show window::SetParent
}

DWORD WINAPI mainFunction(LPVOID lpArgument)
{
	DisableCrashReports();
	ConsoleHacks();
	std::string ConsoleTitle = "Aspect Internal " + Aspect::Whitelist::version;
	Console(ConsoleTitle.c_str());
	//std::cout << "Checking Roblox Version!" << std::endl;
	dprint("Checking Roblox Version!");
	dprint("Checking Aspect Version!");
	InitDiscord();
	starttime = time(0);
	UpdatePresence();
	if (Aspect::Whitelist::CheckVersion())
	{
		if (true)
		{
			//std::cout << "Checking Robox Window!" << std::endl;
			dprint("Checking Roblox Window!");
			hWnd = FindWindowW(nullptr, L"Roblox");
			unsigned long id = 0;
			HANDLE handle;
			GetWindowThreadProcessId(hWnd, &id);

			RBX::RobloxHandle = OpenProcess(PROCESS_ALL_ACCESS, false, id);
			if (RBX::RobloxHandle == INVALID_HANDLE_VALUE) {
				std::cout << "Failure!\n\n";
				std::cout << "Open Roblox!\n";
				system("PAUSE");
			}
			originalWndProc = GetWindowLong(hWnd, GWLP_WNDPROC);
			SetWindowLong(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG>(WndProc));
			dprint("Checking Roblox Addresses!\n");
			RBX::LoadAddresses();
			RBX::UpdateThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Update, 0, 0, 0);
			RBX::AimbotThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartAimbot, 0, 0, 0);
			RBX::TeleMoveThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartTeleMove, 0, 0, 0);
			RBX::TeleKillThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartTeleKill, 0, 0, 0);
			RBX::NoClipThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartNoClip, 0, 0, 0);
			//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)RBXPlayers::UpdatePlayersTable, 0, 0, 0);
			dprint("Checking Roblox vt!\n");
			uintptr_t vt = getSwapChainVT();
			dprint("vt: 0x%X\n", vt);
			dprint("vt + 32: 0x%X\n", vt + 32);
			dprint("Checking Roblox Present!\n");
			originalPresent = *reinterpret_cast<PresentFn*>(vt + 32);
			DWORD dwOldProtection;
			dprint("Setting Roblox Present!\n");
			VirtualProtect(reinterpret_cast<LPVOID>(vt), 64, PAGE_READWRITE, &dwOldProtection);
			*reinterpret_cast<void**>(vt + 32) = &presentHook;
			VirtualProtect(reinterpret_cast<LPVOID>(vt), 64, dwOldProtection, &dwOldProtection);
			dprint("Aspect Loaded!\n");
			RBX::ConsoleTxtCol("lightblue");
			std::cout << "Changelog:" << std::endl;
			Aspect::Whitelist::PrintChangelog();
			std::cout << "\n" << std::endl;
			RBX::ConsoleTxtCol("lightred");
			//std::cout << "Welcome " << RBX::CLocalPlayer->Name << "!\nThanks for buying Aspect Internal!\nDiscord: https://discord.me/bdittnew \nJoin the discord to know when we update!" << std::endl;
			RBX::ConsoleTxtCol("yellow");
		}
		else
		{
			std::cout << "Roblox has updated! Please join https://discord.me/bdittnew to join the discord and get notified when we update!" << std::endl;
		}
	}
	else
	{
		std::cout << "There is a new Aspect update! Please go to http://aspectnetwork.net/aspect/internal/download to get the latest version!" << std::endl;
	}
	return 0;
}

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
		CreateThread(nullptr, 0, &mainFunction, nullptr, 0, nullptr);
	return TRUE;
}