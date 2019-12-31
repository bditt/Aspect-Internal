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
private:
    get_datamodel get_dm;

public:
    RBXDataModel* data_model;
    RBXPlayers* players;

    ViewMatrix_t* view_matrix;

public:
    humanoid_sethipheight sethipheight;
	humanoid_changestate changestate;
	sol::state alua;

public:
    inline void initialize()
    {
        sethipheight = (humanoid_sethipheight)Memory::Scan(
            "55 8B EC F3 0F 10 45 ? 56 51 8B F1 F3 0F 11 04 24 E8 ? ? ? ? 83 C4 "
            "04");
        get_dm = (get_datamodel)Memory::Scan(
            "8D 45 E0 C7 45 ? ? ? ? ? 50 E8 ? ? ? ? 83 7D E0 00", CALL_REL_32, 11);
		changestate = (humanoid_changestate)Memory::unprotect(Memory::Scan("55 8B EC 64 A1 ? ? ? ? 6A FF 68 ? ? ? ? 50 64 89 25 ? ? ? ? 83 EC 08 56 6A 00 "));
        unsigned char dm[8];
        data_model = (RBXDataModel*)(*(uintptr_t*)get_dm(dm) + 0x44);
        printf("data_model: 0x%p\n		->name: ", data_model);
        std::cout << data_model->name << std::endl;
		//Sleep(1000000);
        players = data_model->find_child_class<RBXPlayers>("Players");
        printf("players: 0x%p\n		->name: ", players);
        std::cout << players->name << std::endl;
		printf("LocalPlayer: 0x%p\n		->name: ", players->get_local_player());
		std::cout << players->get_local_player()->name << std::endl;
		std::cout << "LocalPlayer ID: " << players->get_local_player()->user_id << std::endl;
		//std::cout << Head->name << std::endl;
		//std::cout << "LocalPlayer ID: " << players->get_local_player()->user_id << std::endl;
		//Sleep(1000000);
        uint32_t render_view = *(uint32_t*)(*(uint32_t*)(reinterpret_cast<uintptr_t>(data_model) + 0x70) + 0x1C);
        uint32_t visual_engine = *(uint32_t*)(render_view + 0x8);

        view_matrix = (ViewMatrix_t*)(visual_engine + 0xa0);
        printf("0x%p, 0x%p, 0x%p\n", render_view, visual_engine, view_matrix);
		std::cout << "Setting up Aspect Lua." << std::endl;
		alua.open_libraries(sol::lib::base);

		auto class_descriptor = alua.new_usertype<RBXClassDescriptor>("RBXClassDescriptor");
		class_descriptor["class_name"] = sol::readonly_property(&RBXClassDescriptor::class_name);

		auto instance = alua.new_usertype<RBXInstance>("RBXInstance");
		instance["self"] = &RBXInstance::self;
		instance["class_descriptor"] = &RBXInstance::class_descriptor;
		instance["name"] = &RBXInstance::name;
		instance["children"] = &RBXInstance::children;
		instance["get_children"] = &RBXInstance::get_children;
		instance["find_child"] = &RBXInstance::find_child_lua;
		//instance["find_child_class"] = sol::readonly_property(&RBXInstance::find_child_class<RBXInstance*>);
		
		auto datamodel = alua.new_usertype<RBXDataModel>("RBXDataModel");
		datamodel["self"] = &RBXDataModel::self;
		datamodel["class_descriptor"] = &RBXDataModel::class_descriptor;
		datamodel["name"] = &RBXDataModel::name;
		datamodel["children"] = &RBXDataModel::children;
		datamodel["get_children"] = &RBXDataModel::get_children;
		datamodel["find_child"] = &RBXDataModel::find_child_lua;
		//datamodel["find_child_class"] = sol::readonly_property(&RBXDataModel::find_child_class<RBXInstance*>);

		auto container = alua.new_usertype<alua_container>("aspect");
		container["game"] = sol::readonly_property(&alua_container::dm);

		alua_container con;
		con.dm = data_model;
		alua["aspect"] = con;

		alua.safe_script("print(\"from lua > \".. aspect.game.name)");
    }

    template <class T>
    inline float distance_to(T from, T to)
    {
        return sqrt(((from.x - to.x) * (from.x - to.x)) + ((from.y - to.y) * (from.y - to.y)) + ((from.z - to.z) * (from.z - to.z)));
    }
};

extern SDK sdk;
#endif // SDK_H
