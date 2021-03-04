
#include "round.h"

long roundFloat(float num) {
    if(num >= 0)
        return num + 0.5;
    else
        return num - 0.5;
}


