
#ifndef PACKETMANAGER_H
#define PACKETMANAGER_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>

#include "EventAndState.h"

#define TRANS_MODE "NETASCII"
#define MAXERROR 99

using namespace std;

class packetManager
{
public:

        //packetManager();
	bool createPacket(string& packet, typeEvent event, string& dataString, unsigned int blockNum); 
	bool createPacket(string& packet, typeEvent event, string& dataString);
	bool createPacket(string& packet, typeEvent event, unsigned int blockNum);
	bool createErrorPacket(string& packet);
	typeEvent getPacketType(string& packet);
	unsigned int getPacketBLock(string& packet);
	void getPacketData(string& packet, string& dataString);
	void getPacketFileName(string& packet, string& dataString);

};



#endif /* PACKETMANAGER_H */

