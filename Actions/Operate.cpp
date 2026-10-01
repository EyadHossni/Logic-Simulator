#include "Operate.h"

//checking if a gate is within the list of components
bool InList(Gate** Allgates, Gate* gate, int size) {
	for (int i = 0; i < size; i++) {
		if (gate == Allgates[i]) {
			return true;
		}
	}
	return false;
}

Operate::Operate(Component** comps, int count) {
	//making copies of the components
	CompCount = count;
	for (int i = 0; i < count; i++) {
		CompList[i] = comps[i];
	}
	for (int i = count; i < maxCompCount; i++) {
		CompList[i] = NULL;
	}
}

Operate::~Operate() {
	delete CompList;
}

void Operate::OperateAll() {
	int ConnCounter = 0;
	int GatesCounter = 0;
	Connection** stage_connections = new Connection* [CompCount];
	Gate** stage_gates = new Gate * [CompCount];
	for (int i = 0; i < CompCount; i++) {
		if (CompList[i]->GetComponentType() == "Gate") ((Gate*)CompList[i])->CheckCorrection();
	}

	//recursive operation starting from the gates to the end
	for (int i = 0; i < CompCount; i++) {
		if (((Gate*)CompList[i])->GetType() == ITM_SWITCH) {
			OperateGate((Gate*)CompList[i], stage_connections, ConnCounter);
		}
	}
	do {
		GatesCounter = 0;
		for (int i = 0; i < ConnCounter; i++) {
			OperateConnection(stage_connections[i], stage_gates, GatesCounter);
		}

		ConnCounter = 0;
		for (int i = 0; i < GatesCounter; i++) {
			OperateGate(stage_gates[i], stage_connections, ConnCounter);
		}

	} while (ConnCounter > 0);
}

void Operate::OperateGate(Gate* gate, Connection** stage_connections, int& ConnCounter) {
	if (gate->GetCorrect()) gate->Operate();	//checking if it's fully connected
	int numOfConn = gate->GetOutputPin()->GetConnectionsCount();
	for (int i = 0; i < numOfConn; i++) {
		stage_connections[ConnCounter++] = gate->GetOutputPin()->GetConnections(i);
	}
}
void Operate::OperateConnection(Connection* conn, Gate** stage_gates, int& GatesCounter) {
	conn->Operate();
	Gate* gate = (Gate*)(conn->getDestPin()->getComponent());
	if (!InList(stage_gates, gate, GatesCounter) && gate != nullptr) stage_gates[GatesCounter++] = gate;
}