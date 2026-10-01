#pragma once

#include "Action.h"
#include "../Components/Gate.h"
#include "../ApplicationManager.h"
#include "SelectionTools.h"
#include "Select.h"

class AddGate : public Action
{
private:
	//Parameters for rectangular area to be occupied by the gate
	int Cx, Cy;	//Center point of the gate

	int GateType;
public:
	AddGate(int gate, ApplicationManager* pApp);
	virtual ~AddGate(void);

	//Reads parameters required for action to execute
	virtual bool ReadActionParameters();
	//Execute action (code depends on action type)
	virtual bool Execute();

	virtual void Undo();
	virtual void Redo();


};

