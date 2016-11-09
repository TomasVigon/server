#ifndef WATCHPUPPY_H
#define WATCHPUPPY_H

#include <time.h>
#include <cstdlib>
#include <iostream>

#define SEG 5
#define MAX_TIME SEG * CLOCKS_PER_SEC //240

class watchPuppy
{
public:
    watchPuppy();
    void feed_watchPuppy(void);
    bool watchPuppyAlert();

private:
    clock_t timer;

};

#endif /* WATCHPUPPY_H */

