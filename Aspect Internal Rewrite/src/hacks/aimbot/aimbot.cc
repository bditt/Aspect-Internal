
#include "../../commons.h"
#include "../../sdk/sdk.h"
#include "../../renderer/renderer.h"
#include "../../settings.h"
#include "aimbot.h"

extern struct sdk_t g_sdk;
extern struct settings_t g_settings;

void aimbot_t::aim_at(vec2 pos)
{
    vec2 center { g_renderer.s_w / 2, g_renderer.s_h / 2 };
    vec2 target { 0, 0 };
    if (pos.x != 0) {
        if (pos.x > center.x) {
            target.x = -(center.x - pos.x);
            target.x /= 1;
            if (target.x + center.x > center.x * 2)
                target.x = 0;
        }

        if (pos.x < center.x) {
            target.x = pos.x - center.x;
            target.x /= 1;
            if (target.x + center.x < 0)
                target.x = 0;
        }
    }

    if (pos.y != 0) {
        if (pos.y > center.y) {
            target.y = -(center.y - pos.y);
            target.y /= 1;
            if (target.y + center.y > center.y * 2)
                target.y = 0;
        }

        if (pos.y < center.y) {
            target.y = pos.y - center.y;
            target.y /= 1;
            if (target.y + center.y < 0)
                target.y = 0;
        }
    }

    target.x /= g_settings.aim.smooth;
    target.y /= g_settings.aim.smooth;

    if (abs(target.x) < 1) {
        if (target.x > 0) {
            target.x = 1;
        }
        if (target.x < 0) {
            target.x = -1;
        }
    }
    if (abs(target.y) < 1) {
        if (target.y > 0) {
            target.y = 1;
        }
        if (target.y < 0) {
            target.y = -1;
        }
    }

    INPUT input;
    input.type = INPUT_MOUSE;
    input.mi.mouseData = 0;
    input.mi.time = 0;
    input.mi.dx = target.x;
    input.mi.dy = target.y;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    SendInput(1, &input, sizeof(input));
}
