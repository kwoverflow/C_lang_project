#include "util.h"

int rand_int(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

int numlen(int n) {
    return (int)ceil(log10((double)n));
}
