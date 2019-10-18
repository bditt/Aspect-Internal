#pragma once

struct target_t {
	std::string target_name;
	vec2 sc = { 1, 1 };
	float dist = 99999.f;
};

class aimbot_t {
private:
	target_t aim_target;
public:
    inline float distance_cross(vec2 vec)
    {
        RECT rc;
        HWND hwnd = FindWindowW(NULL, L"ROBLOX");
        if (!hwnd)
            ExitProcess(0);

        GetWindowRect(hwnd, &rc);

        float ydist = (vec.y - (rc.bottom - rc.top) / 2);
        float xdist = (vec.x - (rc.right - rc.left) / 2);
        float ret = sqrt(pow(ydist, 2) + pow(xdist, 2));
        return ret;
    }
    void aim_at(vec2 pos);

    void update();
};
