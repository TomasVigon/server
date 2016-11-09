#include "watchPuppy.h"

watchPuppy::watchPuppy()
{
  timer=clock();      
}
    
void watchPuppy::feed_watchPuppy(void)
{
        timer=clock();
}

bool watchPuppy::watchPuppyAlert()
{
    return (clock()-timer)>MAX_TIME;  
}

