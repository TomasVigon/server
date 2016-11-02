
#ifndef EVENTANDSTATE_H
#define EVENTANDSTATE_H





typedef enum {IDLE,FIRST_WRITE,FIRST_READ,WRITE,READ,LAST_WRITE,FINISH} typeState;
typedef enum {wrq,rrq,timebreak,ack,quit,last_data,data,error,no_event}	typeEvent;



#endif /* EVENTANDSTATE_H */

