
#include "fsmServer.h"

fsmServer::fsmServer() {
    cell.nextState=IDLE;
    cell.action=&fsmServer::nothing;
}


fsmServer::~fsmServer() {
}


void fsmServer::sendAck(void)
{
    if(p.getPacketBLock(packet)-1==file.getChunkNum())
    {
    string dataString;
    cout << "enviando ACK" << endl;
    p.getPacketData(packet,dataString);
    cout << "esta es la data que llego" << dataString << endl;
    
    
    
    file.increaseChunkNum();// VOLVER A PONERRRRRRR
    file.chunkToFile(dataString);//GUARDO LA INFO EN EL ARCHIVO
    p.createPacket(packet,ack,file.getChunkNum());
    s.sendInfo(packet); //VOLVER APON ER
    if(dataString.length()<512)cicleFsm(last_send);
    }
    else
    {
        cout << "error ack" << endl;
        cicleFsm(error);
    }
   
}

void fsmServer::acceptWRQ(void)
{
    p.getPacketFileName(packet,filename);
    cout << "recibiendo archivo: " << filename << endl;
    file.openwFile(filename);
    //string dataString;
    //p.getPacketData(packet,dataString);
    //file.chunkToFile(dataString);//GUARDO LA INFO EN EL ARCHIVO
    //file.increaseChunkNum();
    p.createPacket(packet,ack,file.getChunkNum());
    cout << "paquete a enviar" << packet << endl;
    s.sendInfo(packet); //VOLVER APON ER
}

void fsmServer::acceptRRQ(void)
{
    p.getPacketFileName(packet,filename);
    cout << "Enviando archivo:" << filename << endl;
    //file.openrFile(filename);
    if(file.openrFile(filename))
    {
        //SI NUMERO ACK CORRESPONDE A NUMERO DATA
        file.increaseChunkNum();
	string dataString=file.getChunk();
	p.createPacket(packet,data,dataString,file.getChunkNum());
	s.sendInfo(packet);
    }
    else
    {
        cout << "no pude abrir el archivo" << endl;
        p.createErrorPacket(packet);
        s.sendInfo(packet);
    }
    
}

void fsmServer::errorEvent(void)
{
    //ACA LOS ERRORES
    p.createErrorPacket(packet);
    s.sendInfo(packet);
}


void fsmServer::cicleFsm(typeEvent event)
{
    if(event==ack)cout << "ACK" << endl;
    if(cell.nextState==LAST_READ) cout <<"se termino la enviada" << endl;
    cell=fsm_matrix[cell.nextState][event];
    //cell.*((fsmClient*)this)->fsmClient::cellType::action();
    //cell.*(fsmClient::cellType::(fsmClient::action))();
    cout << "apunto de ralizar accion" << endl;
    ((*this).*(cell.action))();
    cout << "accion realizada" << endl;
    timeAlert.feed_watchPuppy();
}

void fsmServer::sendData(void)
{
        if(p.getPacketBLock(packet)==file.getChunkNum())
        {
        file.increaseChunkNum();
	string dataString=file.getChunk();
        if(file.End())
        {
            cell.nextState=LAST_READ;       
            cout << "enviando ultima data" << endl;
        }
	p.createPacket(packet,data,dataString,file.getChunkNum());
	s.sendInfo(packet); // VOLVER A PONER
        }
        else
        {
            cicleFsm(error);
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

void fsmServer::end(void)
{
    cout << "fin transmision" << endl;
    file.closeFile();
}

bool fsmServer::connectServer()
{
    s.doServerConnect();
    return s.isConnected();    
}

bool fsmServer::isTimebreak()
{
    return timeAlert.watchPuppyAlert()&&cell.nextState!=IDLE&&cell.nextState!=FINISH;
}

void fsmServer::resend(void)
{
    s.sendInfo(packet); 
}

/* bool fsmServer::isQuit(void)
 {
     cout << "esta fncion de mierda me caga el programa" <<endl;
     return (tolower(getch())=='q');
 }*/

void fsmServer::reset(void)
{
    cell.nextState=IDLE;
    cell.action=&fsmServer::nothing;
}