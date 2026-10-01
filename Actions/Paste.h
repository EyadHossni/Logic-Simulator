#pragma once
#include "Action.h"
#include "../ApplicationManager.h"
#include "Load.h"

class Load;

class Paste : public Action
{
	Load* pAct;
public:
	Paste(ApplicationManager* pApp);

	~Paste();
	
	virtual bool Execute();
	
	virtual void Undo();

	virtual void Redo();
};

