
#ifndef FILECLASS_H
#define FILECLASS_H

#include <cstdlib>
#include <fstream>
#include <iostream>


#define CHUNKSIZE 512

using namespace std;


class fileClass {
public:
    fileClass();
    fileClass(const fileClass& orig);
    virtual ~fileClass();
    
    string getChunk(); //Setear chunkSize = 512 para TFTP
    void increaseChunkNum();
    void decreaseChunKNum();
    bool End();
    bool openrFile(string str);
    void openwFile(string str);
    void closeFile();
    void chunkToFile(string str);
    unsigned int getChunkNum();
    bool getCheckFile();
    
    //aguante boca y github
private:
    string dwld;
    ifstream readFile;
    ofstream writeFile;
    unsigned int chunkNum;
    bool checkFile;
    string chunk;
};

#endif /* FILECLASS_H */


