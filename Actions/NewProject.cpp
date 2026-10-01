#include "NewProject.h"

NewProject::NewProject(Component** Comps, int* count, ApplicationManager* pApp) : Action(pApp) {
	CompList = Comps;
	CompCount = count;
}

NewProject::~NewProject() {
	delete CompList;
}

bool NewProject::Execute() {
	pManager->GetOutput()->PrintMsg("Warning! Starting a new project will lead to losing any unsaved work. Are you sure you want to continue (Y/N)?");
	string choice = pManager->GetInput()->GetSrting(pManager->GetOutput());
	if (choice == "y" || choice == "Y") {
		
		Save* pAct = new Save(CompList, *CompCount, pManager);
		pAct->SavePart("Saved Circuits\\Autosave.txt");		//Backing up the file for undoing later

		for (int i = 0; i < *CompCount; i++) CompList[i] = NULL;
		*CompCount = 0;
		pManager->GetOutput()->ClearDrawingArea();
		pManager->GetOutput()->CreateDesignToolBar();
		pManager->GetOutput()->SetPaste(false);
		pManager->GetInput()->SetPaste(false);
		
		pManager->GetOutput()->PrintMsg("Successfully created a new project. Enjoy!!");
		return true;
	}
	else {
		pManager->GetOutput()->PrintMsg("Cancelled created the new project");
	}
	return false;
}

void NewProject::Undo() {
	Load* pAct = new Load(pManager);
	pAct->LoadByPath("Saved Circuits\\Autosave.txt");
	pManager->GetOutput()->PrintMsg("Successfully recreated the file. Don't lose it again!!");
}

void NewProject::Redo() {

}