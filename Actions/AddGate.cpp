#include "AddGate.h"

AddGate::AddGate(int gate, ApplicationManager* pApp) :Action(pApp)
{
	GateType = gate;
}

AddGate::~AddGate(void)
{
}

bool AddGate::ReadActionParameters()
{
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Print Action Message
	switch (GateType) {
	case ITM_BUFF: pOut->PrintMsg("Buffer Gate: Click to add the gate"); break;
	case ITM_NOT: pOut->PrintMsg("Inverter Gate: Click to add the gate"); break;
	case ITM_AND2: pOut->PrintMsg("2-Input AND Gate: Click to add the gate"); break;
	case ITM_OR2: pOut->PrintMsg("2-Input OR Gate: Click to add the gate"); break;
	case ITM_NAND: pOut->PrintMsg("2-Input NAND Gate: Click to add the gate"); break;
	case ITM_NOR2: pOut->PrintMsg("2-Input NOR Gate: Click to add the gate"); break;
	case ITM_XOR2: pOut->PrintMsg("2-Input XOR Gate: Click to add the gate"); break;
	case ITM_XNOR: pOut->PrintMsg("2-Input XNOR Gate: Click to add the gate"); break;
	case ITM_AND3: pOut->PrintMsg("3-Input AND Gate: Click to add the gate"); break;
	case ITM_NOR3: pOut->PrintMsg("3-Input NOR Gate: Click to add the gate"); break;
	case ITM_XOR3: pOut->PrintMsg("3-Input XOR Gate: Click to add the gate"); break;
	case ITM_SWITCH: pOut->PrintMsg("SWITCH: Click to add a switch"); break;
	case ITM_LED: pOut->PrintMsg("LED: Click to add an LED"); break;
	}

	//Wait for User Input
	ActionType Act = pManager->GetUserAction(Cx, Cy);

	int Len = UI.GateLength[GateType], Width = UI.GateHeight[GateType];

	GraphicsInfo GInfo;

	GInfo.x1 = Cx - Len / 2;
	GInfo.x2 = Cx + Len / 2;
	GInfo.y1 = Cy - Width / 2;
	GInfo.y2 = Cy + Width / 2;
	
	//checking that the user clicked in a valid place on the screen
	bool not_touching_corners = Cx - UI.GateLength[GateType] / 2 > UI.ToolBarWidth;
	if (Act == Drawing_Area && Cx - UI.GateLength[GateType] / 2 > UI.GateHeight[GateType] && not_touching_corners && !(pManager->Overlappin(GInfo))) {
		return true;
	}
	else {
		pOut->PrintMsg("Failed to place gate in this location.");
		return false;
	}
}

bool AddGate::Execute()
{
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	//Get Center point of the Gate
	if (ReadActionParameters()) {

		//Calculate the rectangle Corners
		int Len = UI.GateLength[GateType], Width = UI.GateHeight[GateType];

		GraphicsInfo GInfo; //Gfx info to be used to construct the AND2 gate

		GInfo.x1 = Cx - Len / 2;
		GInfo.x2 = Cx + Len / 2;
		GInfo.y1 = Cy - Width / 2;
		GInfo.y2 = Cy + Width / 2;

		Gate* pA = new Gate(GInfo, GateType, 5);
		pManager->AddComponent(pA);

		pOut->PrintMsg("Successfully Added the gate!");
		return true;
	}
	return false;
}

void AddGate::Undo()
{
	pManager->FindComponent(Cx, Cy)->Select();
	int* CompCount = new int;
	Component** CompList = pManager->GetAllComponents(CompCount);
	SelectionTools* pAct = new SelectionTools(DEL, CompList, CompCount, pManager);
	pAct->Execute();

	bool True = true;
	Select* s = new Select(pManager, CompList, *CompCount);
	s->DrawToolBar(True);

	pManager->GetOutput()->PrintMsg("Gate placement undone successfully");
}

void AddGate::Redo()
{
}

