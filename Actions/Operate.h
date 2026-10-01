#pragma once
#include "../Components/Component.h"
#include "../Components/Connection.h"
#include "../Components/Gate.h"

const int maxCompCount = 200;

class Operate
{
	Component* CompList[maxCompCount];
	int CompCount;
public:
	Operate(Component**, int);
	~Operate();
	void OperateAll();
	void OperateGate(Gate* gate, Connection** stage_connections, int& ConnCounter);
	void OperateConnection(Connection* conn, Gate** stage_gates, int& GatesCounter);
};
