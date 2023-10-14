#ifndef _NETWORK_UTILS_H_
#define _NETWORK_UTILS_H_

#include <iostream>

#define SOCKETERROR (-1)

int check(int exp, const char *msg) {
    if(exp == SOCKETERROR) {
        perror(msg);
        exit(1);
    }
    return 0;
}

#endif /* _NETWORK_UTILS_H_ */