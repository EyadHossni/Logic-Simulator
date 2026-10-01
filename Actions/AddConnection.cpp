#include "AddConnection.h"

AddConnection::AddConnection(ApplicationManager* pApp): Action(pApp)
{
}
AddConnection::~AddConnection(void)
{
	delete pA;
}

bool AddConnection::Execute() {
	//Get a Pointer to the Input / Output Interfaces
	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();

	pOut->PrintMsg("Connection: Choose the input component");
	ActionType Act = pManager->GetUserAction(Cx, Cy);

	if (Act == SELECT) {	//check that the user clicked on a valid component
		Gate* comp = (Gate*)pManager->FindComponent(Cx, Cy);
		OutputPin* OutPin = comp->GetOutputPin();
		GraphicsInfo Info = comp->GetGraphics();

		GraphicsInfo GInfo;
		GInfo.x1 = Info.x2 - 5;
		GInfo.y1 = pManager->GetInput()->GetConnectionY(Info, 1, false);

		pOut->PrintMsg("Connection: Choose the Output component");
		ActionType Act = pManager->GetUserAction(Cx, Cy);

		if (Act == SELECT) {	//check that the user clicked on a valid component
			comp = (Gate*)pManager->FindComponent(Cx, Cy);
			int PinNum = 0;
			InputPin* InPin = comp->GetInputPin(PinNum, true);
			if (PinNum < comp->GetMaxInputs()) {
				Info = comp->GetGraphics();

				GInfo.x2 = Info.x1 + 5;
				GInfo.y2 = pManager->GetInput()->GetConnectionY(Info, PinNum + 1, true);

				pA = new Connection(GInfo, OutPin, InPin, PinNum);
				pManager->AddComponent(pA);

				OutPin->ConnectTo(pA);
				pOut->PrintMsg("Connection added successfully");
				return true;
			}
			else {
				pOut->PrintMsg("Unable to add connection (Maximum number reached)");
			}
		}
		else {
			pOut->PrintMsg("Adding connection cancelled (No output component found)");
		}
	}
	else {
		pOut->PrintMsg("Adding connection cancelled (No input component found)");
	}
	return false;
}
//funtion to add a connection between two gates without the need for user actions
void AddConnection::CreateConnection(Gate* gate1, Gate* gate2, int InputLocation) {
	GraphicsInfo info;
	GraphicsInfo gate1_GInfo = gate1->GetGraphics();
	GraphicsInfo gate2_GInfo = gate2->GetGraphics();

	info.x1 = gate1_GInfo.x2 - 5;
	info.y1 = pManager->GetInput()->GetConnectionY(gate1_GInfo, 1, false);
	info.x2 = gate2_GInfo.x1 + 5;
	info.y2 = pManager->GetInput()->GetConnectionY(gate2_GInfo, InputLocation + 1, true);

	OutputPin* OutPin = gate1->GetOutputPin();
	InputPin* InPin = gate2->GetInputPinByIndex(InputLocation);

	pA = new Connection(info, OutPin, InPin, InputLocation);
	pManager->AddComponent(pA);

	OutPin->ConnectTo(pA);
}

void AddConnection::Undo()
{
	pA->Select();
	int* CompCount = new int;
	Component** CompList = pManager->GetAllComponents(CompCount);
	SelectionTools* pAct = new SelectionTools(DEL, CompList, CompCount, pManager);
	pAct->Execute();
	bool True = true;
	Select* s = new Select(pManager, CompList, *CompCount);
	s->DrawToolBar(True);

	pManager->GetOutput()->PrintMsg("Connection placement undone successfully");
}

void AddConnection::Redo()
{
}