#include "Component.h"

Component::Component(const GraphicsInfo &r_GfxInfo)
{
	m_GfxInfo = r_GfxInfo;
	selected = false;
	m_Label = "";
}
//Setting the label for a gate
void Component::SetLabel(string word) {
	m_Label = word;
}
//Getting gate label
string Component::GetLabel() {
	return m_Label;
}
//Getting weather a gate is selected or not
bool Component::GetSelection() {
	return selected;
}
//Deselecting a gate
void Component::DeSelect() {
	selected = false;
}
//moving a gate depending on which part of it is to move (For Connections) and moves whole for gates
void Component::Move(int Dx, int Dy, bool first, bool last) {
	if (first) {
		m_GfxInfo.x1 += Dx;
		m_GfxInfo.y1 += Dy;
	}
	if (last) {
		m_GfxInfo.x2 += Dx;
		m_GfxInfo.y2 += Dy;
	}
}

Component::~Component()
{}

