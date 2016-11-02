
#include <cstdlib>
#include <iostream>
#include <string>
#include "fsmServer.h"

using namespace std;


int main(int argc, char** argv) {

    fsmServer fsm;
    string data;
    if(fsm.connectServer())
    while(fsm.getCellState()!=FINISH)
    {
        //ACA TIENE QUE IR TIMEOUT Y KEYBOARD
        if(fsm.isEvent())
        fsm.cicleFsm(fsm.getEvent());
    }
    cout << endl << "fin";
    
    return 0;
}

