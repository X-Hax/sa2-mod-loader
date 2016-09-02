#include "stdafx.h"
#include "GameObject.h"


GameObject::GameObject(int index, const char *name)
{
	objData = LoadObject(CallMain, index, name);
	if (!objData)
	{
		objData->DisplaySub = CallDisplay;
		objData->DeleteSub = CallDelete;
		objData->Data2 = (CharObj2Base *)this;
	}
}

GameObject::GameObject(ObjectMaster *obj)
{
	objData = obj;
	objData->MainSub = CallMain;
	objData->DisplaySub = CallDisplay;
	objData->DeleteSub = CallDelete;
	objData->Data2 = (CharObj2Base *)this;
	initFromObj = true;
}

void GameObject::CallMain(ObjectMaster *obj)
{
	((GameObject *)obj->Data2)->Main();
}

void GameObject::CallDisplay(ObjectMaster *obj)
{
	((GameObject *)obj->Data2)->Display();
}

void GameObject::CallDelete(ObjectMaster *obj)
{
	GameObject *cgo = (GameObject *)obj->Data2;
	cgo->Delete();
	obj->Data2 = nullptr;
	cgo->objData = nullptr;
	if (cgo->initFromObj)
		delete cgo;
}

void GameObject::Display(){}

void GameObject::Delete(){}

const char *GameObject::GetName(){ return objData->Name; }

SETObjectData *GameObject::GetSETData(){ return objData->field_30; }

GameObject::~GameObject()
{
	if (objData)
	{
		Delete();
		objData->Data2 = nullptr;
		objData->DeleteSub = nullptr;
		DeleteObject_(objData);
	}
}

GameEntity::GameEntity(int index, const char *name) :GameObject(index, name)
{
	objData->Data1 = AllocCharObj1();
}

CharObj1 *GameEntity::GetData() { return objData->Data1; }

char GameEntity::GetAction() { return GetData()->Action; }

void GameEntity::SetAction(char action) { GetData()->Action = action; }

Rotation &GameEntity::GetRotation() { return GetData()->Rotation; }

typedef Rotation rot; // bleh
void GameEntity::SetRotation(rot &rotation) { GetData()->Rotation = rotation; }

NJS_VECTOR &GameEntity::GetPosition() { return GetData()->Position; }

void GameEntity::SetPosition(NJS_VECTOR &position) { GetData()->Position = position; }

NJS_VECTOR &GameEntity::GetScale() { return GetData()->Scale; }

void GameEntity::SetScale(NJS_VECTOR &scale) { GetData()->Scale = scale; }

CollisionInfo *GameEntity::GetCollisionInfo() { return GetData()->field_2C; }