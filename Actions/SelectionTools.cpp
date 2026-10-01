#include "SelectionTools.h"
#include "../Components/Gate.h"
#include "../Components/Connection.h"

//removing a gate from all components without leaving spaces
void remove(Component* Array[], Component* Part, int &size) {
	bool replace = false;
	for (int i = 0; i < size; i++) {
		if (replace) Array[i] = Array[i + 1];
		else if (Array[i] == Part) {
			Array[i] = Array[i + 1];
			replace = true;
		}
	}
	size--;
}

//getting the middle point of a box holding different slected components
void GetBoxMiddle(Component* CompList[], int size, int& x, int& y) {
	int sumX = 0, sumY = 0, counter = 0;
	for (int i = 0; i < size; i++) {
		if (CompList[i]->GetComponentType() == "Gate") {
			GraphicsInfo graphics = CompList[i]->GetGraphics();
			sumX += (graphics.x1 + graphics.x2) / 2;
			sumY += (graphics.y1 + graphics.y2) / 2;
			counter++;
		}
	}
	if (counter > 0) {
		x = sumX / counter; y = sumY / counter;
	}
}

SelectionTools::SelectionTools(ActionType act, Component** Comps, int* size, ApplicationManager* pApp) :Action(pApp) {
	ActType = act;
	CompList = Comps;
	CompCount = size;
	SelectedCount = 0;
	SelectedComps = new Component * [*CompCount];
	for (int i = 0; i < *CompCount; i++) {
		SelectedComps[i] = NULL;
	}
}

SelectionTools::~SelectionTools() {
	delete CompList;
	delete SelectedComps;
}

//Collecting all selected components in an array
bool SelectionTools::ReadActionParameters() {
	SelectedCount = 0;
	for (int i = 0; i < *CompCount; i++) {
		if (CompList[i]->GetComponentType() == "Gate" && CompList[i]->GetSelection()) SelectedComps[SelectedCount++] = CompList[i];
		else if (CompList[i]->GetComponentType() == "Connection") {
			Connection* conn = (Connection*)CompList[i];
			Component* destGate = conn->getDestPin()->getComponent();
			Component* sourceGate = conn->getSourcePin()->GetConnectedGate();
			if (destGate->GetSelection() || sourceGate->GetSelection() || conn->GetSelection()) {
				SelectedComps[SelectedCount++] = CompList[i];
			}
		}
	}
	return true;
}

bool SelectionTools::Execute() {
	ReadActionParameters();
	switch (ActType) {
	case DEL:
		Delete();
		return true;
		break;
	case MOVE:
		return Move();
		break;
	case EDIT_Label:
		return EditLabel();
		break;
	case COPY:
		Copy();
		return true;
		break;
	case CUT:
		Copy();
		Delete();
		pManager->GetOutput()->PrintMsg("Successfully cut the selected components");
		return true;
		break;
	}
}

void SelectionTools::Delete() {
	for (int i = 0; i < SelectedCount; i++) {
		if (SelectedComps[i]->GetComponentType() == "Connection"){
			Connection* conn = (Connection*)SelectedComps[i];
			Gate* destGate = (Gate*)(conn->getDestPin()->getComponent());
			Gate* sourceGate = (Gate*)(conn->getSourcePin()->GetConnectedGate());

			destGate->RemoveInput(conn->GetInputPinCount());

			remove(CompList, conn, *CompCount);
		}
		else remove(CompList, SelectedComps[i], *CompCount);
	}
	pManager->GetOutput()->PrintMsg("Deleted items successfully");
}

bool SelectionTools::Move(int x, int y) {

	Output* pOut = pManager->GetOutput();
	Input* pIn = pManager->GetInput();
	pOut->PrintMsg("Please choose a new point for your selection");
	int AnchorX = 0, AnchorY = 0;
	GetBoxMiddle(SelectedComps, SelectedCount, AnchorX, AnchorY);

	int NewX = 0, NewY = 0;
	ActionType click = pIn->GetUserAction(NewX, NewY);
	distanceMovedX = NewX - AnchorX; distanceMovedY = NewY - AnchorY;
	
	//checking that the motion won't move inside of another gate
	bool allow_move = true;
	for (int i = 0; i < SelectedCount; i++) {
		if (SelectedComps[i]->GetComponentType() == "Gate") {
			GraphicsInfo Info = SelectedComps[i]->GetGraphics();
			Info.x1 += distanceMovedX;
			Info.x2 += distanceMovedX;
			Info.y1 += distanceMovedY;
			Info.y2 += distanceMovedY;

			if (pManager->Overlappin(Info)) {
				allow_move = false;
				break;
			}
		}
	}

	if (click <= Drawing_Area && click >= EDIT_Label && allow_move) {

		for (int i = 0; i < SelectedCount; i++) {
			Component* comp = SelectedComps[i];
			
			if (comp->GetComponentType() == "Gate") comp->Move(distanceMovedX, distanceMovedY, true, true);
			
			else {
				Connection* conn = (Connection*)comp;
				Component* destGate = conn->getDestPin()->getComponent();
				Component* sourceGate = conn->getSourcePin()->GetConnectedGate();
				comp->Move(distanceMovedX, distanceMovedY, sourceGate->GetSelection(), destGate->GetSelection());
			}
		}
		pOut->PrintMsg("Successfully moved the components");
		return true;
	}
	else pOut->PrintMsg("Unable to move the point to this location");
	return false;
}

bool SelectionTools::EditLabel(bool undo) {
	string label;
	bool allow_passage = false;
	Output* pOut = pManager->GetOutput();
	if (!undo) {
		pOut->PrintMsg("Please enter the new label you want for the gate");
		label = pManager->GetInput()->GetSrting(pOut);
	}
	else {
		label = past_label; allow_passage = true;
	}

	if (label != "" || allow_passage) {
		if (SelectedComps[0]) {
			past_label = SelectedComps[0]->GetLabel();
			SelectedComps[0]->SetLabel(label);
		}
		if (undo) pOut->PrintMsg("Successfully remade the old label");
		else pOut->PrintMsg("Successfully Added the label");
		return true;
	}
	pOut->PrintMsg("Adding label cancelled");
	return false;
}

bool SelectionTools::Copy() {
	Output* pOut = pManager->GetOutput();
	if (SelectedCount > 0) {
		Save* pAct = new Save(SelectedComps, SelectedCount, pManager);
		bool ability = pAct->SavePart("Saved Circuits\\Copy.txt", true);
		if (ability) {
			pOut->PrintMsg("Successfully copied the components");
			pOut->SetPaste(true);
			pManager->GetInput()->SetPaste(true);

			for (int i = 0; i < SelectedCount; i++) {
				SelectedComps[i]->DeSelect();
			}
			return true;
		}
	}
	pOut->PrintMsg("Unable to copy given components");
	return false;
}

void SelectionTools::Undo()
{
	switch (ActType) {
	case DEL:
		for (int i = 0; i < SelectedCount; i++) {
			SelectedComps[i]->DeSelect();
			pManager->AddComponent(SelectedComps[i]);
		}
		break;
	case MOVE:
		for (int i = 0; i < SelectedCount; i++) {
			Component* comp = SelectedComps[i];

			if (comp->GetComponentType() == "Gate") {
				comp->Move(-distanceMovedX, -distanceMovedY, true, true);
				comp->Select();
			}

			else {
				Connection* conn = (Connection*)comp;
				Component* destGate = conn->getDestPin()->getComponent();
				Component* sourceGate = conn->getSourcePin()->GetConnectedGate();
				comp->Move(-distanceMovedX, -distanceMovedY, sourceGate->GetSelection(), destGate->GetSelection());
			}
		}

		for (int i = 0; i < SelectedCount; i++) {
			SelectedComps[i]->DeSelect();
		}

		pManager->GetOutput()->PrintMsg("Successfully moved the components back to their locations");
		break;
	case EDIT_Label:
		EditLabel(true);
		break;
	case COPY:
		break;
	case CUT:
		Paste* pAct = new Paste(pManager);
		pAct->Execute();
		break;
	}

	bool True = true;
	(new Select(pManager, CompList, 0))->DrawToolBar(True);
}

void SelectionTools::Redo()
{
}