#include "Load.h"

//Getting part of string by index with the tab-spaces between them
string GetItem(string sentence, int index) {
	int counter = 0;
	int length = sentence.length();
	string word;
	for (int i = 0; i < length; i++) {
		if (counter == index && sentence[i] != '\t') {
			word += sentence[i];
		}
		if (sentence[i] == '\t' || i == length - 1) {
			counter++;
			if (counter > index) return word;
		}
	}
	return "";
}

//finding a string in an array of strings
int StringFind(string gatenames[], int size, string s) {
	for (int i = 0; i < size; i++) {
		if (gatenames[i] == s) return i;
	}
	return -1;
}

Load::Load(ApplicationManager* pApp): Action(pApp) {

}

Load::~Load(){
	delete CompList;
}

string Load::ReadActionParameters() {
	pManager->GetOutput()->PrintMsg("Please enter the name of the file you want to import");
	string filename = "Saved Circuits\\" + pManager->GetInput()->GetSrting(pManager->GetOutput()) + ".txt";

	ifstream ReadFile(filename);
	if (ReadFile.good()) {
		ReadFile.close();
		return filename;
	}
	ReadFile.close();
	pManager->GetOutput()->PrintMsg("Error! Unable to find the file \"" + filename + "\". Please check the file and try again");
	return "";
}

bool Load::Execute() {
	string FileName = ReadActionParameters();
	if (FileName != "") {
		LoadByPath(FileName);
		pManager->GetOutput()->PrintMsg("Successfully Loaded the required text file");
		return true;
	}
	return false;
}

void Load::LoadByPath(string path) {
	ifstream ReadFile(path);
	string LineText;
	bool ReachConnection = false;

	getline(ReadFile, LineText);
	CompCount = stoi(LineText);

	CompList = new Gate * [CompCount];	//array depending on component amount

	int counter = 0;

	while (getline(ReadFile, LineText)) {
		pManager->UpdateInterface();
		if (LineText == "Connections") {
			ReachConnection = true;			//checking if the gates are done and begin importing connections
		}
		else if (LineText == "-1") break;	//End of file
		
		//[1] Gates
		else if (!ReachConnection) {
			int gateType = StringFind(UI.GATENAMES, ITM_DSN_CNT, GetItem(LineText, 0));
			string Label = GetItem(LineText, 2);

			GraphicsInfo Graphics;
			Graphics.x1 = stoi(GetItem(LineText, 3));
			Graphics.y1 = stoi(GetItem(LineText, 4));
			Graphics.x2 = Graphics.x1 + UI.GateLength[gateType];
			Graphics.y2 = Graphics.y1 + UI.GateHeight[gateType];

			Gate* pA = new Gate(Graphics, gateType, 5);

			if (Label != "$") pA->SetLabel(Label);

			pManager->AddComponent(pA);
			CompList[counter++] = pA;		//saving them so we can check connections between them
		}

		//[2] connections
		else {
			Gate* StartGate = CompList[stoi(GetItem(LineText, 0))];
			Gate* DestinationGate = CompList[stoi(GetItem(LineText, 1))];
			int InputNum = stoi(GetItem(LineText, 2)) - 1;
			
			(new AddConnection(pManager))->CreateConnection(StartGate, DestinationGate, InputNum);
			DestinationGate->ConnectPin(InputNum);
		}
	}
	CompCount = counter;
	ReadFile.close();
}

void Load::Undo() {
	for (int i = 0; i < CompCount; i++) {
		if (CompList[i]) CompList[i]->Select();
	}

	int* CompCount2 = new int;
	Component** CompList2 = pManager->GetAllComponents(CompCount2);
	SelectionTools* pAct = new SelectionTools(DEL, CompList2, CompCount2, pManager);
	pAct->Execute();

	bool True = true;
	Select* s = new Select(pManager, CompList2, *CompCount2);
	s->DrawToolBar(True);

	pManager->GetOutput()->PrintMsg("Deleted the added items");
}

void Load::Redo() {

}