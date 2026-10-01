#pragma once
#include "Action.h"
#include "../ApplicationManager.h"
#include "../Components/Component.h"
#include "../Components/Gate.h"
#include "Operate.h"
#include "Select.h"

class CreateTruthTable : public Action
{
	int SwitchCount, LEDCount;	//amount of gates and switches in the circuit
	Component** Switchs;	//all switches in the circuit
	Component** LEDs;	//all LEDs in the circuit
	bool circuit_ready;	//if the circuit is fully connected or not
public:
	CreateTruthTable(Component** Comps, int size, ApplicationManager* pApp);
	~CreateTruthTable();

	virtual bool Execute();

	virtual void Undo();
	virtual void Redo();
};

