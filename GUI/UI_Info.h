#ifndef UI_INFO_H
#define UI_INFO_H

#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\Defs.h"

//User Interface information file.
//This file contains info that is needed by Input and Output classes to
//handle the user interface

__declspec(selectany) //This line to prevent "redefinition error"

struct UI_Info	//User Interface Info.
{
	MODE AppMode = DESIGN;		//Application Mode (design or simulation)

	int	width = 192 * 7, height = 108 * 7,	//Window width and height
		wx = 15, wy = 15,			//Window starting coordinates
		StatusBarHeight = 50,	//Status Bar Height
		ToolBarWidth = width / 6,		//Width of each item in toolbar menu
		ToolItemWidth = ToolBarWidth / 1.25,
		ToolItemHeight = 43,
		ToolStartX = (ToolBarWidth - ToolItemWidth) / 2,
		ModeButtonX = width - 180,
		ModeButtonY = 10,
		ModeButtonWidth = 160,
		TopMenuButtonWidth = 100,
		ModeButtonHeight = 40,
		GateLength[13] = { 128, 150, 160, 132, 156, 150, 132, 150, 160, 132, 132, 105, 100 },
		GateHeight[13] = { 92 , 84, 80, 78, 72, 78, 78, 78, 80, 78, 78, 73, 100 },
		SelectionToolSize = 40;

	double drawing_factor = 1;
	
	color SelectColor = color(202, 84, 84),
		ConnColor = color(64, 64, 115),
		MsgColor = color(107, 114, 128),
		BkGrndColor = color(249, 250, 251),
		EdgesPen = color(238, 239, 242),
		ActiveColor = color(125, 208, 144);

	string GATENAMES[ITM_DSN_CNT + ITM_TOP_MENU_CNT] = { "BUFFER","NOT", "AND2", "OR", "NAND", "NOR2", "XOR2", "XNOR", "AND3", "NOR3", "XOR3", "SWITCH", "LED", "CONNECT", "EXIT", "NEW", "UNDO", "SAVE", "LOAD", "PASTE"};
} UI;	//create a single global object UI

#endif