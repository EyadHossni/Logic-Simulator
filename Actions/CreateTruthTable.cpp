#include "CreateTruthTable.h"

//finding all gates of a specific type and collecting them to an array
int FindAndFill(Component* AllComps[], int size, int GateType, Component** CounterList) {
	int counter = 0;
	for (int i = 0; i < size; i++) {
		if (AllComps[i]->GetComponentType() == "Gate") {
			Gate* gate = (Gate*)AllComps[i];
			if (gate->GetType() == GateType) {
				CounterList[counter++] = AllComps[i];
			}
		}
	}
	return counter;
}

CreateTruthTable::CreateTruthTable(Component** Comps, int size, ApplicationManager* pApp) : Action(pApp) {
	//Collecting the required components to arrays
	Switchs = new Component * [size];
	LEDs = new Component * [size];
	SwitchCount = FindAndFill(Comps, size, ITM_SWITCH, Switchs);
	LEDCount = FindAndFill(Comps, size, ITM_LED, LEDs);
	
	//ensuring every gate is fully connected
	circuit_ready = true;
	for (int i = 0; i < size; i++) {
		if (Comps[i]->GetComponentType() == "Gate") {
			Gate* gate = (Gate*)Comps[i];
			gate->CheckCorrection();
			if (!(gate->GetCorrect())) {
				circuit_ready = false;
				break;
			}
		}
	}
}

bool CreateTruthTable::Execute() {
	if (LEDCount != 0 && SwitchCount != 0 && circuit_ready) {
		//creating the table for all the possibilities
		int width = SwitchCount + LEDCount;
		int height = pow(2, SwitchCount);
		int** Table = new int* [width];
		for (int i = 0; i < width; i++) {
			Table[i] = new int[height];
		}

		//adding all the possible combinations for the switches
		for (int i = 0; i < SwitchCount; i++) {
			int zeroswitcher = pow(2, SwitchCount - i - 1);
			int amountplaced = 0;
			int place = 0;
			for (int k = 0; k < height; k++) {
				Table[i][k] = place;
				amountplaced++;
				if (amountplaced % zeroswitcher == 0) {
					place = (place == 0) ? 1 : 0;
					amountplaced = 0;
				}
			}
		}

		for (int i = 0; i < SwitchCount; i++) {
			Switchs[i]->SetLabel("");
			Switchs[i]->SetLabel("Sw" + to_string(i + 1));
		}
		for (int i = 0; i < LEDCount; i++) {
			LEDs[i]->SetLabel("LED" + to_string(i + 1));
		}

		//Setting the switches and operating to check the output
		for (int i = 0; i < height; i++) {
			for (int k = 0; k < SwitchCount; k++) {
				((Gate*)(Switchs[k]))->SetSwitch((STATUS)Table[k][i]);
			}
			pManager->UpdateInterface();

			for (int k = 0; k < LEDCount; k++) {
				Table[k + SwitchCount][i] = LEDs[k]->GetOutPinStatus();
			}
		}

		//Drawing the truth table from the table we made
		int Dx = 0, Dy = 0, startY = UI.ModeButtonY + UI.ModeButtonHeight + 10;
		Output* pOut = pManager->GetOutput();
		pOut->DrawTruthTable(Table, SwitchCount, LEDCount, startY, Dx, Dy);

		//Returning the circuit to a neutral state
		for (int k = 0; k < SwitchCount; k++) {
			((Gate*)(Switchs[k]))->SetSwitch(LOW);
		}

		//Freeing the memory used
		for (int i = 0; i < width; i++) {
			delete[] Table[i];
		}
		delete[] Table;

		pManager->GetOutput()->PrintMsg("Successfully made the truth table");
		return true;
	}
	else pManager->GetOutput()->PrintMsg("Unable to create Truth table for this circuit");
	return false;
}

void CreateTruthTable::Undo() {

}

void CreateTruthTable::Redo() {

}

CreateTruthTable::~CreateTruthTable() {
	delete[] Switchs;
	delete[] LEDs;
}