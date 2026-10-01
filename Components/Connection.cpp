#include "Connection.h"

//Getting the perpendicular distance between a point and a connection
double perpendicularDistance(int Cx, int Cy, int x1, int y1, int x2, int y2)
{
	if (x1 == x2) {
		int BigY = (y1 > y2) ? y1 : y2;
		int SmallY = (y1 < y2) ? y1 : y2;
		if (Cy > SmallY && Cy < BigY) return abs(Cx - x1);
	}
	else if (y1 == y2) {
		int BigX = (x1 > x2) ? x1 : x2;
		int SmallX = (x1 < x2) ? x1 : x2;
		if (Cx > SmallX && Cx < BigX) return abs(Cy - y1);
	}
	return UI.width;
}


Connection::Connection(const GraphicsInfo &r_GfxInfo, OutputPin *pSrcPin,InputPin *pDstPin, int count):Component(r_GfxInfo)	
	
{
	SrcPin = pSrcPin;
	DstPin = pDstPin;
	Inputcount = count;
}
void Connection::setSourcePin(OutputPin *pSrcPin)
{	SrcPin = pSrcPin;	}

OutputPin* Connection::getSourcePin()
{	return SrcPin;	}


void Connection::setDestPin(InputPin *pDstPin)
{	DstPin = pDstPin;	}

string Connection::GetComponentType() {
	return "Connection";
}

InputPin* Connection::getDestPin()
{	return DstPin;	}


void Connection::Operate()
{
	//Status of connection destination pin = status of connection source pin
	DstPin->setStatus((STATUS)SrcPin->getStatus());
}

void Connection::Draw(Output* pOut)
{
	pOut->DrawConnection(m_GfxInfo, (STATUS)SrcPin->getStatus(), selected);
	if (m_Label != "") pOut->DrawMsg(m_Label, m_GfxInfo, 20, true);
}

int Connection::GetOutPinStatus()	//returns status of outputpin if LED, return -1
{
	return DstPin->getStatus();
}


int Connection::GetInputPinStatus(int n)	//returns status of Inputpin # n if SWITCH, return -1
{
	return SrcPin->getStatus();	//n is ignored as connection has only one input pin (src pin)	
}

void Connection::setInputPinStatus(int n, STATUS s)
{
	SrcPin->setStatus(s);
}

void Connection::Select() {
	selected = true;
}

int Connection::GetInputPinCount() {
	return Inputcount;
}

//getting the smallest distance between any point and all lines in connection
double Connection::GetSmallestDistanceFromPoint(int x, int y) {
	double* distances;
	int distancescount;

	//Identifying the connection type from the points and measuring all distances
	if (m_GfxInfo.x2 < m_GfxInfo.x1) {
		int IntermediateY = (m_GfxInfo.y1 + m_GfxInfo.y2) / 2;
		distancescount = 5;
		distances = new double[distancescount];
		distances[0] = perpendicularDistance(x, y, m_GfxInfo.x1, m_GfxInfo.y1, m_GfxInfo.x1 + 20, m_GfxInfo.y1);
		distances[1] = perpendicularDistance(x, y, m_GfxInfo.x1 + 20, m_GfxInfo.y1, m_GfxInfo.x1 + 20, IntermediateY);
		distances[2] = perpendicularDistance(x, y, m_GfxInfo.x1 + 20, IntermediateY, m_GfxInfo.x2 - 20, IntermediateY);
		distances[3] = perpendicularDistance(x, y, m_GfxInfo.x2 - 20, IntermediateY, m_GfxInfo.x2 - 20, m_GfxInfo.y2);
		distances[4] = perpendicularDistance(x, y, m_GfxInfo.x2 - 20, m_GfxInfo.y2, m_GfxInfo.x2, m_GfxInfo.y2);
	}
	else {
		int intermediateX = (m_GfxInfo.x1 + m_GfxInfo.x2) / 2;
		distancescount = 3;
		distances = new double[3];
		distances[0] = perpendicularDistance(x, y, m_GfxInfo.x1, m_GfxInfo.y1, intermediateX, m_GfxInfo.y1);
		distances[1] = perpendicularDistance(x, y, intermediateX, m_GfxInfo.y1, intermediateX, m_GfxInfo.y2);
		distances[2] = perpendicularDistance(x, y, intermediateX, m_GfxInfo.y2, m_GfxInfo.x2, m_GfxInfo.y2);
	}

	//Finding the shortest distance and returning it
	double smallest_distance = UI.width;
	for (int i = 0; i < distancescount; i++) {
		if (distances[i] < smallest_distance) {
			smallest_distance = distances[i];
		}
	}

	return smallest_distance;
}