#include "commons.h"
#include "sdk/sdk.h"
#include "settings.h"
#include "renderer/renderer.h"
#include "hacks/aimbot/aimbot.h"
#include "security/security.h"

#include <DbgHelp.h>
#pragma comment(lib, "dbghelp.lib")

time_t starttime;

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

sdk_t g_sdk;
aimbot_t g_aimbot;
settings_t g_settings;
security_t g_security;

unsigned long __stdcall main(LPVOID)
{
	/* Disable crash reports */
	using RtlSetUnhandledExceptionFilterFn = std::decay_t<
		decltype(SetUnhandledExceptionFilter)>;
	reinterpret_cast<RtlSetUnhandledExceptionFilterFn>(
		GetProcAddress(GetModuleHandle("ntdll.dll"),
			"RtlSetUnhandledExceptionFilter"))(ExceptionFilter);

	/* Console */
	DWORD dummy;
	VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &dummy);
	*(BYTE*)(&FreeConsole) = 0xC3;
	Console("Aspect Internal");

	std::cout << g_security.download_url("http://aspectnetwork.net/aspect/internal/changelog.php") << "\n" << std::endl;

	/* Discord */
	InitDiscord();
	starttime = time(0);
	UpdatePresence();

	g_sdk.initialize();
	g_renderer.initialize();

	/* Old aimbot to be replaced */
	std::thread aim([]()
		{
			while (1)
			{
				if (!g_security.authenticated)
					continue;
				if (!g_settings.aim.enabled)
					continue;

				if (!(GetAsyncKeyState(VK_RBUTTON) & 0x8000))
					continue;

				{
					std::string ClosestPlayerName;
					float closest = 1000000.0f;
					float closestx = 1;
					float closesty = 1;
					for (auto Player : *g_sdk.players->children)
					{
						if (Player->user_id != g_sdk.players->get_local_player()->user_id)
						{
							if (g_sdk.players->get_local_player()->team_id != Player->team_id || !g_settings.aim.team_check)
							{
								RBXCharacter* Character = Player->character;
								if (!Character || reinterpret_cast<uintptr_t>(Character) == 0xF || Character == NULL)
								{
									continue;
								}
								RBXInstance* Head = Character->find_child<RBXInstance>("Head");
								if (!Head || reinterpret_cast<uintptr_t>(Head) == 0xF || Head == NULL)
								{
									std::cout << "Failed to find Head!" << std::endl;
									continue;
								}
								vec2 screenPos1;
								vec2 screenPos2;
								vec3x vpos1;
								vpos1 = Head->get_primitive()->get_body()->get_position();
								if (g_settings.aim.head)
								{
									vpos1.y = vpos1.y + 1.5f;
								}
								if (g_renderer.w2s(vpos1, screenPos1))
								{
									float distance = g_aimbot.distance_cross(screenPos1);
									if (distance < closest && distance <= g_settings.aim.fov)
									{
										closest = distance;
										closestx = screenPos1.x;
										closesty = screenPos1.y;
									}
								}
							}
						}
					}
					if (closestx != 1 && closesty != 1)
					{
						g_aimbot.aim_at(vec2{ closestx, closesty });
					}
				}
				Sleep(25);
			}
		});
	/* To be replaced aswell... i just want to get this out asap */
	std::thread exploits([]()
		{
			while (1)
			{
				if (!g_security.authenticated)
					continue;

				/* Telekill */
				[]()
				{
					if (!(GetAsyncKeyState(0x42) & 0x8000))
						return;
					if (!g_settings.exploits.telekill.enabled)
						return;

					for (auto child : *g_sdk.players->children)
					{
						if (!child)
							continue;
						if (INSTANCE_CHECK(child->character))
							continue;

						auto child_head = child->character->find_child<RBXInstance>("Head");
						if (INSTANCE_CHECK(child_head))
							continue;

						auto head_primitive = child_head->get_primitive();
						if (INSTANCE_CHECK(head_primitive))
							continue;

						auto head_body = head_primitive->get_body();
						if (INSTANCE_CHECK(head_body))
							continue;

						/* Local */
						auto local_player = g_sdk.players->get_local_player();
						if (INSTANCE_CHECK(local_player))
							continue;

						if (child->team_id == local_player->team_id)
							continue;

						if (child->user_id == local_player->user_id)
							continue;

						auto local_character = local_player->character;
						if (INSTANCE_CHECK(local_character))
							continue;

						auto local_head = local_character->find_child<RBXInstance>("Head");
						if (INSTANCE_CHECK(local_head))
							continue;

						auto local_head_primitive = local_head->get_primitive();
						if (INSTANCE_CHECK(local_head_primitive))
							continue;

						auto local_head_body = local_head_primitive->get_body();
						if (INSTANCE_CHECK(local_head_body))
							continue;

						vec3x head_loc = head_body->get_position();
						vec3x local_loc = local_head_body->get_position();

						float dist = g_sdk.distance_to(local_loc, head_loc);
						if (dist <= g_settings.exploits.telekill.distance)
						{
							local_head_body->set_pos_x(head_body->get_pos_x());
							local_head_body->set_pos_y(head_body->get_pos_y());
							local_head_body->set_pos_z(head_body->get_pos_z());
						}
					}
				} ();

				/* Telemover */
				[]()
				{
					if (!g_settings.exploits.telemove.enabled)
						return;

					/* Local */
					auto local_player = g_sdk.players->get_local_player();
					if (INSTANCE_CHECK(local_player))
						return;

					auto local_character = local_player->character;
					if (INSTANCE_CHECK(local_character))
						return;

					auto local_head = local_character->find_child<RBXInstance>("Head");
					if (INSTANCE_CHECK(local_head))
						return;

					auto local_head_primitive = local_head->get_primitive();
					if (INSTANCE_CHECK(local_head_primitive))
						return;

					auto local_head_body = local_head_primitive->get_body();
					if (INSTANCE_CHECK(local_head_body))
						return;

					if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000)
					{
						local_head_body->set_pos_x(local_head_body->get_pos_x() + g_settings.exploits.telemove.amount);
					}
					if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000)
					{
						local_head_body->set_pos_z(local_head_body->get_pos_z() - g_settings.exploits.telemove.amount);
					}
					if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000)
					{
						local_head_body->set_pos_x(local_head_body->get_pos_x() - g_settings.exploits.telemove.amount);
					}
					if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
					{
						local_head_body->set_pos_z(local_head_body->get_pos_z() + g_settings.exploits.telemove.amount);
					}
					if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000)
					{
						local_head_body->set_pos_y(local_head_body->get_pos_y() - g_settings.exploits.telemove.amount);
					}
					if (GetAsyncKeyState(VK_NUMPAD9) & 0x8000)
					{
						local_head_body->set_pos_y(local_head_body->get_pos_y() + g_settings.exploits.telemove.amount);
					}
					Sleep(50);
				} ();
			}
		});

	aim.detach();
	exploits.detach();

	return 0;
}


BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
		CreateThread(nullptr, 0, &main, nullptr, 0, nullptr);
	return TRUE;
}