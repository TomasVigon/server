
#include <cstdlib>
#include <iostream>
#include <string>
#include "fsmServer.h"

using namespace std;

/*void funciondemierda (void)
{
    getch();
}*/

int main(int argc, char** argv) {

    fsmServer fsm;
    //nodelay(stdscr, TRUE);
    if(fsm.connectServer(argv[1]))
    {
        cout << "se CONECTO EL CLIENTE" << endl;
        do
        {
        while(fsm.getCellState()!=FINISH)
        {
            //ACA TIENE QUE IR TIMEOUT Y KEYBOARD
            if(fsm.isEvent())
            {
                cout << "llego un paquete" << endl;                
                fsm.cicleFsm(fsm.getEvent());
            }
            
            if(fsm.isTimebreak())
            {
                cout << "timeout papa";
                fsm.cicleFsm(timebreak);        
            }
            /*if(fsm.isQuit())
            {
                cout << "se apreto Q"<< endl;
                fsm.cicleFsm(quit);
            }*/
        }
        fsm.reset();
        cout<<"marccccccccccccccccccccccc"<<endl;
        }
        while(1);
        
        
    }
    cout << endl << "fin";
    
    return 0;
}

