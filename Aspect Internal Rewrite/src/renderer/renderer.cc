#include "../commons.h"
#include "../sdk/sdk.h"
#include "../settings.h"
#include "renderer.h"
#include "../utils/asprintf.h"
#include "../hacks/aimbot/aimbot.h"
#include "../security/security.h"
#include <imgui/custom/custom.h>
#include "../global.h"

extern struct sdk_t g_sdk;
extern struct aimbot_t g_aimbot;
extern struct settings_t g_settings;
extern struct security_t g_security;

HRESULT WINAPI present(IDXGISwapChain* pSwapChain, UINT SyncInterval,
    UINT Flags)
{
    if (!g_renderer.initialized) {
        pSwapChain->GetDevice(__uuidof(ID3D11Device),
            (void**)(&g_renderer.p_device));
        g_renderer.p_device->GetImmediateContext(&g_renderer.p_context);
        ImGui::CreateContext();

        ImGui_ImplWin32_Init(g_renderer.hWnd);
        ImGui_ImplDX11_Init(g_renderer.p_device, g_renderer.p_context);
		ImGui_ImplDX11_CreateDeviceObjects();
		ImGui::GetIO().MouseDrawCursor = g_renderer.gui.active;

		ImGuiStyle* style = &ImGui::GetStyle();
		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = .0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = .0f;
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 10.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = .0f;

		style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(1.00f, 0.54f, 0.01f, 0.71f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Column] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ColumnHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_ColumnActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
		style->Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);	
		style->Colors[ImGuiCol_Tab] = ImVec4(1.00f, 0.54f, 0.01f, 0.71f);
		style->Colors[ImGuiCol_TabHovered] = ImVec4(0.96f, 0.73f, 0.09f, 0.90f);
		style->Colors[ImGuiCol_TabActive] = ImVec4(1.00f, 0.67f, 0.01f, 0.70f);
		style->Colors[ImGuiCol_TabUnfocused] = ImVec4(0.92f, 0.93f, 0.94f, 0.99f);
		style->Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);

        g_renderer.initialized = true;
    }

    if (!g_renderer.p_render_target_view) {
        pSwapChain->GetBuffer(
            0, __uuidof(ID3D11Texture2D),
            reinterpret_cast<void**>(&g_renderer.p_render_target_texture));
        g_renderer.p_device->CreateRenderTargetView(
            g_renderer.p_render_target_texture, nullptr,
            &g_renderer.p_render_target_view);
        g_renderer.p_render_target_texture->Release();

        ImGui::CreateContext();

        ImGui_ImplDX11_Init(g_renderer.p_device, g_renderer.p_context);
        ImGui_ImplDX11_CreateDeviceObjects();
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    g_renderer.render();

    ImGui::Render();
    g_renderer.p_context->OMSetRenderTargets(1, &g_renderer.p_render_target_view,
        nullptr);
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return g_renderer.o_present(pSwapChain, SyncInterval, Flags);
}

LRESULT IMGUI_IMPL_API wndproc(HWND hWnd, UINT msg, WPARAM w_param,
    LPARAM l_param)
{
    ImGui_ImplWin32_WndProcHandler(hWnd, msg, w_param, l_param);

    switch (msg) {
    case WM_SIZE:
        if (g_renderer.p_render_target_view) {
            g_renderer.p_render_target_view->Release();
            g_renderer.p_render_target_view = nullptr;
        }

        RECT rc;
        GetWindowRect(g_renderer.hWnd, &rc);
        g_renderer.s_w = rc.right - rc.left;
        g_renderer.s_h = rc.bottom - rc.top;
        break;
	case WM_LBUTTONDOWN:
		g_input.key_pressed[VK_LBUTTON] = true;
		break;
	case WM_LBUTTONUP:
		g_input.key_pressed[VK_LBUTTON] = false;
		break;
	case WM_RBUTTONDOWN:
		g_input.key_pressed[VK_RBUTTON] = true;
		break;
	case WM_RBUTTONUP:
		g_input.key_pressed[VK_RBUTTON] = false;
		break;
	case WM_MBUTTONDOWN:
		g_input.key_pressed[VK_MBUTTON] = true;
		break;
	case WM_MBUTTONUP:
		g_input.key_pressed[VK_MBUTTON] = false;
		break;
	case WM_XBUTTONDOWN:
	{
		UINT button = GET_XBUTTON_WPARAM(w_param);
		if (button == XBUTTON1)
		{
			g_input.key_pressed[VK_XBUTTON1] = true;
		}
		else if (button == XBUTTON2)
		{
			g_input.key_pressed[VK_XBUTTON2] = true;
		}
		break;
	}
	case WM_XBUTTONUP:
	{
		UINT button = GET_XBUTTON_WPARAM(w_param);
		if (button == XBUTTON1)
		{
			g_input.key_pressed[VK_XBUTTON1] = false;
		}
		else if (button == XBUTTON2)
		{
			g_input.key_pressed[VK_XBUTTON2] = false;
		}
		break;
	}
	case WM_KEYDOWN:
		g_input.key_pressed[w_param] = true;
		break;
	case WM_KEYUP:
		g_input.key_pressed[w_param] = false;
		break;
	default: break;
    }

	g_renderer.handle_input(msg, w_param, l_param);

    return CallWindowProc(reinterpret_cast<WNDPROC>(g_renderer.o_wndproc), hWnd,
        msg, w_param, l_param);
}

void renderer_t::handle_input(UINT msg, WPARAM w_param, LPARAM l_param)
{
	if (msg == WM_KEYUP && w_param == g_settings.menu_key)
		this->gui.active = !this->gui.active,
		ImGui::GetIO().MouseDrawCursor = this->gui.active;
}

void renderer_t::initialize()
{
    this->hWnd = FindWindowW(NULL, L"ROBLOX");
    /* Create SCD */
    DXGI_SWAP_CHAIN_DESC scd = { 0 };
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    scd.OutputWindow = hWnd;
    scd.SampleDesc.Count = 1;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.Windowed = TRUE;
    D3D_FEATURE_LEVEL requestedLevels[] = { D3D_FEATURE_LEVEL_11_0 };

    /* Create Dummy ID3D Objects*/
    IDXGISwapChain* swap = nullptr;
    ID3D11Device* device;
    ID3D11DeviceContext* context;
    HRESULT result = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, requestedLevels, 1,
        D3D11_SDK_VERSION, &scd, &swap, &device, nullptr, &context);

    if (FAILED(result))
        return;

    this->vt = *reinterpret_cast<uintptr_t*>(swap);
    this->o_present = *reinterpret_cast<present_fn*>(this->vt + 32);

    unsigned long o_proc;
    if (!VirtualProtect((LPVOID)this->vt, 64, PAGE_READWRITE, &o_proc))
        return;
    *reinterpret_cast<void**>(this->vt + 32) = &present;
    VirtualProtect((LPVOID)this->vt, 64, o_proc, &o_proc);

    /* Hook WndProc */
    this->o_wndproc = GetWindowLong(hWnd, GWLP_WNDPROC);
    SetWindowLong(hWnd, GWLP_WNDPROC, (LONG)wndproc);

    RECT rc;
    GetWindowRect(this->hWnd, &rc);
    this->s_w = rc.right - rc.left;
    this->s_h = rc.bottom - rc.top;
}

void renderer_t::render()
{
    if (g_renderer.gui.active && !g_security.authenticated) {
        if(ImGui::Begin("login", 0, ImVec2(370, 175), 1.f, ImGuiWindowFlags_NoCollapse));
        {
            static char userbuff[64] = "";
            static char passbuff[64] = "";
			static bool auth = false;

            ImGui::PushID(1);
            ImGui::Text("Username: ");
            ImGui::SameLine();
            ImGui::InputText("", userbuff, 64, ImGuiInputTextFlags_CharsNoBlank);
            ImGui::PopID();
            ImGui::PushID(2);
            ImGui::Text("Password: ");
            ImGui::SameLine();
            ImGui::InputText(
                "", passbuff, 64,
                ImGuiInputTextFlags_Password | ImGuiInputTextFlags_CharsNoBlank);
            ImGui::PopID();
            if (ImGui::Button("Login")) {
                if (!g_security.authenticate(userbuff, passbuff)) {
                    memset(passbuff, 0, 64);
					auth = true;
                }
			}
			if (auth)
			{
				ImGui::SameLine();
				ImGui::Text("Wrong password or username.");
			}


            ImGui::End();
        }
        return;
    }

    if (g_renderer.gui.active) {
        ImGui::Begin(
            "Aspect", 0, ImVec2(460, 410), 1.f,
            ImGuiWindowFlags_NoCollapse | ImGuiConfigFlags_NoMouseCursorChange);
        if (ImGui::BeginTabBar("Aspect_tab_bar")) {
            if (ImGui::BeginTabItem("ESP")) {
				ImGui::Checkbox("- ESP Enabled", &g_settings.esp.enabled);
				ImGui::SliderInt("- ESP Max distance", &g_settings.esp.max_distance, 0, 1000);
                if (ImGui::CollapsingHeader("ESP Names")) {
                    ImGui::Checkbox("- ESP Names Enabled", &g_settings.esp.names);

                    ImGui::Text("- Enemy Name Color");
					ImGui::SameLine();
					ImGui::PushID(1);
                    ImGui::ColorEdit4(
                        "Enemy Name Color", &g_settings.esp.color.enemy_name.x,
						ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					ImGui::PopID();
					ImGui::Text("- Target Name Color");
					ImGui::SameLine();
					ImGui::PushID(2);
					ImGui::ColorEdit4(
						"Target Name Color", &g_settings.esp.color.target_name.x,
						ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					ImGui::PopID();
				}
				if (ImGui::CollapsingHeader("ESP Distance")) {
					ImGui::Checkbox("- ESP Distance Enabled", &g_settings.esp.distance);
					ImGui::Text("- Enemy Distance Color");
					ImGui::SameLine();
					ImGui::PushID(3);
					ImGui::ColorEdit4(
						"Enemy Distance Color", &g_settings.esp.color.enemy_distance.x,
						ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					ImGui::PopID();
					ImGui::Text("- Target Distance Color");
					ImGui::SameLine();
					ImGui::PushID(4);
					ImGui::ColorEdit4(
						"Target Distance Color", &g_settings.esp.color.target_distance.x,
						ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					ImGui::PopID();
				}
                if (ImGui::CollapsingHeader("ESP Boxes")) {
                    ImGui::Checkbox("- ESP Boxes Enabled", &g_settings.esp.box);
                    ImGui::Text("- Enemy Box Color");
					ImGui::SameLine();
					ImGui::PushID(5);
                    ImGui::ColorEdit4(
                        "Enemy Box Color", &g_settings.esp.color.enemy_box.x,
						ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					ImGui::PopID();
					ImGui::Text("- Target Box Color");
					ImGui::SameLine();
					ImGui::PushID(6);
					ImGui::ColorEdit4(
						"Target Box Color", &g_settings.esp.color.target_box.x,
						ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					ImGui::PopID();
				}
                if (ImGui::CollapsingHeader("ESP Tracers")) {
                    ImGui::Checkbox("- ESP Tracer Enabled", &g_settings.esp.line);

                    ImGui::Text("- Enemy Tracer Color");
					ImGui::SameLine();
					ImGui::PushID(7);
                    ImGui::ColorEdit4(
                        "Enemy Tracer Color", &g_settings.esp.color.enemy_line.x,
                        ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					ImGui::PopID();

					ImGui::Text("- Target Tracer Color");
					ImGui::SameLine();
					ImGui::PushID(8);
					ImGui::ColorEdit4(
						"Target Tracer Color", &g_settings.esp.color.target_line.x,
						ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
					ImGui::PopID();
                }
                if (ImGui::CollapsingHeader("ESP Teams")) {
                    ImGui::Checkbox("- ESP Team Check Enabled",
                        &g_settings.esp.team_check);
                }
                ImGui::EndTabItem();
            }
			if (ImGui::BeginTabItem("Aimbot")) {
				ImGui::PushID(9);
				ImGui::Checkbox("Aimbot", &g_settings.aim.enabled);
                ImGui::Checkbox("- Aim at Head", &g_settings.aim.head);
                ImGui::Checkbox("- Team Check", &g_settings.aim.team_check);
                ImGui::Checkbox("- Draw FOV", &g_settings.aim.draw_fov);
                ImGui::SameLine();
                ImGui::ColorEdit4(
                    "FOV Color", &g_settings.aim.color.fov.x,
                    ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
				ImGui::PopID();
				ImGui::Hotkey("- Aim Key", &g_settings.aim.key);
                ImGui::Text("- Smooth ");
                ImGui::SameLine();
                ImGui::PushID(10);
                ImGui::SliderInt("", &g_settings.aim.smooth, 2, 100);
                ImGui::PopID();
				ImGui::Text("- FOV ");
				ImGui::SameLine();
                ImGui::PushID(11);
				ImGui::SliderInt("", &g_settings.aim.fov, 10, 1000);
				ImGui::PopID();
				ImGui::Text("- Max Distance ");
				ImGui::SameLine();
				ImGui::PushID(12);
				ImGui::SliderInt("", &g_settings.aim.distance, 0, 1000);
				ImGui::PopID();

                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Exploits")) {
                ImGui::Checkbox("Telekill", &g_settings.exploits.telekill.enabled);
                ImGui::Text("- Distance");
                ImGui::SameLine();
                ImGui::PushID(13);
                ImGui::SliderInt("", &g_settings.exploits.telekill.distance, 1, 250);
				ImGui::PopID();
				ImGui::PushID(14);
				ImGui::Text("- Elevator Height");
				ImGui::SameLine();
				if (ImGui::SliderInt("", &g_settings.exploits.elevator.height, 0, 50)) {
					[]() {
						auto local_player = g_sdk.players->get_local_player();
						if (INSTANCE_CHECK(local_player))
							return;

						auto local_character = local_player->character;
						if (INSTANCE_CHECK(local_character))
							return;

						auto local_humanoid = local_character->find_child<RBXInstance>("Humanoid");
						if (INSTANCE_CHECK(local_humanoid))
							return;

						uintptr_t LH = reinterpret_cast<uintptr_t>(local_humanoid);
						g_sdk.sethipheight(LH, g_settings.exploits.elevator.height);
					}();
				}
				ImGui::PopID();
				if (ImGui::CollapsingHeader("TeleMover")) {
					ImGui::Checkbox("- Enabled", &g_settings.exploits.telemove.enabled);
					ImGui::Text("- Amount");
					ImGui::SameLine();
					ImGui::PushID(15);
					ImGui::SliderInt("", &g_settings.exploits.telemove.amount, 1, 50);
					ImGui::PopID();

					ImGui::Hotkey("- Forwards ", &g_settings.exploits.telemove.front);
					ImGui::Hotkey("- Left     ", &g_settings.exploits.telemove.left);
					ImGui::Hotkey("- Backwards", &g_settings.exploits.telemove.back);
					ImGui::Hotkey("- Right    ", &g_settings.exploits.telemove.right);

					ImGui::Spacing();

					ImGui::Hotkey("- Up       ", &g_settings.exploits.telemove.up);
					ImGui::Hotkey("- Down     ", &g_settings.exploits.telemove.down);
				}
               
                ImGui::EndTabItem();
            } 
			if (ImGui::BeginTabItem("Misc")) {
				ImGui::Hotkey("- Menu Key", &g_settings.menu_key);
				ImGui::Checkbox("- Rainbow", &g_settings.esp.rainbow);
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
        }
        ImGui::End();
    }

	if (g_settings.esp.rainbow)
	{
		static float rainbow;
		rainbow += 0.005f;
		if (rainbow > 1.f)
			rainbow = 0.f;

		ImGui::GetStyle().Colors[ImGuiCol_Border] = ImColor::HSV(rainbow, 1.f, 1.f);
		g_settings.esp.color.enemy_box = ImColor::HSV(rainbow, 1.f, 1.f);
		g_settings.esp.color.enemy_distance = ImColor::HSV(rainbow, 1.f, 1.f);
		g_settings.esp.color.enemy_line = ImColor::HSV(rainbow, 1.f, 1.f);
		g_settings.esp.color.enemy_name = ImColor::HSV(rainbow, 1.f, 1.f);
		g_settings.aim.color.fov = ImColor::HSV(rainbow, 1.f, 1.f);
	}

    ImDrawList* list = ImGui::GetOverlayDrawList();

    if (g_settings.aim.draw_fov) {
        const ImU32 col32 = ImColor(g_settings.aim.color.fov);
        list->AddCircle(ImVec2(s_w / 2, s_h / 2), g_settings.aim.fov, col32, 32);
	}
	list->AddCircleFilled(ImVec2((s_w / 2) + g_settings.aim.x_off, s_h / 2), 5.f, ImColor(g_settings.aim.color.fov));

    if (g_settings.esp.enabled) {
        for (auto child : *g_sdk.players->children) {
            if (!child)
                continue;
            if (INSTANCE_CHECK(child->character))
                continue;

            /* Local */
            auto local_player = g_sdk.players->get_local_player();
            if (INSTANCE_CHECK(local_player))
                continue;

            /* Don't continue further if we share the same user_id */
            if (child->user_id == local_player->user_id)
                continue;
            /* Don't continue further if teamcheck is on and we are on the same team */
            if (g_settings.esp.team_check && child->team_id == local_player->team_id)
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

            /* Head */
            auto child_head = child->character->find_child<RBXInstance>("Head");
            if (INSTANCE_CHECK(child_head))
                continue;

            auto head_primitive = child_head->get_primitive();
            if (INSTANCE_CHECK(head_primitive))
                continue;

            auto head_body = head_primitive->get_body();
            if (INSTANCE_CHECK(head_body))
                continue;

            /* Leg */
            auto child_leg = child->character->find_child<RBXInstance>("Left Leg");
            if (INSTANCE_CHECK(child_leg))
                child_leg = child->character->find_child<RBXInstance>("LeftLowerLeg");
            if (INSTANCE_CHECK(child_leg))
                continue;

            auto leg_primitive = child_leg->get_primitive();
            if (INSTANCE_CHECK(leg_primitive))
                continue;

            auto leg_body = leg_primitive->get_body();
            if (INSTANCE_CHECK(leg_body))
                continue;

            /* torso */
            auto child_torso = child->character->find_child<RBXInstance>("Torso");
            if (INSTANCE_CHECK(child_torso))
                child_torso = child->character->find_child<RBXInstance>("UpperTorso");
            if (INSTANCE_CHECK(child_torso))
                continue;

            auto torso_primitive = child_torso->get_primitive();
            if (INSTANCE_CHECK(torso_primitive))
                continue;

            auto torso_body = torso_primitive->get_body();
            if (INSTANCE_CHECK(torso_body))
                continue;

            vec3 head_vec = head_body->get_position();
            vec3 torso_vec = torso_body->get_position();
            vec3 leg_vec = leg_body->get_position();

            vec3 local_head_vec = local_head_body->get_position();

            int distance = g_sdk.distance_to<vec3>(local_head_vec, head_vec);

            vec2 screen_leg;
            leg_vec.y -= 3.0f;
            vec2 screen_head;
            head_vec.y += 4.f;
            vec2 screen_torso;

            if (!this->w2s(head_vec, screen_head))
                continue;
            if (!this->w2s(torso_vec, screen_torso))
                continue;
            if (!this->w2s(leg_vec, screen_leg))
                continue;

            if (distance <= g_settings.esp.max_distance) {
                int offset = -45;
                if (g_settings.esp.names) {
                    offset += 15;

					ImColor color = ImColor(child->name == global.target.target_name
						? g_settings.esp.color.target_name : g_settings.esp.color.enemy_name);

                    list->AddText(ImVec2(screen_leg.x - 50, screen_leg.y + offset),
                        ImColor(color),
                        child->name.c_str(), 0);
                }
                if (g_settings.esp.distance) {
                    offset += 15;
                    char* buff;
					asprintf(&buff, "Distance: %d", distance);

					ImColor color = ImColor(child->name == global.target.target_name
						? g_settings.esp.color.target_distance : g_settings.esp.color.enemy_distance);

                    list->AddText(ImVec2(screen_leg.x - 50, screen_leg.y + offset),
                        ImColor(color), buff, 0);
                }
                if (g_settings.esp.box) {
                    RECT pos;
                    pos.left = screen_torso.x - (screen_head.y - screen_torso.y) / 2.1f;
                    pos.top = screen_head.y;
                    pos.bottom = screen_leg.y;
                    pos.right = screen_torso.x + (screen_head.y - screen_torso.y) / 2.1f;

					ImColor color = ImColor(child->name == global.target.target_name 
						? g_settings.esp.color.target_box : g_settings.esp.color.enemy_box);

                    list->AddRect(ImVec2(pos.left, pos.top),
                        ImVec2(pos.right, pos.bottom),
                        ImColor(color));
                }
                if (g_settings.esp.line) {
                    list->AddLine(ImVec2(this->s_h, this->s_w / 2),
                        ImVec2(screen_leg.x, screen_leg.y),
                        ImColor(g_settings.esp.color.enemy_line));
                }
            }
        }
    }
}

void renderer_t::terminate()
{
	*(present_fn*)(this->vt + 32) = this->o_present;
	SetWindowLong(hWnd, GWLP_WNDPROC, this->o_wndproc);
}

bool renderer_t::w2s(vec3 origin, vec2& screen)
{
    return this->w2s(origin, screen, *g_sdk.view_matrix);
}

bool renderer_t::w2s(vec3 origin, vec2& screen, ViewMatrix_t matrix)
{
    vec4 coords;
    coords.data[0] = origin.x * matrix.matrix[0] + origin.y * matrix.matrix[1] + origin.z * matrix.matrix[2] + matrix.matrix[3];
    coords.data[1] = origin.x * matrix.matrix[4] + origin.y * matrix.matrix[5] + origin.z * matrix.matrix[6] + matrix.matrix[7];
    coords.data[2] = origin.x * matrix.matrix[8] + origin.y * matrix.matrix[9] + origin.z * matrix.matrix[10] + matrix.matrix[11];
    coords.data[3] = origin.x * matrix.matrix[12] + origin.y * matrix.matrix[13] + origin.z * matrix.matrix[14] + matrix.matrix[15];
    if (coords.data[3] < 0.1f)
        return false;

    vec3 NDC;
    NDC.x = coords.data[0] / coords.data[3];
    NDC.y = coords.data[1] / coords.data[3];
    NDC.z = coords.data[2] / coords.data[3];

    screen.x = (this->s_w / 2 * NDC.x) + (NDC.x + this->s_w / 2);
    screen.y = -(this->s_h / 2 * NDC.y) + (NDC.y + this->s_h / 2);
    return true;
}
