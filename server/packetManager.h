
#ifndef PACKETMANAGER_H
#define PACKETMANAGER_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <string.h>

#include "EventAndState.h"

#define TRANS_MODE "NETASCII"
#define MAXERROR 99

using namespace std;

class packetManager
{
public:

        //packetManager();
	bool createPacket(char* packet, typeEvent event, string& dataString, unsigned int blockNum); 
	bool createPacket(char* packet, typeEvent event, string& dataString);
	bool createPacket(char* packet, typeEvent event, unsigned int blockNum);
	bool createErrorPacket(char* packet);
	typeEvent getPacketType(char* packet);
	unsigned int getPacketBLock(char* packet);
	void getPacketData(char* packet, string& dataString);
	void getPacketFileName(char* packet, string& dataString);

};



#endif /* PACKETMANAGER_H */

