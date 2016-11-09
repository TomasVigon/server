
#include "fileClass.h"

fileClass::fileClass() {
    dwld="Downloads/";
    checkFile=false;
    chunkNum=0;
}

fileClass::fileClass(const fileClass& orig) {
}

fileClass::~fileClass() {
    if (readFile.is_open())       
    {
        readFile.close();             //por si nos olvidamos de llamarlo
    }
}

bool fileClass::openrFile(string& str)
{
    
    readFile.open((dwld+str).c_str(),fstream::in);      //el path es "Downloads/nombre del archivo"
    
    if (readFile.is_open())       //verifico que se haya abierto exitosamente
    {
        checkFile=true;
    }
    else
    {
        checkFile=false; 
    }
    
  return checkFile;
}

void fileClass::openwFile(string& str)           //no chequeo porque si no existe el archivo lo crea
{
    writeFile.open((dwld+str).c_str(),fstream::out);
}

void fileClass::closeFile()
{
    readFile.close();
    writeFile.close();
}

string fileClass::getChunk()
{
    char aux[CHUNKSIZE];
    int pos = 0;
    pos = CHUNKSIZE * (chunkNum-1);
    readFile.seekg(pos);
    
    readFile.get(aux,CHUNKSIZE+1,'\0');
    chunk= string(aux);
    cout<<"Extracted "<<readFile.gcount()<<" characters"<<endl;
    return chunk;
}

void fileClass::chunkToFile(string& str)
{
    int pos = 0;
    pos = CHUNKSIZE * (chunkNum-1);
    writeFile.seekp(pos);
    
    
    cout << "numero de chunck=" <<pos << endl;
    cout << "la data a guardar es " << str << endl;
    
    writeFile.write(str.c_str(),str.length());
    writeFile.flush();
    cout << "escribiendo archivo" << endl;
    
    
}

bool fileClass::getCheckFile()
{
    return checkFile;
}


void fileClass::increaseChunkNum()
{
    chunkNum++;
}

void fileClass::decreaseChunKNum()
{
    chunkNum--;
}


bool fileClass::End()
{
    return readFile.eof();
}

unsigned int fileClass::getChunkNum()
{
    return chunkNum;
}