#include "Input.h"
#include "Output.h"

Input::Input(window* pW)
{
	pWind = pW; //point to the passed window

	selection_mode = false;
	selectionX = 0;
	selectionY = 0;
	LabelDrawen = true;
}

void Input::GetPointClicked(int& x, int& y)
{
	pWind->WaitMouseClick(x, y);	//Wait for mouse click
}

string Input::GetSrting(Output* pOut)	//Getting a string from the user
{
	char key = 'NULL';
	int ascii_key = key;
	string written_text = "";
	string old_text = written_text;
	int x = 0, y = 0;

	while (ascii_key != 13 && pWind->GetMouseClick(x, y) == 0) {

		pWind->GetKeyPress(key);
		ascii_key = key;

		if (ascii_key >= 32 && ascii_key <= 127) written_text += key;
		else if (ascii_key == 8 && !written_text.empty()) written_text.pop_back();
		else if (ascii_key == 27) return "";

		if (old_text != written_text) { pOut->PrintMsg("> " + written_text); old_text = written_text; }	//writing what the user is writing each time he makes a change
	}

	return written_text;
	//return NULL;
}

//This function reads the position where the user clicks to determine the desired action
ActionType Input::GetUserAction(int& x, int & y) const
{
	pWind->WaitMouseClick(x, y);	//Get the coordinates of the user click

	if (UI.AppMode == DESIGN)	//application is in design mode
	{
		//[1] If user clicks on the Toolbar
		if (x >= 0 && x < UI.ToolBarWidth)
		{
			if (x < UI.ToolStartX || x > UI.ToolStartX + UI.ToolItemWidth) return DSN_TOOL;

			int ClickedItemOrder = ((y - 12) / (UI.ToolItemHeight + 5));

			switch (ClickedItemOrder)
			{
			case ITM_AND2: return ADD_AND_GATE_2;
			case ITM_AND3: return ADD_AND_GATE_3;
			case ITM_NAND: return ADD_NAND_GATE_2;
			case ITM_OR2: return ADD_OR_GATE_2;
			case ITM_BUFF: return ADD_Buff;
			case ITM_NOT: return ADD_INV;
			case ITM_NOR2: return ADD_NOR_GATE_2;
			case ITM_NOR3: return ADD_NOR_GATE_3;
			case ITM_XOR2: return ADD_XOR_GATE_2;
			case ITM_XOR3: return ADD_XOR_GATE_3;
			case ITM_XNOR: return ADD_XNOR_GATE_2;
			case ITM_SWITCH: return ADD_Switch;
			case ITM_LED: return ADD_LED;
			case ITM_CONNECTION: return ADD_CONNECTION;
			case ITM_EXIT: return EXIT;

			default: return DSN_TOOL;	//A click on empty place in desgin toolbar
			}
		}

		//[2] User clicks on the Status bar
		else if (x >= UI.ToolBarWidth && y > UI.height - UI.StatusBarHeight) return STATUS_BAR;	//user want to select/unselect a component

		//[3] User clickes on changing the menu type
		else if (x >= UI.ModeButtonX && y >= UI.ModeButtonY && x <= UI.ModeButtonX + UI.ModeButtonWidth && y <= UI.ModeButtonY + UI.ModeButtonHeight) return SIM_MODE;

		//[4] User clicks on the selection menu
		else if (selection_mode && x > selectionX && y > selectionY && y < selectionY + UI.SelectionToolSize + 10 && x < selectionX + ITM_SELECT_CNT * UI.SelectionToolSize + (ITM_SELECT_CNT + 1) * 10) {
			int ClickedItemOrder = ((x - selectionX - 10) / (UI.SelectionToolSize + 5));
			
			//Fixing the itemOrder depending on weather the EDIT_LABEL option is on or not
			if (LabelDrawen) ClickedItemOrder += EDIT_Label;
			else ClickedItemOrder += MOVE;

			return (ActionType)ClickedItemOrder;
		}

		//[5] User clicks on the top tool bar
		else if (x >= UI.ToolBarWidth && y > UI.ModeButtonY && y < UI.ModeButtonY + UI.ModeButtonHeight && x < UI.ToolBarWidth + (ITM_TOP_MENU_CNT + 1) * 10 + ITM_TOP_MENU_CNT * UI.TopMenuButtonWidth + UI.TopMenuButtonWidth) {
			int ClickedItemOrder = ((x - UI.ToolBarWidth - 10) / (UI.TopMenuButtonWidth + 5));
			switch (ClickedItemOrder) {
			case ITM_NEW_PROJECT: return CREATE_NEW_PROJECT;
			case ITM_UNDO: return UNDO;
			case ITM_SAVE: return SAVE;
			case ITM_LOAD: return LOAD;
			case ITM_PASTE: return PASTE;
			}
		}

		//[5] User clicks on the drawing area
		return Drawing_Area;
	}
	else	//Application is in Simulation mode
	{
		if (y > UI.height - UI.StatusBarHeight) return STATUS_BAR;	//user clicks on the status bar

		else if (x >= UI.ModeButtonX && y >= UI.ModeButtonY && x <= UI.ModeButtonX + UI.ModeButtonWidth && y <= UI.ModeButtonY + UI.ModeButtonHeight) return DSN_MODE; // user changes the mode back to design

		else if (x >= 10 && x <= 10 + UI.ModeButtonWidth && y > UI.ModeButtonY && y < UI.ModeButtonY + UI.ModeButtonHeight) return Create_TruthTable;	// user wants to generate the truth table

		return Drawing_Area;
	}

}

//Getting the correct Y for drawing the connection for a gate
int Input::GetConnectionY(GraphicsInfo G_Info, int counter, bool input) {
	int c = 1;
	int x = (input) ? G_Info.x1 + 10 : G_Info.x2 - 10;
	bool Inside = false;
	int locY = 0;
	int pointsnum = 0;

	for (int i = G_Info.y1; i < G_Info.y2; i++) {
		if (pWind->GetColor(x, i).ColorCmp(UI.ConnColor) >= 0.9) {	//Comparing every pixel in the gate untill it finds the one for connection
			locY += i;
			pointsnum++;
			Inside = true;
		}
		else if (pWind->GetColor(x, i).ColorCmp(UI.ConnColor) <= 0.5 && Inside) {	//Finds the end of a connection
			if (c == counter) return locY / pointsnum;
			else {
				locY = 0;
				pointsnum = 0;
				Inside = false;
				c++;
			}
		}
	}
	return -1;
}

void Input::GetMousePos(int& Cx, int& Cy) {
	pWind->GetMouseCoord(Cx, Cy);
}

//Setting the x and y of the slection menu
void Input::setSelection(int x, int y, bool Label) {
	selection_mode = true;
	selectionX = x;
	selectionY = y;
	LabelDrawen = Label;
}

//removing the selection menu
void Input::ClearSelection() {
	selection_mode = false;
	selectionX = 0;
	selectionY = 0;
}

//indicating the paste button is on or off
void Input::SetPaste(bool paste) {
	PasteButton = paste;
}


Input::~Input()
{
}
