#pragma once

#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "UI_Info.h"

class Output;
class Input		//The application manager should have a pointer to this class
{
private:
	window* pWind;	//Pointer to the Graphics Window
	bool selection_mode;	//Boolean to identify which mode it is
	int selectionX, selectionY;		//Location of the Selection menu
	bool LabelDrawen;		//checking if the EDIT_LABEL option is avaliable or not
	bool PasteButton;		//Checking if the paste button is drawn or not
public:
	Input(window*);
	void GetPointClicked(int&, int&);	//Get coordinate where user clicks
	string GetSrting(Output*);		//Returns a string entered by the user

	ActionType GetUserAction(int& x, int& yz) const; //Reads the user click and maps it to an action

	int GetConnectionY(GraphicsInfo G_Info, int counter, bool input);	//Getting the correct position to draw the connection

	void GetMousePos(int& Cx, int& Cy);		//Getting mouse pos at any point

	void setSelection(int x, int y, bool Label);	//indicating that the selection window is open

	void SetPaste(bool paste);	//Indicating that the paste button is drawn or not

	void ClearSelection();	//showing that the selection menu was removed

	~Input();
};
