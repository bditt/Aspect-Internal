
#include "../../commons.h"
#include "../../sdk/sdk.h"
#include "../../renderer/renderer.h"
#include "../../settings.h"
#include "../../security/security.h"
#include "../../global.h"
#include "aimbot.h"

extern struct sdk_t g_sdk;
extern struct settings_t g_settings;
extern struct security_t g_security;

void aimbot_t::update()
{
	if (!g_security.authenticated)
		return;
	if (!g_settings.aim.enabled)
		return;

	if (g_settings.aim.key > 0 && g_input.key_pressed[g_settings.aim.key])
	{
		target_t target;
		for (auto child : *g_sdk.players->children)
		{
			RBXPlayer* localplayer = g_sdk.players->get_local_player();
			if (child->user_id == localplayer->user_id)
				continue;
			if (child->team_id == localplayer->team_id && g_settings.aim.team_check)
				continue;

			RBXCharacter* character = child->character;
			if (INSTANCE_CHECK(character))
				continue;

			RBXInstance* head = character->find_child<RBXInstance>("Head");
			if (INSTANCE_CHECK(head))
				continue;

			vec2 sc;
			vec3 vpos = head->get_primitive()->get_body()->get_position();
			if (g_settings.aim.head)
				vpos.y += 1.5f;

			if (g_renderer.w2s(vpos, sc))
			{
				float distance = this->distance_cross(sc);
				if (distance < target.dist && distance <= g_settings.aim.fov)
					target = target_t { character->name, sc, distance };
			}
		}
		if (target.sc.x != 1 && target.sc.y != 1)
			this->aim_at(target.sc),
			global.target = target;
	}
	else {
		global.target = target_t();
	}
}

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
