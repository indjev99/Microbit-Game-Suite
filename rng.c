#include "rng.h"

static const unsigned int a = 21347;
static const unsigned int b = 35771;
static const unsigned int m = 59887;

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
    rng=(a*rng+b)%m;
    return old;
}
