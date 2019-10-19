#pragma once
#ifndef SETTINGS_H
#define SETTINGS_H

struct settings_t {
    struct {
		bool enabled = true;
        bool names = true;
        bool distance = true;
        bool box = true;
        bool line = true;
        bool team_check = true;

        int max_distance = 1000;

		bool rainbow = false;

		struct {
			ImVec4 target_box = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
			ImVec4 target_name = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
			ImVec4 target_distance = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
			ImVec4 target_line = ImVec4(1.00f, 0.54f, 0.01f, 0.71f);

			ImVec4 enemy_distance = ImVec4(1.00f, 0.54f, 0.01f, 0.71f);
            ImVec4 enemy_name = ImVec4(1.00f, 0.54f, 0.01f, 0.71f);
			ImVec4 enemy_box = ImVec4(1.00f, 0.54f, 0.01f, 0.71f);
            ImVec4 enemy_line = ImVec4(1.00f, 0.54f, 0.01f, 0.71f);
        } color;
    } esp;

    struct {
        bool enabled = true;
		int key = 2;

		bool head = true;
        bool team_check = true;
        bool draw_fov = true;

        int fov = 100;
        int smooth = 3;

        struct {
			ImVec4 fov = ImVec4(1.00f, 0.54f, 0.01f, 0.71f);
        } color;
    } aim;

    struct {
        struct {
            bool enabled = true;
            int distance = 25;
        } telekill;

		struct {
			int left = 100;
			int back = 101;
			int right = 102;
			int front = 104;

			int up = 103;
			int down = 105;

            bool enabled = true;
            int amount = 5;
        } telemove;

        struct {
			int increase = 107;
			int decrease = 109;

            bool enabled = true;
            int height = 0;
        } elevator;
    } exploits;

	int menu_key = 45;
};

#endif // SETTINGS_H
