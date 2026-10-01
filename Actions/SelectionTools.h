#pragma once
#include "../ApplicationManager.h"
#include "Action.h"
#include "Save.h"
#include "Load.h"
#include "Paste.h"

class SelectionTools : public Action
{
	ActionType ActType;
	int* CompCount;
	Component** CompList;
	int SelectedCount;
	Component** SelectedComps;
	string past_label;
	int distanceMovedX;
	int distanceMovedY;
public:
	SelectionTools(ActionType act, Component** Comps, int* size, ApplicationManager* pApp);
	~SelectionTools();
	bool ReadActionParameters();
	virtual bool Execute();

	void Delete();
	bool Move(int x = 0, int y = 0);
	bool EditLabel(bool undo = false);
	bool Copy();

	virtual void Undo();
	virtual void Redo();
};
