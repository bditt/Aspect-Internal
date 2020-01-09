#pragma once
#include "vec.h"
#include <sol2/sol.hpp>

class RBXBody {
public:
    /* 0x */
    char pad_0000[0xD4];
    /* 0xD4 */
    float* x;
    /* 0xD8*/
    float* y;
    /* 0xDC */
    float* z;
    /* 0xE0 */
    vec3* velocity;

public:
    vec3 get_position() { return *(this->get_position_ptr()); }

    vec3* get_position_ptr()
    {
        return (vec3*)(reinterpret_cast<uintptr_t>(this) + 0xD4);
    }

    void set_pos_x(float x)
    {
        *(float*)(reinterpret_cast<uintptr_t>(this) + 0xD4) = x;
    }

    void set_pos_y(float y)
    {
        *(float*)(reinterpret_cast<uintptr_t>(this) + 0xD8) = y;
    }

    void set_pos_z(float z)
    {
        *(float*)(reinterpret_cast<uintptr_t>(this) + 0xDC) = z;
    }

    float get_pos_x()
    {
        return *(float*)(reinterpret_cast<uintptr_t>(this) + 0xD4);
    }

    float get_pos_y()
    {
        return *(float*)(reinterpret_cast<uintptr_t>(this) + 0xD8);
    }

    float get_pos_z()
    {
        return *(float*)(reinterpret_cast<uintptr_t>(this) + 0xDC);
    }

    vec3 get_velocity() { return *(this->get_velocity_ptr()); }

    vec3* get_velocity_ptr() { return (vec3*)(this + 0xe0); }
};

class RBXPrimitive {
public:
    /* 0x */
    char pad_0000[0x80];
    /* 0x80 */
    class RBXBody* body;
    /* 0x84 */

public:
    RBXBody* get_body()
    {
        return *(RBXBody**)(reinterpret_cast<uintptr_t>(this) + 0x80);
    }
};

class RBXInstance {
public:
    /* 0x */
    char pad_0000[0x4];
    /* 0x4 */
    class RBXInstance* self; // 0x4
    /* 0x8 */
    char pad_0008[0x4];
    /* 0xc */
    class RBXClassDescriptor* class_descriptor; // 0x4
    /* 0x10 */
    char pad_0010[0x18];
    /* 0x2c */
    char* name; // 0x4
    /* 0x30 */
    std::shared_ptr<std::vector<std::shared_ptr<RBXInstance>>> children; // 0x4
    /* 0x34 */
    char pad_0034[0x64];
    /* 0x98 */
    class RBXPrimitive* primitive; // 0x4
    /* 0x9c */
public:
	template <class T>
	T* find_child(const char* name)
	{
		if (!this->children)
			return 0;
		for (auto child : *this->children)
			if (child->name == name)
				return reinterpret_cast<T*>(child.get());
		return 0;
	}

	template <class T = RBXInstance>
	T* find_class(const char* name)
	{
		if (!this->children)
			return 0;
		for (auto child : *this->children)
			if (child->class_descriptor->class_name == name)
				return reinterpret_cast<T*>(child.get());
		return 0;
	}

	auto get_children()
	{
		return sol::as_table(*this->children);
	}

	RBXInstance* find_child_lua(std::string name)
	{
		if (!this->children)
			return 0;
		for (auto child : *this->children)
			if (child->name == name)
				return reinterpret_cast<RBXInstance*>(child.get());
		return 0;
	}

    RBXPrimitive* get_primitive()
    {
        return *(RBXPrimitive**)(reinterpret_cast<uintptr_t>(this) + 0x98);
    }


	RBXInstance* get_local_player()
	{
		return *(RBXInstance**)(reinterpret_cast<uintptr_t>(this) + 0xC8);
	}
};

class RBXClassDescriptor {
public:
    /* 0x */
    char pad_0000[0x4];
    /* 0x4 */
    char* class_name;
};

class RBXCharacter {
public:
    /* 0x */
    char pad_0000[0x4];
    /* 0x4*/
    class RBXCharacter* self;
    /* 0x8 */
    char pad_0008[0x4];
    /* 0xc */
    class RBXClassDescriptor* class_descriptor;
    /* 0x10 */
    char pad_0010[0x18];
    /* 0x2c*/
    std::string& name;
    /* 0x30 */
    std::shared_ptr<std::vector<std::shared_ptr<RBXInstance>>> children;

public:
	template <class T>
	T* find_child(const char* name)
	{
		if (!this->children)
			return 0;
		for (auto child : *this->children)
			if (child->name == name)
				return reinterpret_cast<T*>(child.get());
		return 0;
	}

	template <class T = RBXInstance>
	T* find_class(const char* name)
	{
		if (!this->children)
			return 0;
		for (auto child : *this->children)
			if (child->class_descriptor->class_name == name)
				return reinterpret_cast<T*>(child.get());
		return 0;
	}
};

class RBXTeam
{
public:
	char pad_0000[4]; //0x0000
	class RBXTeam* self; //0x0004
	char pad_0008[4]; //0x0008
	class RBXClassDescriptor* class_descriptor; //0x000C
	char pad_0010[24]; //0x0010
	std::string& team_name; //0x0028
};

class RBXPlayer {
public:
    /* 0x */
    char pad_0000[0x4];
    /* 0x4 */
    class RBXInstance* self;
    /* 0x8 */
    char pad_0008[0x4];
    /* 0xc */
    class RBXClassDescriptor* class_descriptor;
    /* 0x10 */
    char pad_0010[0x18];
    /* 0x28 */
    std::string& name;
    /* 0x2C */
    char pad_0030[0x8];
    /* 0x34 */
    class RBXPlayers* parent;
    /* 0x38 */
    char pad_003C[0x20];
    /* 0x58 */
    class RBXCharacter* character;
    /* 0x5C */
    char pad_0060[0x2C];
    /* 0x88 */
	class RBXTeam* team; //0x000C
    /* 0x9C*/
    char pad_0090[0x54];
    /* 0xE0 */
    int32_t user_id;
};

class RBXPlayers {
public:
    /* 0x */
    char pad_0000[0xc];
    /* 0xc */
    class RBXClassDescriptor* class_descriptor;
    /* 0x10 */
    char pad_0010[0x18];
    /* 0x2c */
    std::string& name;
    /* 0x30 */
    std::shared_ptr<std::vector<std::shared_ptr<RBXPlayer>>> children;
    /* 0x34 */
    char pad_0034[0x4];
    /* 0x38 */
    class RBXInstance* parent;

public:
    std::string get_name() { return this->name; }

    RBXPlayer* get_local_player()
    {
        return *(RBXPlayer**)(reinterpret_cast<uintptr_t>(this) + 0xC8);
    }

    RBXPlayer* get_player(std::string name)
    {
        for (auto child : *this->children) {
            if (child->name == name) {
                return *(RBXPlayer**)&child;
            }
        }
        return 0;
    }

	template <class T>
	T* find_child(const char* name)
	{
		if (!this->children)
			return 0;
		for (auto child : *this->children)
			if (child->name == name)
				return reinterpret_cast<T*>(child.get());
		return 0;
	}

	template <class T = RBXInstance>
	T* find_class(const char* name)
	{
		if (!this->children)
			return 0;
		for (auto child : *this->children)
			if (child->class_descriptor->class_name == name)
				return reinterpret_cast<T*>(child.get());
		return 0;
	}
};

class RBXHumanoid {
public:
    /* 0x */
    char pad_0000[0x4];
    /* 0x4 */
    class RBXHumanoid* self;
    /* 0x8 */
    char pad_0008[0x4];
    /* 0xc */
    class RBXClassDescriptor* class_descriptor;
    /* 0x10 */
    char pad_0010[0x18];
    /* 0x2c */
    std::string& name;
    /* 0x30 */
    char pad_0030[0x18c];
    /* 0x1bc */
    uintptr_t walkspeed_ptr;
    /* 0x1c0 */
    char pad_01C0[0x8];
    /* 0x1c8 */
    uintptr_t health_ptr;
    /* 0x1cc */
    uintptr_t maxhealth_ptr;
    /* 0x1d0*/
    char pad_01D0[0x4];
    /* 0x1d4*/
    uintptr_t jumppower_ptr;
};