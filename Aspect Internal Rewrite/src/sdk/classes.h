#pragma once
#include "vec.h"

class RBXBody
{
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
	vec3x get_position()
	{ 
		return *(this->get_position_ptr()); 
	}

	vec3x* get_position_ptr()
	{ 
		return (vec3x*)(reinterpret_cast<uintptr_t>(this) + 0xd4);
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

	vec3 get_velocity()
	{
		return *(this->get_velocity_ptr());
	}

	vec3* get_velocity_ptr()
	{
		return (vec3*)(this + 0xe0);
	}
};

class RBXPrimitive
{
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

class RBXInstance
{
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
	char pad_0010[0x1c];
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
	T* find_child(std::string name)
	{
		for (auto child : *this->children)
		{
			if (child->name == name)
			{
				return *reinterpret_cast<T**>(&child);
			}
		}
		return 0;
	}

	template <class T>
	T* find_child_class(std::string name)
	{
		for (auto child : *this->children)
		{
			if (child->class_descriptor->class_name == name)
			{
				return *reinterpret_cast<T**>(&child);
			}
		}
		return 0;
	}

	RBXPrimitive* get_primitive()
	{
		return *(RBXPrimitive * *)(reinterpret_cast<uintptr_t>(this) + 0x98);
	}

	float get_health()
	{
		uint32_t Humanoid = (uint32_t)this;
		uint32_t PointerBase = *(uint32_t*)(Humanoid + 0x1C8);
		uint32_t Pointer2 = *(uint32_t*)(PointerBase);
		uint32_t health_dec = PointerBase ^ Pointer2;
		return *(float*)&health_dec;
	}

	float get_max_health()
	{
		uint32_t Humanoid = (uint32_t)this;
		uint32_t PointerBase = *(uint32_t*)(Humanoid + 0x1CC);
		uint32_t Pointer2 = *(uint32_t*)(PointerBase);
		uint32_t maxhealth_dec = PointerBase ^ Pointer2;
		return *(float*)&maxhealth_dec;
	}
}; 

class RBXClassDescriptor
{
public:
	/* 0x */
	char pad_0000[0x4];
	/* 0x4 */
	std::string& class_name;
};

class RBXServicesPtr
{
public:
	/* 0x */
	int32_t ServicesStart; 
	/* 0x4 */
	int32_t ServicesEnd;
};

class RBXService
{
public:
	/* 0x */
	char pad_0000[0x4];
	/* 0x4 */
	class RBXService* self;
	/* 0x8 */
	char pad_0008[0x4];
	/* 0xc */
	class RBXClassDescriptor* class_descriptor;
	/* 0x10 */
	char pad_0010[0x1c];
	/* 0x2c */
	char* name;
	/* 0x30 */
	std::shared_ptr<std::vector<std::shared_ptr<RBXService>>> children;
	/* 0x34 */
	char pad_0034[0x4];
	/* 0x38 */
	class RBXDataModel* Parent;

public:
	template <class T>
	T* find_child(std::string name)
	{
		for (auto child : *this->children)
		{
			if (child->name == name)
			{
				return *reinterpret_cast<T**>(&child);
			}
		}
		return 0;
	}

	template <class T>
	T* find_child_class(std::string name)
	{
		for (auto child : *this->children)
		{
			if (child->class_descriptor->class_name == name)
			{
				return *reinterpret_cast<T**>(&child);
			}
		}
		return 0;
	}
};

class RBXDataModel
{
public:
	/* 0x */
	char pad_0000[0xc];
	/* 0xc */
	class RBXClassDescriptor* class_descriptor;
	/* 0x10 */
	char pad_0010[0x1c];
	/* 0x2c */
	std::string& name;
	/* 0x30 */
	std::shared_ptr<std::vector<std::shared_ptr<RBXService>>> children;
	/* 0x34 */
public:
	template <class T>
	T* find_child(std::string name)
	{
		for (auto child : *this->children)
		{
			if (child->name == name)
			{
				return *reinterpret_cast<T**>(&child);
			}
		}
		return 0;
	}

	template <class T>
	T* find_child_class(std::string name)
	{
		for (auto child : *this->children)
		{
			if (child->class_descriptor->class_name == name)
			{
				return *reinterpret_cast<T**>(&child);
			}
		}
		return 0;
	}
};

class RBXCharacter
{
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
	char pad_0010[0x1c];
	/* 0x2c*/
	std::string& name;
	/* 0x30 */
	std::shared_ptr<std::vector<std::shared_ptr<RBXInstance>>> children;

public:
	template <class T>
	T* find_child(std::string name)
	{
		for (auto child : *this->children)
		{
			if (child->name == name)
			{
				return *reinterpret_cast<T**>(&child);
			}
		}
		return 0;
	}

	template <class T>
	T* find_child_class(std::string name)
	{
		for (auto child : *this->children)
		{
			if (child->class_descriptor->class_name == name)
			{
				return *reinterpret_cast<T**>(&child);
			}
		}
		return 0;
	}
}; 

class RBXPlayer
{
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
	char pad_0010[0x1c]; 
	/* 0x2c */
	std::string& name;
	/* 0x30 */
	char pad_0030[0x8];
	/* 0x38 */
	class RBXPlayers* parent;
	/* 0x3c */
	char pad_003C[0x20];
	/* 0x5c */ 
	class RBXCharacter* character;
	/* 0x60 */
	char pad_0060[0x2c];
	/* 0x8c */
	int32_t team_id;
	/* 0x90*/
	char pad_0090[0xd0];
	/* 0x160 */
	int32_t user_id;
public:
	std::string get_name()
	{
		return this->name;
	}

	int32_t get_userid()
	{
		return this->user_id;
	}
};

class RBXPlayers
{
public:
	/* 0x */
	char pad_0000[0xc];
	/* 0xc */
	class RBXClassDescriptor* class_descriptor;
	/* 0x10 */
	char pad_0010[0x1c];
	/* 0x2c */
	std::string& name;
	/* 0x30 */
	std::shared_ptr<std::vector<std::shared_ptr<RBXPlayer>>> children;
	/* 0x34 */
	char pad_0034[0x4]; 
	/* 0x38 */
	class RBXDataModel* parent;
	/* 0x3c */
	char pad_003C[90];
	/* 0xCC */
	class RBXPlayer* local_player;
public:
	std::string get_name()
	{
		return this->name;
	}

	RBXPlayer* get_local_player()
	{
		return *(RBXPlayer**)(reinterpret_cast<uintptr_t>(this) + 0xCC);
	}

	RBXPlayer* get_player(std::string name)
	{
		for (auto child : *this->children)
		{
			if (child->name == name)
			{
				return *(RBXPlayer**)&child;
			}
		}
		return 0;
	}

	template <class T>
	T* find_child(std::string name)
	{
		for (auto child : *this->children)
		{
			if (child->name == name)
			{
				return *reinterpret_cast<T**>(&child);
			}
		}
		return 0;
	}

	template <class T>
	T* find_child_class(std::string name)
	{
		for (auto child : *this->children)
		{
			if (child->class_descriptor->class_name == name)
			{
				return *reinterpret_cast<T**>(&child);
			}
		}
		return 0;
	}
};

class RBXHumanoid
{
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
	char pad_0010[0x1c];
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