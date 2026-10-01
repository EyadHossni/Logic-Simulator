#include <iostream>

#include "ApplicationManager.h"

int main()
{
	
	ActionType ActType;
	//Create an object of ApplicationManager
	ApplicationManager AppManager;
	int x = 0, y = 0;
	
	do
	{
		//Read user action
		ActType = AppManager.GetUserAction(x, y);

		//Exexute the action
		AppManager.ExecuteAction(ActType, x, y);
		
		//Update the drawing window
		AppManager.UpdateInterface();
	

	}while(ActType != EXIT);
		
	return 0;
}

