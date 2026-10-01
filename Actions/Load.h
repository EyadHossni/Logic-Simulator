#pragma once
#include "Action.h"
#include "../Components/Component.h"
#include "../ApplicationManager.h"
#include "../Components/Gate.h"
#include "AddConnection.h"


class Load : public Action
{
	int CompCount;
	Gate** CompList;
public:
	Load(ApplicationManager* pApp);

	~Load();

	string ReadActionParameters();

	//Execute action (code depends on action type)
	virtual bool Execute();

	void LoadByPath(string path);

	//To undo this action (code depends on action type)
	virtual void Undo();

	//To redo this action (code depends on action type)
	virtual void Redo();
};

