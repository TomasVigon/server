
#include "fsmServer.h"

fsmServer::fsmServer() {
}

fsmServer::fsmServer(const fsmServer& orig) {
}

fsmServer::~fsmServer() {
}





#include "serverFsm.h"
#include <ctype.h>
//typedef
struct cellType_t{
	typeState nextState;
	void(*action)(ptdata);
};

//prototype
static void sendWrq(ptdata userData);
static void sendRrq(ptdata userData);
static void sendData(ptdata userData);
static void sendAck(ptdata userData);
static void sendLastData(ptdata userData);
static void errorEvent(ptdata userData);
static void nothing(ptdata userData);
static void end(ptdata userData);
static void changeEvent(typeEvent event, ptdata userData);
static void changeState(typeState state, ptdata userData);

cellType FSM_matrix[STATES_COUNT][EVENTS_COUNT] = {

	//wrq						rrq				timeount						ack                               quit				last_data 			data       			error
	{ { WRITE, sendAck },			{ READ, sendData },			{ IDLE, nothing },				{ IDLE, nothing },		{ IDLE, end },                  { IDLE, nothing },		{ IDLE, nothing },		{ IDLE, errorEvent } },		//IDLE
	{ { WRITE, nothing },			{ WRITE, nothing },			{ WRITE, sendAck },				{ WRITE, nothing },		{ IDLE, end },                  { IDLE, sendAck },		{ WRITE, sendAck },		{ IDLE, errorEvent } },		//WRITE
	{ { READ, nothing },			{ READ, nothing },			{ READ, sendData },				{ READ, sendData },		{ IDLE, end },                  { LAST_READ, nothing },		{ READ, nothing },		{ IDLE, errorEvent } },		//READ
	{ { LAST_READ, nothing },		{ LAST_READ, nothing },                 { LAST_READ, sendLastData },                    { IDLE, nothing },		{ IDLE, end },                  { LAST_READ, nothing },		{ LAST_READ, nothing },		{ IDLE, errorEvent } },		//LAST_READ
};

cell createFSM(void)
{
	cell cellFSM = (cell)malloc(sizeof(cellType));
	cellFSM->nextState = IDLE;
	cellFSM->action = nothing;
	return cellFSM;
}

int getEvent(void)
{
	int c;
	c = getchar();
	c = tolower(c);
	if ((c >= 'a') && (c <= 'h'))
		return c;
	else
		return NOEVENT;
}

int cicleFsm(cell fsmData, int event, ptdata userData)
{

	event = event - 'a';
	changeEvent(event, userData);
	*fsmData = FSM_matrix[fsmData->nextState][event];
	(fsmData->action)(userData);
	changeState(fsmData->nextState, userData);
	printStatus(userData);
	return event;



}

void sendWrq(ptdata userData)
{
	writeAction(userData, "Wrq sent");
}

void sendRrq(ptdata userData)
{
	writeAction(userData, "Rrq sent");
}

void sendData(ptdata userData)
{
	writeAction(userData, "Data sent");
}

void sendAck(ptdata userData)
{
	writeAction(userData, "Ack sent");
}

void sendLastData(ptdata userData)
{
	writeAction(userData, "Last data sent");
}

void errorEvent(ptdata userData)
{
	writeAction(userData, "Error");
}

static void end(ptdata userData)
{
	writeAction(userData, "End");
}

void changeState(typeState state, ptdata userData)
{
	if (state == IDLE)writeState(userData, "Start");
	else if (state == WRITE)writeState(userData, "Writing");
	else if (state == READ)writeState(userData, "Reading");
	else if (state == LAST_READ)writeState(userData, "Reading last package");
}



void changeEvent(typeEvent event, ptdata userData)
{
	if (event == wrq)writeEvent(userData, "Wrq");
	else if (event == rrq)writeEvent(userData, "Rrq");
	else if (event == timeout)writeEvent(userData, "Time out");
	else if (event == ack)writeEvent(userData, "Ackowledge");
	else if (event == quit)writeEvent(userData, "Quit");
	else if (event == last_data)writeEvent(userData, "Last Data");
	else if (event == data)writeEvent(userData, "Data");
	else if (event == error)writeEvent(userData, "Error");
}

void nothing(ptdata userData)
{
	writeAction(userData, "none");
}