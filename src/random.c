unsigned long next = 1; // This variable holds the next random value.

// Seed the generator
void srand(unsigned int seed)
{
    next = seed;
}

// Generate a pseudo-random number
int rand()
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;
}
