#pragma once
#include "Action.h"
#include "../Components/Component.h"
#include "../Components/Connection.h"
#include "../ApplicationManager.h"
#include "SelectionTools.h"
#include "Select.h"

class AddConnection: public Action
{
	int Cx, Cy;
	Connection* pA;
public:
	AddConnection(ApplicationManager* pApp);
	virtual ~AddConnection(void);

	//Execute action (code depends on action type)
	virtual bool Execute();

	void CreateConnection(Gate* gate1, Gate* gate2, int InputLocation);	//funtion to add a connection between two gates without the need for user actions

	virtual void Undo();
	virtual void Redo();
};

