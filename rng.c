#include "rng.h"

static const unsigned a = 69504049;
static const unsigned b = 81414301;
static const unsigned c = 93562019;

static unsigned rng = 0;

/* srand -- seed the rng */
void srand(unsigned seed) {
    rng=seed;
    rand();
    rand();
}
/* rand -- generates a random number */
unsigned rand() {
    unsigned old=rng;
    rng=a*rng*rng+b*rng+c;
    return old;
}
