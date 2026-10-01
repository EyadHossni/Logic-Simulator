#pragma once
#include "../ApplicationManager.h"
#include "../Components/Component.h"
#include "../Components/Gate.h"
#include "../Components/Connection.h"

const int MaxcompCount = 200;

class Save: public Action
{
	int CompCount;
	Component* CompList[MaxcompCount];
public:
	Save(Component* Comp_List[], int count, ApplicationManager* pApp);

	~Save();

	bool SavePart(string path, bool Copying = false);

	//Execute action (code depends on action type)
	virtual bool Execute();

	//To undo this action (code depends on action type)
	virtual void Undo();

	//To redo this action (code depends on action type)
	virtual void Redo();
};

