#include "alua.h"
#include "../commons.h"
#include "../sdk/classes.h"

void alua_t::initialize(RBXInstance* dm)
{
	state = sol::state();
	state.open_libraries(sol::lib::base);

	auto class_descriptor = state.new_usertype<RBXClassDescriptor>("RBXClassDescriptor");
	class_descriptor["class_name"] = sol::readonly_property(&RBXClassDescriptor::class_name);

	auto instance = state.new_usertype<RBXInstance>("RBXInstance");
	instance["self"] = &RBXInstance::self;
	instance["class_descriptor"] = &RBXInstance::class_descriptor;
	instance["name"] = &RBXInstance::name;
	instance["children"] = &RBXInstance::children;
	instance["get_children"] = &RBXInstance::get_children;
	instance["find_child"] = &RBXInstance::find_child_lua;
	//instance["find_child_class"] = sol::readonly_property(&RBXInstance::find_child_class<RBXInstance*>);

	auto container = state.new_usertype<alua_container>("aspect");
	container["game"] = sol::readonly_property(&alua_container::dm);

	auto imvec = state.new_usertype<ImVec2>("ImVec2", sol::constructors<ImVec2(), ImVec2(float, float)>());
	imvec["x"] = &ImVec2::x;
	imvec["y"] = &ImVec2::y;

	auto imcolor = state.new_usertype<ImColor>("ImColor", sol::constructors<ImColor(),
		ImColor(float, float, float, float)>());

	auto drawlist = state.new_usertype<ImDrawList>("drawlist");
	drawlist["circle"] = &ImDrawList::AddCircle;

	auto aspect = state.create_table();
	aspect["game"] = dm;
	aspect["register_callback"] = [](const char* event, const char* unique, sol::function func)
	{
		alua.hookmanager->register_hook(event, unique, func);
		printf("alua > script subscribed to event \"%s\" with unique \"%s\"\n", event, unique);
	};

	state["aspect"] = aspect;

	state.safe_script("print(\"from lua > \".. aspect.game.name)");
	state.safe_script("aspect.register_callback(\"render\", \"unique_name\", function(list) print(list.circle) list:circle(ImVec2.new(250,250), 500, ImColor.new(255, 0, 0, 255)) end)");
}