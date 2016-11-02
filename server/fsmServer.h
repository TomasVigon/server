
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

#define STATE_COUNT 5
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
    bool connectServer();
	
private:
        string packet;
	cellType cell;
        string filename;
        fileClass file;        
        packetManager p;
        server s;
        const cellType fsm_matrix[STATE_COUNT][EVENT_COUNT] = {

            //wrq						rrq					timeount					ack                                         quit				last_data 					data       					error
	{{ WRITE,&fsmServer::sendAck},              { READ, &fsmServer::sendData },		{ IDLE, &fsmServer::nothing },			{ IDLE, &fsmServer::nothing },		{ FINISH, &fsmServer::end },		{ IDLE, &fsmServer::nothing },			{ IDLE, &fsmServer::nothing },			{ IDLE, &fsmServer::errorEvent } },		//IDLE              
	{{ WRITE, &fsmServer::nothing},             { WRITE, &fsmServer::nothing },		{ WRITE, &fsmServer::sendAck },			{ WRITE, &fsmServer::nothing },		{ FINISH, &fsmServer::end },		{ FINISH, &fsmServer::sendAck },			{ WRITE, &fsmServer::sendAck },			{ IDLE, &fsmServer::errorEvent } },		//WRITE             
	{{ READ, &fsmServer::nothing },             { READ, &fsmServer::nothing },		{ READ, &fsmServer::sendData },			{ READ, &fsmServer::sendData },		{ FINISH, &fsmServer::end },		{ LAST_READ, &fsmServer::nothing },		{ READ, &fsmServer::nothing },			{ IDLE, &fsmServer::errorEvent } },		//READ              
	{{ LAST_READ, &fsmServer::nothing },        { LAST_READ, &fsmServer::nothing },		{ LAST_READ, &fsmServer::sendData },            { IDLE, &fsmServer::nothing },		{ FINISH, &fsmServer::end },		{ LAST_READ, &fsmServer::nothing },		{ LAST_READ, &fsmServer::nothing },		{ IDLE, &fsmServer::errorEvent } },		//LAST_READ         
        {{ FINISH, &fsmServer::nothing },           { FINISH, &fsmServer::nothing },            { FINISH, &fsmServer::nothing },                { FINISH, &fsmServer::nothing },        { FINISH, &fsmServer::nothing },        { FINISH, &fsmServer::nothing },                { FINISH, &fsmServer::nothing },                { FINISH, &fsmServer::nothing }  },             //FINISH          
                

        };

	//void sendWrq(void);
	//void sendRrq(void);
	void end(void);
	void errorEvent(void);
	void sendData(void);
	void sendAck(void);	
	void nothing(void);
        
  

};


#endif /* FSMSERVER_H */
