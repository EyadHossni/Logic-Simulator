#include "Paste.h"

Paste::Paste(ApplicationManager* pApp): Action(pApp) {

}

Paste::~Paste() {

}

bool Paste::Execute() {		//simply loading from a Copy.txt made when copying
	pAct = new Load(pManager);
	pAct->LoadByPath("Saved Circuits\\Copy.txt");
	pManager->GetOutput()->PrintMsg("Successfully pasted the copied components");
	return true;
}

void Paste::Undo() {
	pAct->Undo();
	delete pAct;
}

void Paste::Redo() {

}