#include "Select.h"

Select::Select(ApplicationManager* pApp, Component** Comps, int n) : Action(pApp) {
	CompCount = n;
	for (int i = 0; i < n; i++) {
		CompList[i] = Comps[i];
	}
}

Select::~Select() {
	delete CompList;
}

void Select::DeselectAll() {
	int counter = 0;
	for (int i = 0; i < CompCount; i++) {
		if (CompList[i]->GetSelection()) {
			(CompList[i])->DeSelect();
			counter++;
		}
		if (CompList[i]->GetComponentType() == "Gate") ((Gate*)CompList[i])->Correct();
	}

	if (counter != 0) {
		pManager->GetOutput()->ClearDrawingArea();
		pManager->UpdateInterface();
		pManager->GetInput()->ClearSelection();
	}
}

bool Select::ReadActionParameters(int &AverageX, int& MinY) {
	AverageX = 0;
	MinY = UI.height;
	int count = 0;
	//getting a box inside which all selected components exist
	for (int i = 0; i < CompCount; i++) {
		Component* gate = CompList[i];
		if (CompList[i]->GetSelection()) {
			count++;
			if (CompList[i]->GetGraphics().y1 < MinY) {
				MinY = CompList[i]->GetGraphics().y1;
			}
			AverageX += (CompList[i]->GetGraphics().x1 + CompList[i]->GetGraphics().x2) / 2;
		}
	}
	if (count != 0) {
		AverageX /= count;
		return true;
	}
	else return false;
}

bool Select::Execute() {
	int Cx = 0, Cy = 0;
	Input* InputInterface = pManager->GetInput();
	InputInterface->GetMousePos(Cx, Cy);
	Component* gate = pManager->FindComponent(Cx, Cy);	//finding components and adding them to an array
	if (gate) gate->Select();
	return true;
}

//Drawing the selection tool bar when something is selected
void Select::DrawToolBar(bool& refresh) {
	int Cx = 0, Cy = 0;
	bool draw_selection = ReadActionParameters(Cx, Cy);
	if (draw_selection || refresh) {
		Input* InputInterface = pManager->GetInput();
		pManager->GetOutput()->ClearDrawingArea();
		
		for (int i = 0; i < CompCount; i++)
			CompList[i]->Draw(pManager->GetOutput());
		
		if (draw_selection) {
			bool DrawLabel = (GetSelectionAmount() == 1);	//checking if the label button should be drawn or not
			int x = pManager->GetOutput()->DrawSelectionToolbar(Cx, Cy - UI.SelectionToolSize - 30, DrawLabel);
			
			InputInterface->setSelection(x, Cy - UI.SelectionToolSize - 30, DrawLabel);
		}
		refresh = false;
	}
}

int Select::GetSelectionAmount() {
	int count = 0;
	for (int i = 0; i < CompCount; i++) {
		if (CompList[i]->GetSelection()) count++;
	}
	return count;
}

void Select::Undo()
{
}

void Select::Redo()
{
}