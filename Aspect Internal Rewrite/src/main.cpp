#include "commons.h"
#include "sdk/sdk.h"
#include "renderer/renderer.h"
#include "hacks/aimbot/aimbot.h"
#include "security/security.h"
#include "sdk/vec.h"
#include "misc/config.h"

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
    HANDLE hFile = CreateFile((std::to_string(time(nullptr)) + ".dmp").c_str(),
        GENERIC_WRITE, FILE_SHARE_READ, nullptr,
        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

    MINIDUMP_EXCEPTION_INFORMATION eInfo;
    eInfo.ThreadId = GetCurrentThreadId();
    eInfo.ExceptionPointers = excpInfo;
    eInfo.ClientPointers = FALSE;

    MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile,
        MiniDumpNormal, &eInfo, nullptr, nullptr);
    CloseHandle(hFile);

    MessageBox(nullptr, "Aspect has crashed", "Crash", 0);
    exit(0);
}

void ConsoleHacks()
{
    DWORD nothing;
    VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &nothing);
    *(BYTE*)(&FreeConsole) = 0xC3;
}

void Console(const char* title)
{
    AllocConsole();
    SetConsoleTitleA(title);
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);
    HWND ConsoleHandle = GetConsoleWindow();
    SetWindowPos(
        ConsoleHandle, HWND_NOTOPMOST, 0, 0, 0, 0,
        SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    ShowWindow(ConsoleHandle, SW_NORMAL);
}

SDK sdk;
Aimbot g_aimbot;
Security security;
Renderer renderer;
Config config{ "Aspect" };

unsigned long __stdcall main(LPVOID)
{
    /* Disable crash reports */
    using RtlSetUnhandledExceptionFilterFn = std::decay_t<decltype(SetUnhandledExceptionFilter)>;
    reinterpret_cast<RtlSetUnhandledExceptionFilterFn>(GetProcAddress(
        GetModuleHandle("ntdll.dll"), "RtlSetUnhandledExceptionFilter"))(
        ExceptionFilter);

    /* Console */
    DWORD dummy;
    VirtualProtect((PVOID)&FreeConsole, 1, PAGE_EXECUTE_READWRITE, &dummy);
    *(BYTE*)(&FreeConsole) = 0xC3;
    Console("Aspect Internal");

    std::cout << security.download_url(
        "http://aspectnetwork.net/aspect/internal/changelog.php")
              << "\n"
              << std::endl;

	if (!security.check_version())
		MessageBox(nullptr, 
			"Aspect has been updated, please download latest update", "Update", 0);

    /* Discord */
    InitDiscord();
    starttime = time(0);
    UpdatePresence();
	
	sdk.initialize();
    renderer.initialize();

    std::thread aim([]() {
		while (1) {
			g_aimbot.update();
            Sleep(25);
        }
    });

    /* To be replaced aswell... i just want to get this out asap */
    std::thread exploits([]() {
        while (1) {
            if (!security.authenticated)
                continue;

            /* Telekill */
            []() {
                if (!Input.key_pressed[config.exploits.m_Telekill.m_key])
                    return;
                if (!config.exploits.m_Telekill.m_Enabled)
                    return;
				if (Input.key_pressed[config.exploits.m_Telekill.m_key])
				{
					for (auto child : *sdk.players->children) {
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
						auto local_player = sdk.players->get_local_player();
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

						vec3 head_loc = head_body->get_position();
						vec3 local_loc = local_head_body->get_position();

						float dist = sdk.distance_to(local_loc, head_loc);
						if (dist <= config.exploits.m_Telekill.m_Distance) {
							local_head_body->set_pos_x(head_body->get_pos_x());
							local_head_body->set_pos_y(head_body->get_pos_y());
							local_head_body->set_pos_z(head_body->get_pos_z());
						}
					}
				}
            }();

            /* Telemover */
            []() {
                if (!config.exploits.m_Telemove.m_Enabled)
                    return;

                /* Local */
                auto local_player = sdk.players->get_local_player();
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
				
				auto amount = config.exploits.m_Telemove.m_Amount;
				if (Input.key_pressed[config.exploits.m_Telemove.m_Right])
					local_head_body->set_pos_x(local_head_body->get_pos_x() - amount);
				if (Input.key_pressed[config.exploits.m_Telemove.m_Left])
					local_head_body->set_pos_x(local_head_body->get_pos_x() + amount);

				if (Input.key_pressed[config.exploits.m_Telemove.m_Back])
					local_head_body->set_pos_z(local_head_body->get_pos_z() - amount);
				if (Input.key_pressed[config.exploits.m_Telemove.m_Front])
                    local_head_body->set_pos_z(local_head_body->get_pos_z() + amount);

				if (Input.key_pressed[config.exploits.m_Telemove.m_Down])
                    local_head_body->set_pos_y(local_head_body->get_pos_y() - amount);
				if (Input.key_pressed[config.exploits.m_Telemove.m_Up])
                    local_head_body->set_pos_y(local_head_body->get_pos_y() + amount);
                
                Sleep(50);
            }();
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
