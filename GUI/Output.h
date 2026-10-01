#pragma once
#include "..\Defs.h"
#include "Input.h"
#include "UI_Info.h"
#include "../Components/Component.h"

class Component;

class Output
{
private:
	window* pWind;	//Pointer to the Graphics Window
	bool PasteButton;	//Boolean that tracks if the paste button should be drawn or not
public:
	Output(); // Performs the Window Initialization
	Input* CreateInput() const; //creates a pointer to the Input object
	void ChangeTitle(string Title) const;

	void CreateDesignToolBar() const;	//Tool bar of the design mode
	void CreateTopToolBar() const;
	void CreateSimulationToolBar() const;//Tool bar of the simulation mode
	void CreateStatusBar() const;	//Create Status bar
	void CreateModeChangeButton() const;	//Draw Button to change from app modes
	void ClearScreen() const;	//Clearing the entire screen

	void ClearStatusBar() const;		//Clears the status bar
	void ClearDrawingArea() const;	//Clears the drawing area

	window* CreateWind(int wd, int h, int x, int y) const; //Creates user interface window

	//Draws all components depending on their type
	void DrawComponent(Component* component, STATUS status = LOW, bool selected = false, bool correct = true) const;

	// Draws Connection
	void DrawConnection(GraphicsInfo r_GfxInfo, STATUS status = LOW, bool selected = false) const;

	void PrintMsg(string msg) const;	//Print a message on Status bar
	void DrawMsg(string msg, GraphicsInfo g_info, int size = 20, bool conn = false) const;	//Draws a string on the screen

	int DrawSelectionToolbar(int x, int y, bool DrawLabel = false);	//Creates the selection toolbar

	void DrawTruthTable(int** Table, int SwitchCount, int LEDCount, int startY, int& Dx, int& Dy);	//Draw the truth table for the circuit

	void SetPaste(bool paste);	//Sets the paste boolean to draw the paste button later
	
	~Output();
};
