#include "ApplicationManager.h"
#include "Actions\AddGate.h"
#include "Actions\AddConnection.h"
#include "Actions\Select.h"
#include "Actions\Operate.h"
#include "Actions\SelectionTools.h"
#include "Actions\Save.h"
#include "Actions\Load.h"
#include "Actions\CreateTruthTable.h"
#include "Actions\NewProject.h"
#include "Actions\Paste.h"

ApplicationManager::ApplicationManager()
{
	CompCount = 0;
	ActionsCount = 0;
	refresh = false;

	for(int i=0; i<MaxCompCount; i++)
		CompList[i] = NULL;

	for (int i = 0; i < MaxUndo; i++)
		ActionList[i] = NULL;

	//Creates the Input / Output Objects & Initialize the GUI
	OutputInterface = new Output();
	InputInterface = OutputInterface->CreateInput();
}
////////////////////////////////////////////////////////////////////
void ApplicationManager::AddComponent(Component* pComp)
{
	CompList[CompCount++] = pComp;
}
//Adding an action to the undo list
void ApplicationManager::AddAction(Action* pAct) {
	if (ActionsCount >= MaxUndo) {
		for (int i = 0; i < ActionsCount; i++) {
			if (i < ActionsCount - 1) ActionList[i] = ActionList[i + 1];
			else ActionList[i] = pAct;
		}
	}
	else {
		ActionList[ActionsCount++] = pAct;
	}
}

//Finding which component exist at any point(x, y)
Component* ApplicationManager::FindComponent(int x, int y) {
	for (int i = 0; i < CompCount; i++) {
		GraphicsInfo G_info = CompList[i]->GetGraphics();

		if (CompList[i]->GetComponentType() == "Gate") {
			if (x >= G_info.x1 && x <= G_info.x2 && y >= G_info.y1 && y <= G_info.y2) {	//Within the gate box
				return CompList[i];
			}
		}
		else {
			if (((Connection*)CompList[i])->GetSmallestDistanceFromPoint(x, y) < 20) {	//withing a small distance from the connection
				return CompList[i];
			}
		}
	}
	return NULL;
}
////////////////////////////////////////////////////////////////////
ActionType ApplicationManager::GetUserAction(int& x, int& y)
{
	//Call input to get what action is reuired from the user
	ActionType Act = InputInterface->GetUserAction(x, y);

	if (Act == Drawing_Area) {
		if (FindComponent(x, y)) return SELECT;
		
	}

	return Act;
}
////////////////////////////////////////////////////////////////////
bool Inside_Square(int x, int y, GraphicsInfo Square) {
	return (x > Square.x1 && x < Square.x2 && y > Square.y1 && y < Square.y2);
}

bool ApplicationManager::Overlappin(GraphicsInfo G_Info) {
	if (G_Info.x1 < UI.ToolBarWidth || G_Info.y2 > UI.height - UI.StatusBarHeight || G_Info.y1 < UI.ModeButtonHeight) return true;
	
	for (int i = 0; i < CompCount; i++) {
		if (!(CompList[i]->GetSelection()) && CompList[i]->GetComponentType() == "Gate") {
			
			//Checking all rectangle sides and its center to check if it touches another gate
			if (Inside_Square(G_Info.x1, G_Info.y1, CompList[i]->GetGraphics())) return true;
			else if (Inside_Square(G_Info.x2, G_Info.y1, CompList[i]->GetGraphics())) return true;
			else if (Inside_Square(G_Info.x1, G_Info.y2, CompList[i]->GetGraphics())) return true;
			else if (Inside_Square(G_Info.x2, G_Info.y2, CompList[i]->GetGraphics())) return true;
			else if (Inside_Square((G_Info.x1 + G_Info.x2) / 2, (G_Info.y1 + G_Info.y2) / 2, CompList[i]->GetGraphics())) return true; 
		
		}
	}
	return false;
}
////////////////////////////////////////////////////////////////////
void ApplicationManager::ExecuteAction(ActionType ActType, int Cx, int Cy)
{
	Action* pAct = NULL;
	if (ActType == SELECT) {
		pAct = new Select(this, CompList, CompCount);
	}
	else if (ActType >= EDIT_Label && ActType <= CUT) {
		pAct = new SelectionTools(ActType, CompList, &CompCount, this);
		refresh = true;
	}
	else {
		if (CompCount > 0) {
			Select* s = new Select(this, CompList, CompCount);
			s->DeselectAll();	//deselecting all gates if a click is made and not related to selection
		}
		
		if (ActType >= ADD_Buff && ActType <= ADD_LED) pAct = new AddGate(ActType, this);

		else if (ActType == SIM_MODE) OutputInterface->CreateSimulationToolBar(); //Create the desgin toolbar

		else if (ActType == DSN_MODE) {
			OutputInterface->ClearStatusBar();	//Create the desgin toolbar
			OutputInterface->CreateDesignToolBar();	//Create the desgin toolbar
			OutputInterface->PrintMsg("Action: Switch to Design Mode, creating Design tool bar");
		}

		else if (ActType == ADD_CONNECTION) pAct = new AddConnection(this);

		else if (ActType == SAVE) pAct = new Save(CompList, CompCount, this);

		else if (ActType == LOAD) pAct = new Load(this);

		else if (ActType == CREATE_NEW_PROJECT) pAct = new NewProject(CompList, &CompCount, this);

		else if (ActType == Create_TruthTable) pAct = new CreateTruthTable(CompList, CompCount, this);

		else if (ActType == PASTE) pAct = new Paste(this);

		else if (ActType == UNDO) {
			if (ActionsCount > 0) {
				ActionList[ActionsCount - 1]->Undo();
				ActionList[ActionsCount-- - 1] = NULL;
			}
		}
		
		else if (ActType == EXIT) OutputInterface->PrintMsg("Ending the program. Thank you for using the program!!");
	}
	if(pAct)
	{
		bool ActionDone;
		ActionDone = pAct->Execute();
		if (ActionDone) {
			AddAction(pAct);
		}
		else {
			delete pAct;
		}
		pAct = NULL;
	}
}
////////////////////////////////////////////////////////////////////

void ApplicationManager::UpdateInterface()
{
	if (UI.AppMode == SIMULATION) (new Operate(CompList, CompCount))->OperateAll();

	for(int i=0; i<CompCount; i++)
		if (CompList[i] != NULL) CompList[i]->Draw(OutputInterface);
	
	Select* s = new Select(this, CompList, CompCount);
	s->DrawToolBar(refresh);
}

void ApplicationManager::OperateAll() {
	Operate* opp = new Operate(CompList, CompCount);
	opp->OperateAll();
}

////////////////////////////////////////////////////////////////////
Input* ApplicationManager::GetInput()
{
	return InputInterface;
}

////////////////////////////////////////////////////////////////////
Output* ApplicationManager::GetOutput()
{
	return OutputInterface;
}

Component** ApplicationManager::GetAllComponents(int*& counter) {
	counter = &CompCount;
	return CompList;
}

////////////////////////////////////////////////////////////////////
ApplicationManager::~ApplicationManager()
{
	for(int i=0; i<CompCount; i++)
		delete CompList[i];
	delete OutputInterface;
	delete InputInterface;
	
}