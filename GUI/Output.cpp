#include "Output.h"
#include "../Components/Gate.h"

Output::Output()
{
	//Initialize user interface parameters
	
	UI.AppMode = DESIGN;	//Design Mode is the startup mode
	PasteButton = false;
	for (int i = 0; i < 13; i++) {
		UI.GateLength[i] /= UI.drawing_factor;
		UI.GateHeight[i] /= UI.drawing_factor;
	}
	
	//Create the drawing window
	pWind = CreateWind(UI.width, UI.height, UI.wx, UI.wy);
	//pWind = new window(UI.width, UI.height, UI.wx, UI.wy);
	ChangeTitle("Logic Simulator");

	CreateDesignToolBar();	//Create the desgin toolbar

}

Input* Output::CreateInput() const
{
	//Create an Input Object & assign it to the Same Window
	Input* pIn = new Input(pWind);
	return pIn;
}


//======================================================================================//
//								Interface Functions										//
//======================================================================================//

window* Output::CreateWind(int wd, int h, int x, int y) const
{
	window* wind = new window(wd, h, x, y);
	return wind;
}
//////////////////////////////////////////////////////////////////////////////////
void Output::ChangeTitle(string Title) const
{
	pWind->ChangeTitle(Title);
}
//////////////////////////////////////////////////////////////////////////////////
void Output::CreateStatusBar() const
{
	int startX = (UI.AppMode == DESIGN) ? UI.ToolBarWidth : 0;
	pWind->SetPen(UI.EdgesPen, 3);
	pWind->DrawLine(startX, UI.height - UI.StatusBarHeight, UI.width, UI.height - UI.StatusBarHeight);
}
//////////////////////////////////////////////////////////////////////////////////
void Output::PrintMsg(string msg) const
{
	ClearStatusBar();	//Clear Status bar to print message on it
	// Set the Message offset from the Status Bar
	int MsgX = (UI.AppMode == DESIGN) ? UI.ToolBarWidth + 10 : 10;
	int MsgY = UI.StatusBarHeight - 10;
		
	// Print the Message
	pWind->SetFont(20, BOLD | ITALICIZED, BY_NAME, "Arial");
	pWind->SetPen(UI.MsgColor);
	pWind->DrawString(MsgX, UI.height - MsgY, msg);
}

void Output::DrawMsg(string msg, GraphicsInfo g_info, int size, bool conn) const {
	pWind->SetFont(size, BOLD | ITALICIZED, BY_NAME, "Arial");
	pWind->SetPen(UI.ConnColor);
	
	//Find the right location to draw the label for a gate
	if (conn) pWind->DrawString((g_info.x1 + (g_info.x2 - g_info.x1 - msg.length() * 20) / 2), g_info.y1 - 20, msg);
	else pWind->DrawString((g_info.x1 + (g_info.x2 - g_info.x1 - msg.length() * 10) / 2), g_info.y1 - 20, msg);
}
//////////////////////////////////////////////////////////////////////////////////
void Output::ClearStatusBar()const
{
	// Set the Message offset from the Status Bar
	int MsgX = (UI.AppMode == DESIGN) ? UI.ToolBarWidth + 10 : 10;
	int MsgY = UI.StatusBarHeight - 10;

	//Overwrite using bachground color to erase the message
	pWind->SetPen(WHITE);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(MsgX, UI.height - MsgY, UI.width, UI.height);
}
//////////////////////////////////////////////////////////////////////////////////
//Clears the drawing (degin) area
void Output::ClearDrawingArea() const
{
	pWind->SetPen(UI.EdgesPen, 3);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(UI.ToolBarWidth, 0, UI.width, UI.height - UI.StatusBarHeight);

	CreateTopToolBar();

}
//////////////////////////////////////////////////////////////////////////////////
//Draws the menu (toolbar) in the Design mode
void Output::CreateDesignToolBar() const
{
	UI.AppMode = DESIGN;
	ClearDrawingArea();
	CreateStatusBar();		//Create Status bar


	//You can draw the tool bar icons in any way you want.
	pWind->SetPen(UI.EdgesPen, 1);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(0, 0, UI.ToolBarWidth,UI.height);

	//First prepare List of images for each menu item
	string MenuItemImages[ITM_DSN_CNT];
	MenuItemImages[ITM_AND2] = "images\\and-gate.jpg";
	MenuItemImages[ITM_AND3] = "images\\and-gate3.jpg";
	MenuItemImages[ITM_NAND] = "images\\nand-gate.jpg";
	MenuItemImages[ITM_OR2] = "images\\or-gate.jpg";
	MenuItemImages[ITM_BUFF] = "images\\buffer-gate.jpg";
	MenuItemImages[ITM_NOT] = "images\\not-gate.jpg";
	MenuItemImages[ITM_NOR2] = "images\\nor-gate.jpg";
	MenuItemImages[ITM_NOR3] = "images\\nor-gate3.jpg";
	MenuItemImages[ITM_XOR2] = "images\\xor-gate.jpg";
	MenuItemImages[ITM_XOR3] = "images\\xor-gate3.jpg";
	MenuItemImages[ITM_XNOR] = "images\\xnor-gate.jpg";
	MenuItemImages[ITM_SWITCH] = "images\\switch-gate.jpg";
	MenuItemImages[ITM_LED] = "images\\led.jpg";
	MenuItemImages[ITM_CONNECTION] = "images\\connection.jpg";
	MenuItemImages[ITM_EXIT] = "images\\exit.jpg";

	//TODO: Prepare image for each menu item and add it to the list
	
	//Draw menu item one image at a time
	for (int i = 0; i < ITM_DSN_CNT; i++) {
		string gate_name = UI.GATENAMES[i];
		int y = i * (UI.ToolItemHeight + 5) + 12;
		pWind->SetPen(BLACK, 1);
		pWind->SetBrush(WHITE);
		pWind->DrawRectangle(UI.ToolStartX, y, UI.ToolStartX + UI.ToolItemWidth, y + UI.ToolItemHeight, FRAME, UI.ToolItemHeight / 2, UI.ToolItemHeight / 2);

		pWind->SetFont(20, BOLD | ITALICIZED, BY_NAME, "Arial");
		pWind->SetPen(BLACK);

		//Draws LED with different dimensions than the rest of the gates
		if (i == ITM_LED) {
			pWind->DrawImage(MenuItemImages[i], UI.ToolStartX + 30, y + 7.5, UI.ToolItemHeight / 1.5, UI.ToolItemHeight / 1.5);
			pWind->DrawString(UI.ToolStartX + UI.ToolItemWidth / 2 + 15, y + 15, gate_name);
		}
		else {
			pWind->DrawImage(MenuItemImages[i], UI.ToolStartX + 5, y + 7.5, UI.ToolItemWidth / 2, UI.ToolItemHeight / 1.5);
			if (gate_name.length() >= 7) pWind->DrawString(UI.ToolStartX + UI.ToolItemWidth / 2 + 5, y + 15, gate_name);
			else pWind->DrawString(UI.ToolStartX + UI.ToolItemWidth / 2 + 15, y + 15, gate_name);
		}
	}
	CreateTopToolBar();
}

// Draw the top toolbar (Includes Save - Load - New Project - Undo)
void Output::CreateTopToolBar() const {
	int amount = (PasteButton) ? ITM_TOP_MENU_CNT : ITM_TOP_MENU_CNT - 1;

	string TopMenuItemImages[ITM_TOP_MENU_CNT];
	TopMenuItemImages[ITM_NEW_PROJECT] = "images\\new-project.jpg";
	TopMenuItemImages[ITM_UNDO] = "images\\undo.jpg";
	TopMenuItemImages[ITM_SAVE] = "images\\save.jpg";
	TopMenuItemImages[ITM_LOAD] = "images\\load.jpg";
	TopMenuItemImages[ITM_PASTE] = "images\\paste.jpg";

	for (int i = 0; i < amount; i++) {
		int x = UI.ToolBarWidth + (i + 1) * 10 + i * UI.TopMenuButtonWidth;
		pWind->SetPen(UI.EdgesPen, 2);
		pWind->SetBrush(UI.BkGrndColor);
		pWind->DrawRectangle(x, UI.ModeButtonY, x + UI.TopMenuButtonWidth, UI.ModeButtonY + UI.ModeButtonHeight, FILLED, 20, 20);

		pWind->DrawImage(TopMenuItemImages[i], x + 5, UI.ModeButtonY + 5, UI.ModeButtonHeight - 10, UI.ModeButtonHeight - 10);

		pWind->SetFont(17, BOLD | ITALICIZED, BY_NAME, "Arial");
		pWind->SetPen(BLACK);
		pWind->DrawString(x + 40, UI.ModeButtonY + 12.5, UI.GATENAMES[i + ITM_DSN_CNT]);
	}

	CreateModeChangeButton();
}

// Program State Identifier Drawing
void Output::CreateModeChangeButton() const {
	string image = (UI.AppMode == DESIGN) ? "images\\design-mode.jpg" : "images\\simulation-mode.jpg";
	string mode = (UI.AppMode == DESIGN) ? "DESIGN MODE" : "SIMULATION MODE";
	int font_size = (UI.AppMode == DESIGN) ? 17 : 15;

	pWind->SetPen(UI.EdgesPen, 2);
	pWind->SetBrush(UI.BkGrndColor);
	pWind->DrawRectangle(UI.ModeButtonX, UI.ModeButtonY, UI.ModeButtonX + UI.ModeButtonWidth, UI.ModeButtonY + UI.ModeButtonHeight, FILLED, 20, 20);

	pWind->DrawImage(image, UI.ModeButtonX + 5, UI.ModeButtonY + 5, UI.ModeButtonHeight - 10, UI.ModeButtonHeight - 10);

	pWind->SetFont(font_size, BOLD | ITALICIZED, BY_NAME, "Arial");
	pWind->SetPen(BLACK);
	pWind->DrawString(UI.ModeButtonX + 40, UI.ModeButtonY + 12.5, mode);
}
//////////////////////////////////////////////////////////////////////////////////////////
int Output::DrawSelectionToolbar(int x, int y, bool DrawLabel) {
	int start = 0, SelectCount = ITM_SELECT_CNT;
	if (!DrawLabel) {
		start++;
		SelectCount--;
	}
	x -= (SelectCount * UI.SelectionToolSize + (SelectCount + 1) * 10) / 2;

	if (x < UI.ToolBarWidth) x = UI.ToolBarWidth;
	string MenuItemImages[ITM_SELECT_CNT];
	MenuItemImages[ITM_LABEL] = "Images\\label.jpg";
	MenuItemImages[ITM_MOVE] = "Images\\move.jpg";
	MenuItemImages[ITM_DELETE] = "Images\\delete.jpg";
	MenuItemImages[ITM_COPY] = "Images\\copy.jpg";
	MenuItemImages[ITM_CUT] = "Images\\cut.jpg";

	pWind->SetPen(BLACK, 1);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(x, y, x + SelectCount *UI.SelectionToolSize + (SelectCount + 1) * 10, y + UI.SelectionToolSize + 10, FILLED, 10, 10);

	for (int i = start; i < ITM_SELECT_CNT; i++) {
		pWind->DrawImage(MenuItemImages[i], x + (i - start) * UI.SelectionToolSize + ((i - start)+1) * 10, y + 5, UI.SelectionToolSize, UI.SelectionToolSize);
	}
	return x;
}

//////////////////////////////////////////////////////////////////////////////////////////
//Draws the menu (toolbar) in the simulation mode
void Output::CreateSimulationToolBar() const
{
	UI.AppMode = SIMULATION;

	ClearScreen();
	ClearStatusBar();
	CreateStatusBar();		//Create Status bar

	string image = "images\\truth-table.jpg";

	pWind->SetPen(UI.EdgesPen, 2);
	pWind->SetBrush(UI.BkGrndColor);
	pWind->DrawRectangle(10, UI.ModeButtonY, 10 + UI.ModeButtonWidth, UI.ModeButtonY + UI.ModeButtonHeight, FILLED, 20, 20);

	pWind->DrawImage(image, 30, UI.ModeButtonY + 2.5, UI.ModeButtonHeight - 10, UI.ModeButtonHeight - 5);

	pWind->SetFont(20, BOLD | ITALICIZED, BY_NAME, "Arial");
	pWind->SetPen(BLACK);
	pWind->DrawString(75, UI.ModeButtonY + 10, "Truth Table");

	CreateModeChangeButton();

	PrintMsg("Action: Switch to Simulation Mode, creating simualtion tool bar");

}

//Drawing the truth table depending on a two dimensional array (Table) of numbers
void Output::DrawTruthTable(int** Table, int SwitchCount, int LEDCount, int startY, int& Dx, int& Dy) {
	int SizeX = SwitchCount + LEDCount;
	int SizeY = pow(2, SwitchCount) + 1;

	Dx = UI.ToolBarWidth / SizeX;
	Dy = Dx / 2;
	if (Dy < 30) {		//Ensuring the size remains consistent
		Dy = 30;
	}

	pWind->SetPen(UI.ConnColor, 1);

	for (int i = 0; i <= SizeX; i++) {
		pWind->DrawLine(10 + i * Dx, startY, 10 + i * Dx, startY + Dy * SizeY);		//Ploting the vertical lines

		GraphicsInfo Box;
		Box.x1 = 20 + i*Dx;
		Box.x2 = 10 + (i + 1) * Dx;
		Box.y1 = startY + Dy;

		int font_size = (Dx / 2.5 > 20) ? 20 : Dx / 2.5;
		if (i < SwitchCount) DrawMsg("Sw" + to_string(i + 1), Box, font_size);
		else if (i != SizeX) DrawMsg("LED" + to_string(i - SwitchCount + 1), Box, font_size);	//Writing the names for switches and LEDs
	}
	for (int i = 0; i <= SizeY; i++) {
		pWind->DrawLine(10 , startY + i*Dy, 10 + SizeX * Dx, startY + i * Dy);		//Ploting the Horizontal lines
	}

	for (int i = 0; i < SizeX; i++) {
		for (int k = 0; k < SizeY - 1; k++) {

			GraphicsInfo Box;
			Box.x1 = 10 + i * Dx;
			Box.x2 = 10 + (i + 1) * Dx;
			Box.y1 = startY + 2* Dy + Dy*k;

			DrawMsg(to_string(Table[i][k]), Box);	//Writing all the numbers in their places from the table
		}
	}
}

void Output::ClearScreen() const {
	pWind->SetPen(WHITE, 2);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(0, 0, UI.width, UI.height);
}

//======================================================================================//
//								Components Drawing Functions							//
//======================================================================================//

void Output::DrawComponent(Component* component, STATUS status, bool selected, bool correct) const
{
	int GateType = ((Gate*)(component))->GetType();
	GraphicsInfo r_GfxInfo = ((Gate*)(component))->GetGraphics();
	string GateImage;
	int height = UI.GateHeight[GateType], width = UI.GateLength[GateType];
	if (UI.AppMode == DESIGN) status = LOW;

	//Drawing all gates depending on if selected or not or if not connected correctly in simulation mode
	if (status == LOW) {
		if (GateType == ITM_AND2) GateImage = (selected || !correct) ? "Images\\and-gate-h.jpg" : "Images\\and-gate.jpg";
		if (GateType == ITM_AND3) GateImage = (selected || !correct) ? "Images\\and-gate3-h.jpg" : "Images\\and-gate3.jpg";
		if (GateType == ITM_NAND) GateImage = (selected || !correct) ? "Images\\nand-gate-h.jpg" : "Images\\nand-gate.jpg";
		if (GateType == ITM_OR2) GateImage = (selected || !correct) ? "Images\\or-gate-h.jpg" : "Images\\or-gate.jpg";
		if (GateType == ITM_BUFF) GateImage = (selected || !correct) ? "Images\\buffer-gate-h.jpg" : "Images\\buffer-gate.jpg";
		if (GateType == ITM_NOT) GateImage = (selected  || !correct) ? "Images\\not-gate-h.jpg" : "Images\\not-gate.jpg";
		if (GateType == ITM_NOR2) GateImage = (selected  || !correct) ? "Images\\nor-gate-h.jpg" : "Images\\nor-gate.jpg";
		if (GateType == ITM_NOR3) GateImage = (selected  || !correct) ? "Images\\nor-gate3-h.jpg" : "Images\\nor-gate3.jpg";
		if (GateType == ITM_XOR2) GateImage = (selected  || !correct) ? "Images\\xor-gate-h.jpg" : "Images\\xor-gate.jpg";
		if (GateType == ITM_XOR3) GateImage = (selected  || !correct) ? "Images\\xor-gate3-h.jpg" : "Images\\xor-gate3.jpg";
		if (GateType == ITM_XNOR) GateImage = (selected  || !correct) ? "Images\\xnor-gate-h.jpg" : "Images\\xnor-gate.jpg";
		if (GateType == ITM_SWITCH) GateImage = (selected  || !correct) ? "Images\\switch-gate-h.jpg" : "Images\\switch-gate.jpg";
		if (GateType == ITM_LED) GateImage = (selected  || !correct) ? "Images\\led-off-h.jpg" : "Images\\led-off.jpg";
	}

	//Draw gates with a HIGH status
	else {
		if (GateType == ITM_AND2) GateImage = "Images\\and-gate-on.jpg";
		if (GateType == ITM_AND3) GateImage = "Images\\and-gate3-on.jpg";
		if (GateType == ITM_NAND) GateImage = "Images\\nand-gate-on.jpg";
		if (GateType == ITM_OR2) GateImage = "Images\\or-gate-on.jpg";
		if (GateType == ITM_BUFF) GateImage = "Images\\buffer-gate-on.jpg";
		if (GateType == ITM_NOT) GateImage = "Images\\not-gate-on.jpg";
		if (GateType == ITM_NOR2) GateImage = "Images\\nor-gate-on.jpg";
		if (GateType == ITM_NOR3) GateImage = "Images\\nor-gate3-on.jpg";
		if (GateType == ITM_XOR2) GateImage = "Images\\xor-gate-on.jpg";
		if (GateType == ITM_XOR3) GateImage = "Images\\xor-gate3-on.jpg";
		if (GateType == ITM_XNOR) GateImage = "Images\\xnor-gate-on.jpg";
		if (GateType == ITM_SWITCH) GateImage = "Images\\switch-gate-on.jpg";
		if (GateType == ITM_LED) GateImage = "Images\\led-on.jpg";
	}

	//Set the Image Width & Height by AND2 Image Parameter in UI_Info
	pWind->DrawImage(GateImage, r_GfxInfo.x1, r_GfxInfo.y1, width, height);
}

void Output::DrawConnection(GraphicsInfo r_GfxInfo, STATUS status, bool selected) const
{
	if (UI.AppMode == SIMULATION) {
		if (status == HIGH) pWind->SetPen(UI.ActiveColor, 7);
		else pWind->SetPen(UI.ConnColor, 7);
	}
	else {
		if (selected) pWind->SetPen(UI.SelectColor, 7);
		else pWind->SetPen(UI.ConnColor, 7);
	}

	//Identify the type of connection needed between the two gates
	if (r_GfxInfo.x2 < r_GfxInfo.x1) {
		int IntermediateY = (r_GfxInfo.y1 + r_GfxInfo.y2) / 2;
		pWind->DrawLine(r_GfxInfo.x1, r_GfxInfo.y1, r_GfxInfo.x1 + 20, r_GfxInfo.y1);
		pWind->DrawLine(r_GfxInfo.x1 + 20, r_GfxInfo.y1, r_GfxInfo.x1 + 20, IntermediateY);
		pWind->DrawLine(r_GfxInfo.x1 + 20, IntermediateY, r_GfxInfo.x2 - 20, IntermediateY);
		pWind->DrawLine(r_GfxInfo.x2 - 20, IntermediateY, r_GfxInfo.x2 - 20, r_GfxInfo.y2);
		pWind->DrawLine(r_GfxInfo.x2 - 20, r_GfxInfo.y2, r_GfxInfo.x2, r_GfxInfo.y2);
	}
	else {
		int intermediateX = (r_GfxInfo.x1 + r_GfxInfo.x2) / 2;
		pWind->DrawLine(r_GfxInfo.x1, r_GfxInfo.y1, intermediateX, r_GfxInfo.y1);
		pWind->DrawLine(intermediateX, r_GfxInfo.y1, intermediateX, r_GfxInfo.y2);
		pWind->DrawLine(intermediateX, r_GfxInfo.y2, r_GfxInfo.x2, r_GfxInfo.y2);
	}
}

void Output::SetPaste(bool paste) {		//Setting if the paste button should be drawn
	PasteButton = paste;
}


Output::~Output()
{
	delete pWind;
}
