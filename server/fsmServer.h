
#ifndef FSMSERVER_H
#define FSMSERVER_H

#include <cstdlib>
#include <iostream>
#include <string>
//#include "cellType.h"
#include "packetManager.h"
#include "server.h"
#include "EventAndState.h"
#include "fileClass.h"

#define STATE_COUNT 4
#define EVENT_COUNT 8 
#define NOEVENT -1

using namespace std;

class fsmServer {
        typedef struct{
            typeState nextState;
            void (fsmServer::*action) (void);
	}cellType;
        
public:
    fsmServer();
    virtual ~fsmServer();
    void cicleFsm(typeEvent event);
    cellType getCell(void);
    typeState getCellState(void);
    bool isEvent();
    typeEvent getEvent();
    void setFilename(string& name);	
	
private:
        string packet;
	cellType cell;
        string filename;
        fileClass file;        
        packetManager p;
        server clientServer;
        const cellType FSM_matrix[STATE_COUNT][EVENT_COUNT] = {

	//wrq							rrq						timeount						ack                     quit				last_data 					data       					error
	{{ WRITE,&fsmServer::sendAck},		{ READ, sendData },			{ IDLE, nothing },				{ IDLE, nothing },		{ IDLE, end },		{ IDLE, nothing },			{ IDLE, nothing },			{ IDLE, errorEvent } },		//IDLE
	{{ WRITE, &fsmServer::nothing},		{ WRITE, nothing },			{ WRITE, sendAck },				{ WRITE, nothing },		{ IDLE, end },		{ IDLE, sendAck },			{ WRITE, sendAck },			{ IDLE, errorEvent } },		//WRITE
	{{ READ, nothing },			{ READ, nothing },			{ READ, sendData },				{ READ, sendData },		{ IDLE, end },		{ LAST_READ, nothing },		{ READ, nothing },			{ IDLE, errorEvent } },		//READ
	{{ LAST_READ, nothing },		{ LAST_READ, nothing },		{ LAST_READ, sendLastData },	{ IDLE, nothing },		{ IDLE, end },		{ LAST_READ, nothing },		{ LAST_READ, nothing },		{ IDLE, errorEvent } },		//LAST_READ
};

	void sendWrq(void);
	void sendRrq(void);
	void end(void);
	void errorEvent(void);
	void sendData(void);
	void sendAck(void);	
	void nothing(void);
        
        
  

};


#endif /* FSMSERVER_H */
