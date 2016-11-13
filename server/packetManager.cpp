#include "packetManager.h"

bool packetManager::createPacket(char* packet, typeEvent event, string& dataString, unsigned int blockNum)
{//ESTE ES PARA DATA
    memset(packet, 0, strlen(packet));
    
    packet[0]='0';
    if(event==data)
    packet[1]='3';
    else return false;
    packet[2]=blockNum;    
    strcpy(packet+3,dataString.c_str());
    return true;
}

bool packetManager::createPacket(char* packet, typeEvent event, string& dataString)
{//ESTE ES PARA WRQ Y RRQ
    memset(packet, 0, strlen(packet));
    packet[0]='0';
    if(event==rrq)packet[1]='1';//RRQ
    else if(event==wrq)packet[1]='2';//WRQ	
    else return false;
    strcpy(packet+3,dataString.c_str());
    //cout << "el paquete es:" << packet << endl;
    //packet+="0"; //NO ES ESTE CERO
    strcpy(packet+4+dataString.length(),TRANS_MODE);
    return true;
}

bool packetManager::createPacket(char* packet, typeEvent event, unsigned int blockNum)
{//ESTE ES PARA ACK
        memset(packet, 0, strlen(packet));
        packet[0]='0';
	if(event==ack) packet[1]='4';
	else return false;
	packet[2]=blockNum;
        packet[3]=0;
	return true;

}

bool packetManager::createErrorPacket(char* packet)
{	
    memset(packet, 0, strlen(packet));
    packet[0]='0';
    packet[1]='5';
    return true;
}

typeEvent packetManager::getPacketType(char* packet)
{
	typeEvent event;
	if(packet[1]=='1')event=rrq;
	else if(packet[1]=='2') event=wrq;
	else if(packet[1]=='3') event=data;
	else if(packet[1]=='4') event=ack;
	else if(packet[1]=='5') event=error;
	else event=no_event;
	return event;
}

unsigned int packetManager::getPacketBLock(char* packet)
{	
return packet[2];
}

void packetManager::getPacketData(char* packet, string& dataString)
{
    char dataAux[512];
    strcpy(dataAux,packet+3);
    dataString=string(dataAux);
}
void packetManager::getPacketFileName(char* packet, string& dataString)
{
    char dataAux[512];
    
    //char *strchr(packet,0)
    strcpy(dataAux,packet+3);
    dataString=string(dataAux);
}