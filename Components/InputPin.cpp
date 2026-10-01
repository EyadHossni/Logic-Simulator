#include "InputPin.h"

InputPin::InputPin()
{
	pComp = NULL;
}

void InputPin::setComponent(Component*pCmp)
{
	this->pComp = pCmp;
}

Component* InputPin::getComponent()
{
	return pComp;
}