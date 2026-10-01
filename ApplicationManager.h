#ifndef APPLICATION_MANAGER_H
#define APPLICATION_MANAGER_H

#include <iostream>
#include <fstream>
#include "Defs.h"
#include "GUI\Output.h"
#include "GUI\Input.h"
#include "Actions\Action.h"
#include "Components\Component.h"

//Main class that manages everything in the application.
class ApplicationManager
{
	
	enum { MaxCompCount = 200, MaxUndo = 8};	//Max no of Components	

private:
	int CompCount;		//Actual number of Components
	Component* CompList[MaxCompCount];	//List of all Components (Array of pointers)

	int ActionsCount;	//numbers of stored actions to undo
	Action* ActionList[MaxUndo];	//List of actions to undo
	
	Output* OutputInterface; //pointer to the Output Clase Interface
	Input* InputInterface; //pointer to the Input Clase Interface
	bool refresh;

public:	
	ApplicationManager(); //constructor

	//Reads the required action from the user and returns the corresponding action type
	ActionType GetUserAction(int& x, int& y);
	
	//Creates an action and executes it
	void ExecuteAction(ActionType, int, int);
	
	void UpdateInterface();	//Redraws all the drawing window

	//Gets a pointer to Input / Output Object
	Output* GetOutput();
	Input* GetInput();

	//Adds a new component to the list of components
	void AddComponent(Component* pComp);

	void AddAction(Action* pAct);	//adding a connection to undo later

	Component* FindComponent(int x, int y);	//Finding a component at a given point

	void OperateAll();	//operating the entire circuit

	Component** GetAllComponents(int*& counter);	//returns CompList

	bool Overlappin(GraphicsInfo G_Info);	//checking if a gate overlaps with anything else

	//destructor
	~ApplicationManager();
};

#endif