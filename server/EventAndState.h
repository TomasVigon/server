
#ifndef EVENTANDSTATE_H
#define EVENTANDSTATE_H





typedef enum {IDLE,WRITE,READ,LAST_READ,FINISH} typeState;
typedef enum {wrq,rrq,timebreak,ack,quit,last_send,data,error,no_event}	typeEvent;




#endif /* EVENTANDSTATE_H */

