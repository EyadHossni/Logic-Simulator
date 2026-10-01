#ifndef _COMPONENT_H
#define _COMPONENT_H

#include "..\Defs.h"
#include "..\GUI\Output.h"

//Base class for classes Gate, Switch, and LED.
class Component
{
protected:
	GraphicsInfo m_GfxInfo;	//The parameters required to draw a component
	bool selected;	//weather a gate is selected or not
	string m_Label;	//Gate label
public:
	Component(const GraphicsInfo &r_GfxInfo);
	virtual void Operate() = 0;	//Calculates the output according to the inputs
	virtual void Draw(Output* pOut) = 0;	//for each component to Draw itself
	
	
	virtual int GetOutPinStatus()=0;	//returns status of outputpin if LED, return -1
	virtual int GetInputPinStatus(int n)=0;	//returns status of Inputpin # n if SWITCH, return -1
	virtual string GetComponentType() = 0;	//Getting the type of the compoennt (Gate - Connection)
	
	virtual void Select() = 0;	//select function
	void DeSelect();	//Deselecting a gate

	virtual void setInputPinStatus(int n, STATUS s)=0;	//set status of Inputpin # n, to be used by connection class.

	GraphicsInfo GetGraphics() const { return m_GfxInfo; }	//getting a gate graphics

	void Move(int Dx, int Dy, bool first, bool last);	//moving a gate by an x and y and knowing which part to move

	void SetLabel(string);	//setting label
	string GetLabel();	//getting label

	bool GetSelection();	//getting weather it's selected or not
	
	//Destructor must be virtual
	virtual ~Component();
};

#endif
