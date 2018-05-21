#pragma once
#include "SA2ModLoader.h"
class GameObject
{
private:
	static void CallMain(ObjectMaster *obj);
	static void CallDisplay(ObjectMaster *obj);
	static void CallDelete(ObjectMaster *obj);
	bool initFromObj;
protected:
	ObjectMaster *objData;
	GameObject(int index, const char *name = "GameObject");
	GameObject(ObjectMaster *obj);
	virtual void Delete();
public:
	~GameObject();
	virtual void Main() = 0;
	virtual void Display();
	template <typename T>
	static void Load(ObjectMaster *obj)
	{
		T *t = new T(obj);
		t->Main();
	}
	const char *GetName();
	SETObjectData *GetSETData();
	// MSVC-specific property emulation.
#ifdef _MSC_VER
	__declspec(property(get = GetName)) const char *Name;
	__declspec(property(get = GetSETData)) SETObjectData *SETData;
#endif
};

class GameEntity : GameObject
{
protected:
	EntityData1 *GetData();
public:
	GameEntity(int index, const char *name = "GameEntity");
	char GetAction();
	void SetAction(char action);
	Rotation &GetRotation();
	void SetRotation(Rotation &rotation);
	NJS_VECTOR &GetPosition();
	void SetPosition(NJS_VECTOR &position);
	NJS_VECTOR &GetScale();
	void SetScale(NJS_VECTOR &scale);
	CollisionInfo *GetCollisionInfo();
	// MSVC-specific property emulation.
#ifdef _MSC_VER
	__declspec(property(get = GetAction, put = SetAction)) char Action;
	__declspec(property(get = GetRotation, put = SetRotation)) Rotation &Rotation;
	__declspec(property(get = GetPosition, put = SetPosition)) NJS_VECTOR &Position;
	__declspec(property(get = GetScale, put = SetScale)) NJS_VECTOR &Scale;
	__declspec(property(get = GetCollisionInfo)) CollisionInfo *CollisionInfo;
#endif
};