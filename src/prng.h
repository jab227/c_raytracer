#ifndef PRNG_H_
#define PRNG_H_

double randomd();
double randomd_in(double min, double max);
#endif // PRNG_H_

#ifdef PRNG_IMPL

#include <math.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>

#define MAGIC_VALUE 6364136223846793005ULL

typedef struct
{
    uint64_t state;
    uint64_t inc;
} PCG32Random;

static uint32_t
pcg32_random_r(PCG32Random *rng)
{
    assert(rng != NULL);
    uint64_t oldstate = rng->state;
    rng->state = oldstate * MAGIC_VALUE + (rng->inc | 1);

    uint32_t xorshifted = (uint32_t) (((oldstate >> 18u) ^ oldstate) >> 27u);
    uint32_t rot = (uint32_t) (oldstate >> 59u);

    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

static void
pcg32_srandom(PCG32Random *rng, uint64_t init_state, uint64_t initseq)
{
    assert(rng != NULL);
    rng->state = 0U;
    rng->inc = (initseq << 1u) | 1;
    pcg32_random_r(rng);
    rng->state += init_state;
    pcg32_random_r(rng);
}

double
randomd()
{
    static int initialized = 0;
    static PCG32Random rng = { 0 };
    if (!initialized) {
        pcg32_srandom(&rng, (uint64_t) time(NULL), (uint64_t) &rng);
        initialized = 1;
    }
    return ldexp(pcg32_random_r(&rng), -32);
}

double
randomd_in(double min, double max)
{
    return min + (max - min) * randomd();
}

#endif // PRNG_IMPL

