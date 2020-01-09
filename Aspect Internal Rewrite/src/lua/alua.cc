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

	auto imvec = state.new_usertype<ImVec2>("ImVec2", sol::constructors<ImVec2(), ImVec2(float, float)>());
	imvec["x"] = &ImVec2::x;
	imvec["y"] = &ImVec2::y;

	auto imcolor = state.new_usertype<ImColor>("ImColor", sol::constructors<ImColor(),
		ImColor(float, float, float, float)>());
	imcolor["as_u32"] = &ImColor::as_u32;

	auto drawlist = state.new_usertype<ImDrawList>("drawlist");
	drawlist["circle_filled"] = &ImDrawList::AddCircleFilled;
	drawlist["rect_filled"] = &ImDrawList::AddRectFilled;
	drawlist["circle"] = &ImDrawList::AddCircle;
	drawlist["line"] = &ImDrawList::AddLine;
	drawlist["rect"] = &ImDrawList::AddRect;
	drawlist["text"] = [](ImDrawList& draw, const ImVec2& pos, ImU32 col, const char* text) {
		draw.AddText(pos, col, text, 0);
	};

	auto aspect = state.create_table();
	aspect["game"] = dm;
	aspect["register_callback"] = [](const char* event, const char* unique, sol::function func)
	{
		alua.hookmanager->register_hook(event, unique, func);
		printf("alua > script subscribed to event \"%s\" with unique \"%s\"\n", event, unique);
	};

	state["aspect"] = aspect;
	auto result = state.safe_script("aspect.register_callback(\"render\", \"unique_name2\", function(list) list:text(ImVec2.new(250, 250), ImColor.new(255, 255, 0, 255):as_u32(), \"test\") end)");
	if (!result.valid())
	{
		sol::error e = result;
		printf("lua err: %s", e.what());
	}
}