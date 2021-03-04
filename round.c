
#include "round.h"

long roundFloat(float num) {
    if(num >= 0.0f)
        return num + 0.5f;
    else
        return num - 0.5f;
}


