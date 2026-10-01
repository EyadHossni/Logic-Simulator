#pragma once
#include "Action.h"
#include "../ApplicationManager.h"
#include "../Components/Component.h"
#include "Save.h"
#include "Load.h"

class NewProject : public Action
{
	int* CompCount;
	Component** CompList;
public:
	NewProject(Component** Comps, int* count, ApplicationManager* pApp);

	~NewProject();
	
	virtual bool Execute();

	virtual void Undo();

	virtual void Redo();
};

