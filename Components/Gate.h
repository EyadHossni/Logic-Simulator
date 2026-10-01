#ifndef _GATE_H
#define _GATE_H

#include "Component.h"
#include "InputPin.h"
#include "OutputPin.h"

class Gate: public Component
{
protected:
	int GateType;  //Identify what gate this is
	InputPin* m_InputPins;	//Array of input pins of the Gate
	OutputPin m_OutputPin;	//The Gate output pin
	int m_Inputs;		//No. of input pins of that Gate.
	int active_Inputs; //No. of connected pins of the Gate
	bool* connected_Inputs;	//an array of weather every pin is connected or not
	bool correct;	//weather the gate is connected properly from all pins
public:
	Gate(const GraphicsInfo& r_GfxInfo, int gate, int r_FanOut);

	virtual void Operate();	//Calculates the output of the AND gate
	virtual void Draw(Output* pOut);	//Draws 2-input gate

	virtual int GetOutPinStatus();	//returns status of outputpin if LED, return -1
	virtual int GetInputPinStatus(int n);	//returns status of Inputpin # n if SWITCH, return -1

	virtual void setInputPinStatus(int n, STATUS s);	//set status of Inputpin # n, to be used by connection class.

	string GetComponentType();	//returns "Gate"

	OutputPin* GetOutputPin();
	InputPin* GetInputPin(int &InPinNum, bool connect);
	InputPin* GetInputPinByIndex(int& InPinNum);

	int GetInputLocation(InputPin* connection);
	int GetMaxInputs();

	virtual void Select();
	void SetSwitch(STATUS required);

	int GetType();

	void RemoveInput(int n);

	void ConnectPin(int n);

	void Correct();

	bool GetCorrect();

	int CheckCorrection();	//Checking that a gate is connected from all sides correctly
};

#endif