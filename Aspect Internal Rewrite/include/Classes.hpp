#pragma once
#include <Includes.hpp>
#include <FunctionWrapper.hpp>

using Humanoid_ChangeStateFunc = int(*)(std::uintptr_t Humanoid, int state);
extern FunctionWrapper<Humanoid_ChangeStateFunc> ChangeStateFunc;

struct vec2
{
	float x, y;
};

struct vec3x
{
	float x, y, z;
};

struct vec4
{
	float x, y, z, w;
};

class vec3 {

public:

	union {
		float data[3];
		struct {
			float x;
			float y;
			float z;
		};
	};

	// Constructors

	// Vectors default to 0, 0, 0.
	vec3() {
		x = 0;
		y = 0;
		z = 0;
	}

	// Construct with values, 3D
	vec3(float ax, float ay, float az) {
		x = ax;
		y = ay;
		z = az;
	}

	// Construct with values, 2D
	vec3(float ax, float ay) {
		x = ax;
		y = ay;
		z = 0;
	}

	// Copy constructor
	vec3(const vec3& o) {
		x = o.x;
		y = o.y;
		z = o.z;
	}

	// Addition

	vec3 operator+(const vec3& o) {
		return vec3(x + o.x, y + o.y, z + o.z);
	}

	vec3& operator+=(const vec3& o) {
		x += o.x;
		y += o.y;
		z += o.z;
		return *this;
	}

	// Subtraction

	vec3 operator-() {
		return vec3(-x, -y, -z);
	}

	vec3 operator-(const vec3 o) {
		return vec3(x - o.x, y - o.y, z - o.z);
	}

	vec3& operator-=(const vec3 o) {
		x -= o.x;
		y -= o.y;
		z -= o.z;
		return *this;
	}

	// Multiplication by scalars

	vec3 operator*(const float s) {
		return vec3(x * s, y * s, z * s);
	}

	vec3& operator*=(const float s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}

	// Division by scalars

	vec3 operator/(const float s) {
		return vec3(x / s, y / s, z / s);
	}

	vec3& operator/=(const float s) {
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	// Dot product

	float operator*(const vec3 o) {
		return (x * o.x) + (y * o.y) + (z * o.z);
	}

	// An in-place dot product does not exist because
	// the result is not a vector.

	// Cross product

	vec3 operator^(const vec3 o) {
		float nx = y * o.z - o.y * z;
		float ny = z * o.x - o.z * x;
		float nz = x * o.y - o.x * y;
		return vec3(nx, ny, nz);
	}

	vec3& operator^=(const vec3 o) {
		float nx = y * o.z - o.y * z;
		float ny = z * o.x - o.z * x;
		float nz = x * o.y - o.x * y;
		x = nx;
		y = ny;
		z = nz;
		return *this;
	}

	// Other functions

	// Length of vector
	float magnitude() {
		return sqrt(magnitude_sqr());
	}

	// Length of vector squared
	float magnitude_sqr() {
		return (x * x) + (y * y) + (z * z);
	}

	// Returns a normalised copy of the vector
	// Will break if it's length is 0
	vec3 normalised() {
		return vec3(*this) / magnitude();
	}

	// Modified the vector so it becomes normalised
	vec3& normalise() {
		(*this) /= magnitude();
		return *this;
	}

};

// Created with ReClass.NET by KN4CK3R

class RBXBody
{
public:

	float GetXPosition()
	{
		return *(float*)(reinterpret_cast<uintptr_t>(this) + 0xD4);
	}

	float GetYPosition()
	{
		return *(float*)(reinterpret_cast<uintptr_t>(this) + 0xD8);
	}

	float GetZPosition()
	{
		return *(float*)(reinterpret_cast<uintptr_t>(this) + 0xDC);
	}

	void SetXPosition(float x)
	{
		*(float*)(reinterpret_cast<uintptr_t>(this) + 0xD4) = x;
	}

	void SetYPosition(float y)
	{
		*(float*)(reinterpret_cast<uintptr_t>(this) + 0xD8) = y;
	}

	void SetZPosition(float z)
	{
		*(float*)(reinterpret_cast<uintptr_t>(this) + 0xDC) = z;
	}

	vec3 GetPosition()
	{
		return *(vec3*)(reinterpret_cast<uintptr_t>(this) + 0xD4);
	}

	void SetPosition(vec3 newposition)
	{
		*(vec3*)(reinterpret_cast<uintptr_t>(this) + 0xD4) = newposition;
	}

	vec3 GetVelocity()
	{
		return *(vec3*)(reinterpret_cast<uintptr_t>(this) + 0xE0);
	}
}; //Size: 0x00EC

class RBXPrimitive
{
public:
	char pad_0000[128]; //0x0000
	class RBXBody* Body; //0x0080

public:
	RBXBody* GetBody()
	{
		return *(RBXBody * *)(reinterpret_cast<uintptr_t>(this) + 0x80);
	}
}; //Size: 0x0084

class RBXInstance
{
public:
	char pad_0000[4]; //0x0000
	class RBXInstance* Self; //0x0004
	char pad_0008[4]; //0x0008
	class RBXClassDescriptor* ClassDescriptor; //0x000C
	char pad_0010[28]; //0x0010
	char* Name; //0x002C
	std::shared_ptr<std::vector<std::shared_ptr<RBXInstance>>> Children; //0x0030
	char pad_0034[100]; //0x0034
	class RBXPrimitive* Primitive; //0x0098
public:
	template <class T>
	T* FindFirstChild(std::string Name)
	{
		for (auto Child : *this->Children)
		{
			if (Child->Name == Name)
			{
				return *reinterpret_cast<T * *>(&Child);
			}
		}
		return NULL;
		//throw "Unable to find child!";
	}

	template <class T>
	T* FindFirstChildOfClass(std::string Name)
	{
		for (auto Child : *this->Children)
		{
			if (Child->ClassDescriptor->ClassName == Name)
			{
				//std::cout << "Returning: " << Child->Name << std::endl;
				return *reinterpret_cast<T * *>(&Child);
			}
		}
		return NULL;
		//throw "Unable to find child!";
	}

	RBXPrimitive* GetPrimitive()
	{
		return *(RBXPrimitive * *)(reinterpret_cast<uintptr_t>(this) + 0x98);
	}

	float GetCurrentHealth()
	{
		try
		{
			uint32_t Humanoid = reinterpret_cast<uint32_t>(this);
			uint32_t PointerBase = *(uint32_t*)(Humanoid + 0x1C8);
			uint32_t Pointer2 = *(uint32_t*)(PointerBase);
			uint32_t DecryptedPtr = PointerBase ^ Pointer2;
			return *reinterpret_cast<float*>(&DecryptedPtr);
		}
		catch (std::exception e)
		{

		}
	}

	float GetMaxHealth()
	{
		try
		{
			uint32_t Humanoid = reinterpret_cast<uint32_t>(this);
			uint32_t PointerBase = *(uint32_t*)(Humanoid + 0x1CC);
			uint32_t Pointer2 = *(uint32_t*)(PointerBase);
			uint32_t DecryptedPtr = PointerBase ^ Pointer2;
			return *reinterpret_cast<float*>(&DecryptedPtr);
		}
		catch (std::exception e)
		{

		}
	}
}; //Size: 0x009C

class RBXClassDescriptor
{
public:
	char pad_0000[4]; //0x0000
	std::string& ClassName; //0x0004
}; //Size: 0x0008

class RBXServicesPtr
{
public:
	int32_t ServicesStart; //0x0000
	int32_t ServicesEnd; //0x0004
}; //Size: 0x0008

class RBXService
{
public:
	char pad_0000[4]; //0x0000
	class RBXService* Self; //0x0004
	char pad_0008[4]; //0x0008
	class RBXClassDescriptor* ClassDescriptor; //0x000C
	char pad_0010[28]; //0x0010
	char* Name; //0x002C
	std::shared_ptr<std::vector<std::shared_ptr<RBXService>>> Children; //0x0030
	char pad_0034[4]; //0x0034
	class RBXDataModel* Parent; //0x0038

public:
	template <class T>
	T* FindFirstChild(std::string Name)
	{
		for (auto Child : *this->Children)
		{
			if (Child->Name == Name)
			{
				return *reinterpret_cast<T * *>(&Child);
			}
		}
		return NULL;
	}

	template <class T>
	T* FindFirstChildOfClass(std::string Name)
	{
		for (auto Child : *this->Children)
		{
			if (Child->ClassDescriptor->ClassName == Name)
			{
				//std::cout << "Returning: " << Child->Name << std::endl;
				return *reinterpret_cast<T * *>(&Child);
			}
		}
		return NULL;
	}
};

class RBXDataModel
{
public:
	char pad_0000[12]; //0x0000
	class RBXClassDescriptor* ClassDescriptor; //0x000C
	char pad_0010[28]; //0x0010
	std::string& Name; //0x002C
	std::shared_ptr<std::vector<std::shared_ptr<RBXService>>> Children; //0x0030
	char pad_0034[12]; //0x0034
public:
	template <class T>
	T* GetChildren()
	{
		std::vector<T> ChildrenList;
		for (auto Child : *this->Children)
		{
			ChildrenList.push_back(*(T*)i);
		}
		return ChildrenList;
	}

	template <class T>
	T* FindFirstChild(std::string Name)
	{
		for (auto Child : *this->Children)
		{
			if (Child->Name == Name)
			{
				return *reinterpret_cast<T**>(&Child);
			}
		}
		return NULL;
	}

	template <class T>
	T* FindFirstChildOfClass(std::string Name)
	{
		for (auto Child : *this->Children)
		{
			if (Child->ClassDescriptor->ClassName == Name)
			{
				//std::cout << "Returning: " << Child->Name << std::endl;
				return *reinterpret_cast<T**>(&Child);
			}
		}
		return NULL;
	}
};

class RBXCharacter
{
public:
	char pad_0000[4]; //0x0000
	class RBXCharacter* Self; //0x0004
	char pad_0008[4]; //0x0008
	class RBXClassDescriptor* ClassDescriptor; //0x000C
	char pad_0010[28]; //0x0010
	std::string& Name; //0x002C
	std::shared_ptr<std::vector<std::shared_ptr<RBXInstance>>> Children;//0x0030

public:
	template <class T>
	T* FindFirstChild(std::string Name)
	{
		for (auto Child : *this->Children)
		{
			if (Child->Name == Name)
			{
				return *reinterpret_cast<T * *>(&Child);
			}
		}
		return NULL;
	}

	template <class T>
	T* FindFirstChildOfClass(std::string Name)
	{
		for (auto Child : *this->Children)
		{
			if (Child->ClassDescriptor->ClassName == Name)
			{
				//std::cout << "Returning: " << Child->Name << std::endl;
				return *reinterpret_cast<T * *>(&Child);
			}
		}
		return NULL;
	}
}; //Size: 0x0044

class RBXPlayer
{
public:
	char pad_0000[4]; //0x0000
	class RBXInstance* Self; //0x0004
	char pad_0008[4]; //0x0008
	class RBXClassDescriptor* ClassDescriptor; //0x000C
	char pad_0010[28]; //0x0010
	std::string& Name; //0x002C
	char pad_0030[8]; //0x0030
	class RBXPlayers* Parent; //0x0038
	char pad_003C[32]; //0x003C
	class RBXCharacter* Character; //0x005C
	char pad_0060[44]; //0x0060
	int32_t TeamID; //0x008C
	char pad_0090[208]; //0x0090
	int32_t UserId; //0x0160
public:
	std::string GetName()
	{
		return this->Name;
	}

	int32_t GetUserId()
	{
		return this->UserId;
	}
}; //Size: 0x0164

class AspectPlayers
{
public:
	std::vector<RBXPlayer*> ChildrenList;
};

class RBXPlayers
{
public:
	char pad_0000[12]; //0x0000
	class RBXClassDescriptor* ClassDescriptor; //0x000C
	char pad_0010[28]; //0x0010
	std::string& Name; //0x002C
	std::shared_ptr<std::vector<std::shared_ptr<RBXPlayer>>> Children;//0x0030
	char pad_0034[4]; //0x0034
	class RBXDataModel* Parent; //0x0038
	char pad_003C[144]; //0x003C
	class RBXPlayer* LocalPlayer; //0x00CC
private:
	std::vector<RBXPlayer*> ChildrenList;
	sol::table ChildListAsTable;
public:
	std::string GetName()
	{
		return this->Name;
	}

	RBXPlayer* GetLocalPlayer()
	{
		return *(RBXPlayer**)(reinterpret_cast<uintptr_t>(this) + 0xCC);
	}

	RBXPlayer* GetPlayer(std::string Name)
	{
		for (auto Child : *this->Children)
		{
			if (Child->Name == Name)
			{
				return *reinterpret_cast<RBXPlayer * *>(&Child);
			}
		}
		return NULL;
	}

	void UpdatePlayersTable()
	{
		ChildListAsTable.clear();
		for (auto Child : *this->Children)
		{
			ChildListAsTable.add(Child.get());
		}
	}

	sol::as_table_t<std::vector<RBXPlayer*>> GetPlayers()
	{
		return sol::as_table(ChildrenList);
	}

	template <class T>
	T* FindFirstChild(std::string Name)
	{
		for (auto Child : *this->Children)
		{
			if (Child->Name == Name)
			{
				return *reinterpret_cast<T**>(&Child);
			}
		}
		return NULL;
	}

	template <class T>
	T* FindFirstChildOfClass(std::string Name)
	{
		for (auto Child : *this->Children)
		{
			if (Child->ClassDescriptor->ClassName == Name)
			{
				//std::cout << "Returning: " << Child->Name << std::endl;
				return *reinterpret_cast<T**>(&Child);
			}
		}
		return NULL;
	}
}; //Size: 0x00D0

class RBXHumanoid
{
public:
	char pad_0000[4]; //0x0000
	class RBXHumanoid* Self; //0x0004
	char pad_0008[4]; //0x0008
	class RBXClassDescriptor* ClassDescriptor; //0x000C
	char pad_0010[28]; //0x0010
	std::string& Name; //0x002C
	char pad_0030[396]; //0x0030
	uintptr_t WalkSpeedPtr; //0x01BC
	char pad_01C0[8]; //0x01C0
	uintptr_t HealthPtr; //0x01C8
	uintptr_t MaxHealthPtr; //0x01CC
	char pad_01D0[4]; //0x01D0
	uintptr_t JumpPowerPtr; //0x01D4
};