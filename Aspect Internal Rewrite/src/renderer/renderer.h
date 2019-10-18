#pragma once
#ifndef RENDERER_H
#define RENDERER_H

#include "imgui/custom/custom.h"

struct input_t
{
	char key_pressed[256] = {};
} inline g_input;

using present_fn = HRESULT(__stdcall*)(IDXGISwapChain* pSwapChain,
    UINT SyncInterval, UINT Flags);

class renderer_t {
public:
    bool w2s(vec3 origin, vec2& screen);
    bool w2s(vec3 origin, vec2& screen, ViewMatrix_t matrix);

public:
    /* D3D11 */
    ID3D11Device* p_device;
    ID3D11DeviceContext* p_context;
    ID3D11Texture2D* p_render_target_texture;
    ID3D11RenderTargetView* p_render_target_view;

    uintptr_t vt;
    present_fn o_present;
    LONG o_wndproc;
    HWND hWnd;

    unsigned int s_w, s_h;

public:
    union {
        bool active = true;
    } gui;
public:
    bool initialized = false;

    void initialize();
    void terminate();
    void render();
	void handle_input(UINT msg, WPARAM w_param, LPARAM l_param);
} inline g_renderer;

#endif // RENDERER_H
