
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
#include "watchPuppy.h"
#include "ncurses.h"


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
    bool connectServer(char * ip);
    bool isTimebreak(void);
    void reset(void);
    //bool isQuit(void);
	
private:
        char* packet;
	cellType cell;
        watchPuppy timeAlert;
        string filename;
        fileClass file;        
        packetManager p;
        server s;
        const cellType fsm_matrix[STATE_COUNT][EVENT_COUNT] = {

        //wrq                                       rrq                                         timeount					ack                                      quit                                   last_send 				data       					error
	{{ WRITE, &fsmServer::acceptWRQ},           { READ, &fsmServer::acceptRRQ },		{ IDLE, &fsmServer::nothing },			{ IDLE, &fsmServer::nothing },		{ FINISH, &fsmServer::end },		{ IDLE, &fsmServer::nothing },		{ IDLE, &fsmServer::nothing },			{ IDLE, &fsmServer::errorEvent } },		//IDLE              
	{{ WRITE, &fsmServer::nothing},             { WRITE, &fsmServer::nothing },		{ WRITE, &fsmServer::resend},			{ WRITE, &fsmServer::nothing },		{ FINISH, &fsmServer::end },		{ IDLE, &fsmServer::end },		{ WRITE, &fsmServer::sendAck },			{ IDLE, &fsmServer::errorEvent } },             //WRITE             
	{{ READ, &fsmServer::nothing },             { READ, &fsmServer::nothing },		{ READ, &fsmServer::resend },			{ READ, &fsmServer::sendData },		{ FINISH, &fsmServer::end },		{ IDLE, &fsmServer::end },		{ READ, &fsmServer::nothing },			{ IDLE, &fsmServer::errorEvent } },		//READ              
	{{ LAST_READ, &fsmServer::nothing },        { LAST_READ, &fsmServer::nothing },		{ LAST_READ, &fsmServer::resend},               { IDLE, &fsmServer::end },		{ FINISH, &fsmServer::end },		{ LAST_READ, &fsmServer::nothing },	{ LAST_READ, &fsmServer::nothing },		{ IDLE, &fsmServer::errorEvent } },		//LAST_READ         
        {{ FINISH, &fsmServer::nothing },           { FINISH, &fsmServer::nothing },            { FINISH, &fsmServer::nothing },                { FINISH, &fsmServer::nothing },        { FINISH, &fsmServer::nothing },        { FINISH, &fsmServer::nothing },        { FINISH, &fsmServer::nothing },                { FINISH, &fsmServer::nothing  } },             //FINISH          
                

        };

	void end(void);
	void errorEvent(void);
	void sendData(void);
	void sendAck(void);	
	void nothing(void);
        void acceptWRQ(void);
        void acceptRRQ(void);
        void resend(void);
        
        

};


#endif /* FSMSERVER_H */
