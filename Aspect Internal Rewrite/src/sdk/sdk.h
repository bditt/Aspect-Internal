#pragma once
#ifndef SDK_H
#define SDK_H

#include "classes.h"
#include "../Memory.h"

#define INSTANCE_CHECK(x) (!x || reinterpret_cast<uintptr_t>(x) == 0xF)

typedef __int16(__thiscall* humanoid_sethipheight)(uintptr_t Humanoid, float value);
typedef void* (__cdecl* get_datamodel)(void* dm);

struct sdk_t
{
private:
	get_datamodel get_dm;
public:
	RBXDataModel* data_model;
	RBXPlayers* players;
	RBXService* workspace;
	RBXService* script_context;

	ViewMatrix_t* view_matrix;
public:
	humanoid_sethipheight sethipheight;
public:
	inline void initialize()
	{
		sethipheight = (humanoid_sethipheight)Memory::Scan(
			"55 8B EC F3 0F 10 45 ? 56 51 8B F1 F3 0F 11 04 24 E8 ? ? ? ? 83 C4 04");
		get_dm = (get_datamodel)Memory::Scan(
			"8D 45 E0 C7 45 ? ? ? ? ? 50 E8 ? ? ? ? 83 7D E0 00", CALL_REL_32, 11);
		unsigned char dm[8];
		data_model = (RBXDataModel*)(*(uintptr_t*)get_dm(dm) + 0x44);
		printf("data_model: 0x%p\n		->name: ", data_model);
		std::cout << data_model->name << std::endl;

		players = data_model->find_child_class<RBXPlayers>("Players");
		printf("players: 0x%p\n		->name: ", players);
		std::cout << players->name << std::endl;


		script_context = data_model->find_child_class<RBXService>("ScriptContext");
		printf("script_context: 0x%p\n		->name: ", script_context);
		std::cout << script_context->name << std::endl;

		RBXService* replicated = data_model->find_child_class<RBXService>("ReplicatedStorage");
		printf("replicated: 0x%p\n", replicated);

		RBXService* uis = data_model->find_child_class<RBXService>("UserInputService");
		printf("uis: 0x%p\n", uis);

		uint32_t render_view = *(uint32_t*)(*(uint32_t*)(reinterpret_cast<uintptr_t>(data_model) + 0x74) + 0x1C);
		uint32_t visual_engine = *(uint32_t*)(render_view + 0x8);

		view_matrix = (ViewMatrix_t*)(visual_engine + 0xa0);
		printf("0x%p, 0x%p, 0x%p\n", render_view, visual_engine, view_matrix);
	}

	template <class T>
	inline float distance_to(T from, T to)
	{
		return sqrt(((from.x - to.x) * (from.x - to.x)) + ((from.y - to.y) * (from.y - to.y)) + ((from.z - to.z) * (from.z - to.z)));
	}
};
#endif // SDK_H