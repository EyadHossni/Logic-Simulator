#include "Gate.h"


Gate::Gate(const GraphicsInfo& r_GfxInfo, int gate, int r_FanOut): m_OutputPin(this, r_FanOut), Component(r_GfxInfo)
{
	GateType = gate;

	//Identifying the amount of inputs depending on the gateType
	int r_Inputs;
	if (GateType == ITM_AND3 || GateType == ITM_NOR3 || GateType == ITM_XOR3) r_Inputs = 3;
	else if (GateType == ITM_SWITCH) r_Inputs = 0;
	else if (GateType == ITM_BUFF || GateType == ITM_NOT || GateType == ITM_LED) r_Inputs = 1;
	else r_Inputs = 2;

	//Allocate number of input pins (equals r_Inputs)
	m_Inputs = r_Inputs;	//set no. of inputs of that gate
	m_InputPins = new InputPin[r_Inputs];
	connected_Inputs = new bool[r_Inputs];
	
	//Associate all input pins to this gate
	for (int i = 0; i < r_Inputs; i++) {
		m_InputPins[i].setComponent(this);
		connected_Inputs[i] = false;
	}
	active_Inputs = 0;
	selected = false;
	correct = true;
}

//Calculating the gate output depending on its inputs
void Gate::Operate()
{
	int outstatus = m_OutputPin.getStatus();
	switch (GateType) {

	case ITM_BUFF:
		outstatus = m_InputPins[0].getStatus();
		break;

	case ITM_LED:
		outstatus = m_InputPins[0].getStatus();
		break;

	case ITM_NOT:
		outstatus = (m_InputPins[0].getStatus() == LOW) ? HIGH : LOW;
		break;

	case ITM_AND2:
		outstatus = HIGH;
		for (int i = 0; i < active_Inputs; i++) {
			outstatus *= m_InputPins[i].getStatus();
		}
		break;
	case ITM_AND3:
		outstatus = HIGH;
		for (int i = 0; i < active_Inputs; i++) {
			outstatus *= m_InputPins[i].getStatus();
		}
		break;

	case ITM_OR2:
		outstatus = LOW;
		for (int i = 0; i < active_Inputs; i++) {
			outstatus += m_InputPins[i].getStatus();
			if (outstatus > 1) outstatus = 1;
		}
		break;

	case ITM_NAND:
		outstatus = HIGH;
		for (int i = 0; i < active_Inputs; i++) {
			outstatus *= m_InputPins[i].getStatus();
		}
		outstatus = (outstatus == LOW) ? HIGH : LOW;
		break;

	case ITM_NOR2:
		outstatus = LOW;
		for (int i = 0; i < active_Inputs; i++) {
			outstatus += m_InputPins[i].getStatus();
			if (outstatus > 1) outstatus = 1;
		}
		outstatus = (outstatus == LOW) ? HIGH : LOW;
		break;
	case ITM_NOR3:
		outstatus = LOW;
		for (int i = 0; i < active_Inputs; i++) {
			outstatus += m_InputPins[i].getStatus();
			if (outstatus > 1) outstatus = 1;
		}
		outstatus = (outstatus == LOW) ? HIGH : LOW;
		break;

	case ITM_XOR2:
		if (m_InputPins[0].getStatus() != m_InputPins[1].getStatus()) outstatus = HIGH;
		else outstatus = LOW;
		break;
	case ITM_XOR3:
		if (m_InputPins[0].getStatus() != m_InputPins[1].getStatus()) outstatus = HIGH;
		else outstatus = LOW;

		if (outstatus != m_InputPins[2].getStatus()) outstatus = HIGH;
		else outstatus = LOW;
		break;

	case ITM_XNOR:
		if (m_InputPins[0].getStatus() != m_InputPins[1].getStatus()) outstatus = HIGH;
		else outstatus = LOW;
		outstatus = (outstatus == LOW) ? HIGH : LOW;
		break;
	}
	m_OutputPin.setStatus((STATUS)outstatus);
}

//Checking that a gate is connected from all sides correctly
int Gate::CheckCorrection() {
	if (active_Inputs != m_Inputs || (m_OutputPin.GetConnectionsCount() == 0 && GateType != ITM_LED)) {
		correct = false;
		return false;
	}
	else {
		correct = true;
		return true;
	}
}


// Function Draw
void Gate::Draw(Output* pOut)
{
	//Call output class and pass gate drawing info to it.
	pOut->DrawComponent(this, (STATUS)GetOutPinStatus(), selected, correct);
	if (m_Label != "") pOut->DrawMsg(m_Label, m_GfxInfo);
}

//returns status of outputpin
int Gate::GetOutPinStatus()
{
	return m_OutputPin.getStatus();
}


//returns status of Inputpin #n
int Gate::GetInputPinStatus(int n)
{
	return m_InputPins[n - 1].getStatus();	//n starts from 1 but array index starts from 0.
}

//Set status of an input pin ot HIGH or LOW
void Gate::setInputPinStatus(int n, STATUS s)
{
	m_InputPins[n - 1].setStatus(s);
}

OutputPin* Gate::GetOutputPin() { return &m_OutputPin; }

//Getting the free inputPin
InputPin* Gate::GetInputPin(int& InPinNum, bool connect) {
	for (int i = 0; i < m_Inputs; i++) {
		if (!connected_Inputs[i]) {
			if (connect) {
				InPinNum = i;
				connected_Inputs[i] = true;
				active_Inputs++;
			}
			return &(m_InputPins[i]);
		}
	}
	return nullptr;

}
//Getting an inputPin by index
InputPin* Gate::GetInputPinByIndex(int& InPinNum) {
	return &(m_InputPins[InPinNum]);
}
//Getting an input location on a gate
int Gate::GetInputLocation(InputPin* connection) {
	for (int i = 0; i < m_Inputs; i++) {
		if (&(m_InputPins[i]) == connection) {
			return i;
		}
	}
	return -1;
}

int Gate::GetMaxInputs() {
	return m_Inputs;
}

string Gate::GetComponentType() {
	return "Gate";
}
//selecting a gate and changing the output of a switch
void Gate::Select() {
	if (UI.AppMode == DESIGN) selected = true;
	else {
		if (GateType == ITM_SWITCH) {
			STATUS status = GetOutputPin()->getStatus();
			m_OutputPin.setStatus((status == LOW) ? HIGH : LOW);
		}
	}
}
//setting a switch to something exact
void Gate::SetSwitch(STATUS required) {
	m_OutputPin.setStatus(required);
}

int Gate::GetType() {
	return GateType;
}

void Gate::RemoveInput(int n) {
	connected_Inputs[n] = false;
	if (active_Inputs > 0) active_Inputs--;
}

void Gate::ConnectPin(int n) {
	connected_Inputs[n] = true;
	active_Inputs++;
}

void Gate::Correct() {
	correct = true;
}

bool Gate::GetCorrect() {
	return correct;
}