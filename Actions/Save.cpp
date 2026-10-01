#include "Save.h"

int FindComponent(Component* Comps[], Component* find, int count) {
	for (int i = 0; i < count; i++) {
		if (Comps[i] == find) return i;
	}
	return -1;
}

Save::Save(Component* Comp_List[], int count, ApplicationManager* pApp) :Action(pApp) {
	CompCount = count;
	for (int i = 0; i < count; i++) CompList[i] = Comp_List[i];
}

Save::~Save() {
	delete CompList;
}

bool Save::SavePart(string path, bool Copying) {
	ofstream MyFile(path);

	MyFile << CompCount << endl;	//adding the component type

	int ID = 0;

	Component** CompList2 = new Component * [CompCount];

	for (int i = 0; i < CompCount; i++) {
		string type = CompList[i]->GetComponentType();
		if (type == "Gate") {
			CompList2[ID] = CompList[i];
			Gate* gate = (Gate*)(CompList[i]);
			MyFile << UI.GATENAMES[gate->GetType()] << "\t";	//Gate name
			MyFile << ID++ << "\t";								//id
			if (gate->GetLabel() == "") MyFile << "$" "\t";		//label
			else MyFile << gate->GetLabel() << "\t";
			GraphicsInfo Graphics = gate->GetGraphics();

			//if copying so the points are moved slightly to ensure it doesn't get pasted on top of something else
			if (Copying) MyFile << Graphics.x1 + UI.GateLength[gate->GetType()] << "\t" << Graphics.y1 + UI.GateHeight[gate->GetType()] << endl;
			else MyFile << Graphics.x1 << "\t" << Graphics.y1 << endl;
		}
	}
	if (ID > 0) {
		MyFile << "Connections" << endl;

		for (int i = 0; i < CompCount; i++) {
			string type = CompList[i]->GetComponentType();
			//Connections
			if (type == "Connection") {
				Connection* conn = (Connection*)(CompList[i]);
				Gate* source_Gate = conn->getSourcePin()->GetConnectedGate();
				Gate* dest_Gate = (Gate*)(conn->getDestPin()->getComponent());
				if ((source_Gate->GetSelection() && dest_Gate->GetSelection()) || !Copying) {
					MyFile << FindComponent(CompList2, source_Gate, ID) << "\t";
					MyFile << FindComponent(CompList2, dest_Gate, ID) << "\t";
					MyFile << dest_Gate->GetInputLocation(conn->getDestPin()) + 1 << endl;
				}
			}
		}
	}
	else return false;

	MyFile << -1;

	MyFile.close();
	return true;
}

bool Save::Execute() {
	pManager->GetOutput()->PrintMsg("Please Enter the file name for this circuit to save it as");
	string FileName = "Saved Circuits\\" + pManager->GetInput()->GetSrting(pManager->GetOutput()) + ".txt";
	if (FileName != "Saved Circuits\\.txt") {
		SavePart(FileName);

		pManager->GetOutput()->PrintMsg("Successfully saved the file to \"" + FileName + "\"");
		return true;
	}
	else pManager->GetOutput()->PrintMsg("Cancelled saving the file");
	return false;
}

void Save::Undo() {

}

void Save::Redo() {

}
