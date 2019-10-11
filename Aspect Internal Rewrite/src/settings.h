#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

struct settings_t
{

	const char* aimbot_types[3] = { "New Aimbot", "Old Aimbot", "3rd Person" };
	const char* aimbot_type = aimbot_types[0];
	const char* keyboard_keys[36] = { "RMB", "LMB", "Mouse 4", "Mouse 5", "Left Shift", "Left Shift", "Left CTRL", "Left ALT", "Right CTRL", "Right ALT", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" };
	const char* aimbot_key = keyboard_keys[0];
	const char* telekill_key = keyboard_keys[11];

	struct esp_t
	{
		bool enabled = true;
		bool names = true;
		bool distance = true;
		bool box = true;
		bool line = true;
		bool health = false;
		bool health_bar = false;
		bool team_check = true;

		int max_distance = 1000;

		struct color_t
		{
			ImVec4 enemy_name = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
			ImVec4 enemy_box = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
			ImVec4 enemy_line = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
			ImVec4 health = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
			ImVec4 health_bar = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
			ImVec4 health_bar_outline = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		} color;
	} esp;


	struct aim_t
	{
		bool enabled = true;
		bool head = true;
		bool team_check = true;
		bool draw_fov = true;
		
		int fov = 100;
		int smooth = 3;

		struct color_t
		{
			ImVec4 fov = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
		} color;
	} aim;

	struct exploits_t
	{
		struct telekill_t
		{
			bool enabled = true;
			int distance = 25;
		} telekill;

		struct telemove_t
		{
			bool enabled = true;
			int amount = 5;
		} telemove;

		struct elevator_t
		{
			bool enabled = true;
			int height = 0;
		} elevator;
	} exploits;
};

#endif // SETTINGS_H