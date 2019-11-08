
#include "../../commons.h"
#include "../../sdk/sdk.h"
#include "../../renderer/renderer.h"
#include "../../misc/config.h"
#include "../../security/security.h"
#include "../../global.h"
#include "aimbot.h"

vec2 smooth(vec2 pos)
{
	vec2 center{ renderer.s_w / 2, renderer.s_h / 2 };
	vec2 target{ 0, 0 };
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

	target.x /= config.aim.m_AimSmooth;
	target.y /= config.aim.m_AimSmooth;

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

	return target;
}

void Aimbot::update()
{
	if (!security.authenticated)
		return;
	if (!config.aim.m_Enabled)
		return;

	if (config.aim.m_AimKey > 0 && Input.key_pressed[config.aim.m_AimKey])
	{
		target_t target;

		for (auto child : *sdk.players->children)
		{
			auto local_player = sdk.players->get_local_player();
			if (child->user_id == local_player->user_id)
				continue;
			if (child->team_id == local_player->team_id && config.aim.m_TeamCheck)
				continue;

			auto local_character = local_player->character;
			if (INSTANCE_CHECK(local_character))
				continue;

			auto local_head = local_character->find_child<RBXInstance>("Head");
			if (INSTANCE_CHECK(local_head))
				continue;

			auto character = child->character;
			if (INSTANCE_CHECK(character))
				continue;

			auto head = character->find_child<RBXInstance>("Head");
			if (INSTANCE_CHECK(head))
				continue;

			vec2 sc;
			vec3 target_pos = head->get_primitive()->get_body()->get_position();
			vec3 local_pos = local_head->get_primitive()->get_body()->get_position();
			if (config.aim.m_Head)
				target_pos.y += 1.5f;

			float pdistance = sdk.distance_to<vec3>(local_pos, target_pos);

			if (renderer.w2s(target_pos, sc))
			{
				float distance = this->distance_cross(sc);
				if (distance < target.dist && distance <= config.aim.m_AimFov 
					&& pdistance <= config.aim.m_MaxDistance
					&& distance > config.aim.m_DeadZone)
				{
					//std::cout << "2" << std::endl;
					target = target_t { character->name, sc, distance };
				}
			}
		}
		if (target.sc.x != 1 && target.sc.y != 1)
		{
			if (config.aim.m_AimMethod == 1)
			{
				this->aim_at(
					smooth(target.sc)),
					global.target = target;
			}
			if (config.aim.m_AimMethod == 2)
			{
				this->aim_at(
					vec2{ target.sc.x, target.sc.y }),
					global.target = target;
			}
		}
	}
	else {
		global.target = target_t();
	}
}

void Aimbot::aim_at(vec2 target)
{
	if (config.aim.m_AimMethod == 1)
	{
		INPUT input;
		input.type = INPUT_MOUSE;
		input.mi.mouseData = 0;
		input.mi.time = 0;
		input.mi.dx = target.x;
		input.mi.dy = target.y;
		input.mi.dwFlags = MOUSEEVENTF_MOVE;
		SendInput(1, &input, sizeof(input));
	} 
	else if (config.aim.m_AimMethod == 2)
	{
		SetCursorPos(target.x, target.y);
		INPUT input[2];
		memset(input, 0, 2 * sizeof(input[0]));
		input[0].type = 1;
		input[0].mi.dx = 1;
		input[0].mi.dy = 1;
		input[0].mi.dwFlags = 0x0001;
		
		input[1].type = 1;
		input[1].mi.dx = -1;
		input[1].mi.dy = -1;
		input[1].mi.dwFlags = 0x0001;
		SendInput(1, input, sizeof(input[0]));
	}
}
