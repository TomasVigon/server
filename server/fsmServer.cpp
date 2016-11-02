
#include "fsmServer.h"

fsmServer::fsmServer() {
    cell.nextState=IDLE;
    cell.action=&fsmServer::nothing;
}


fsmServer::~fsmServer() {
}


void fsmServer::sendAck(void)
{
    string dataString;
    p.getPacketData(packet,dataString);
    file.chunkToFile(dataString);//GUARDO LA INFO EN EL ARCHIVO
    file.increaseChunkNum();
    p.createPacket(packet,ack,file.getChunkNum());
    s.sendInfo(packet); //VOLVER APON ER
   
}

void fsmServer::acceptWRQ(void)
{
    p.getPacketFileName(packet,filename);
    file.openwFile(filename);
    string dataString;
    p.getPacketData(packet,dataString);
    file.chunkToFile(dataString);//GUARDO LA INFO EN EL ARCHIVO
    file.increaseChunkNum();
    p.createPacket(packet,ack,file.getChunkNum());
    s.sendInfo(packet); //VOLVER APON ER
}

void fsmServer::acceptRRQ(void)
{
    p.getPacketFileName(packet,filename);
    file.openrFile(filename);
    if(file.getCheckFile()==true)
    {
        //SI NUMERO ACK CORRESPONDE A NUMERO DATA
        file.increaseChunkNum();
	string dataString=file.getChunk();
	p.createPacket(packet,data,dataString,file.getChunkNum());
	s.sendInfo(packet);
    }
    else
        cout << "no pude abrir el archivo";
}

void fsmServer::errorEvent(void)
{
    //ACA LOS ERRORES
}


void fsmServer::cicleFsm(typeEvent event)
{
    cell=fsm_matrix[cell.nextState][event];
    //cell.*((fsmClient*)this)->fsmClient::cellType::action();
    //cell.*(fsmClient::cellType::(fsmClient::action))();
    ((*this).*(cell.action))();
}

void fsmServer::sendData(void)
{
        //SI NUMERO ACK CORRESPONDE A NUMERO DATA
        file.increaseChunkNum();
	string dataString=file.getChunk();
        if(file.End())
        {
            cell=fsm_matrix[cell.nextState][last_data];
        }
        else
        {
	p.createPacket(packet,data,dataString,file.getChunkNum());
	s.sendInfo(packet); // VOLVER A PONER
        }
}

bool fsmServer::isEvent()
{
    return s.isEvent(packet);
}

void fsmServer::setFilename(string& name)
{
    filename=name;
}

void fsmServer::nothing()
{
    
}

typeState fsmServer::getCellState(void)
{
    return cell.nextState;
}
        
typeEvent fsmServer::getEvent()
{
    return p.getPacketType(packet);
}

void fsmServer::end()
{
    //CIERRA EL ARCHIVO
    //Y HACES OTRAS COSAS
}

bool fsmServer::connectServer()
{
    s.doServerConnect();
    return s.isConnected();    
}

