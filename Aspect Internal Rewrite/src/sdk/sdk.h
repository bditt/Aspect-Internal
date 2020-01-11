#pragma once
#ifndef SDK_H
#define SDK_H

#include "../Memory.h"
#include "classes.h"

#define INSTANCE_CHECK(x) (!x || reinterpret_cast<uintptr_t>(x) == 0xF)

typedef __int16(__thiscall* humanoid_sethipheight)(uintptr_t Humanoid,
    float value);
typedef int (__thiscall* humanoid_changestate)(uintptr_t Humanoid, int state);
typedef void*(__cdecl* get_datamodel)(void* dm);


struct SDK {
public:
    RBXInstance* data_model;
    RBXPlayers* players;

    ViewMatrix_t* view_matrix;

public:
    humanoid_sethipheight sethipheight;
	humanoid_changestate changestate;

public:
    inline void initialize()
    {
        sethipheight = (humanoid_sethipheight)Memory::Scan(
            "55 8B EC F3 0F 10 45 ? 56 51 8B F1 F3 0F 11 04 24 E8 ? ? ? ? 83 C4 "
            "04");
		changestate = (humanoid_changestate)Memory::unprotect(
			Memory::Scan("55 8B EC 64 A1 ? ? ? ? 6A FF 68 ? ? ? ? 50 64 89 25 ? ? ? ? 83 EC 08 56 6A 00 "));

        auto get_dm = (void* (__cdecl *)(void*))Memory::Scan(
            "8D 45 D8 C7 45 FC 01 ? ? ? 50 E8 13 E7 ? ? 8B 7D D8", CALL_REL_32, 11);
        unsigned char dm[8];
        data_model = (RBXInstance*)(*(uintptr_t*)get_dm(dm) + 0x44);

        players = data_model->find_class<RBXPlayers>("Players");
		printf("data_model: 0x%p\n", reinterpret_cast<uintptr_t>(data_model));
		printf("players: 0x%p\n", reinterpret_cast<uintptr_t>(players));
		//printf("LocalPlayer: 0x%p\n", reinterpret_cast<uintptr_t>(players->get_local_player()));
        uintptr_t render_view = *(uintptr_t*)(*(uintptr_t*)(reinterpret_cast<uintptr_t>(data_model) + 0x70) + 0x1C);
		uintptr_t visual_engine = *(uintptr_t*)(render_view + 0x8);

        view_matrix = (ViewMatrix_t*)(visual_engine + 0xa0);
        printf("0x%p, 0x%p, 0x%p\n", render_view, visual_engine, view_matrix);
		std::cout << "Setting up Aspect Lua." << std::endl;
    }

    template <class T>
    inline float distance_to(T from, T to)
    {
        return sqrt(((from.x - to.x) * (from.x - to.x)) + ((from.y - to.y) * (from.y - to.y)) + ((from.z - to.z) * (from.z - to.z)));
    }
};

extern SDK sdk;
#endif // SDK_H
