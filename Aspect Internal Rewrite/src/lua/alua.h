#pragma once

#include "hook_manager.h"
extern class RBXInstance;

class alua_t
{
public:
	void initialize(RBXInstance* dm);
	
	hookmanager_t* hookmanager = new hookmanager_t();
	sol::state state;
};

extern alua_t alua;