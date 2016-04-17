#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SEQLENGTH 100
#define SEQCOUNT 20000000

static char bases[] = "ACGT";

void generate_random_sequence(char * sequence, long length, 
                              unsigned short * randstate)
{
  /* assumes sequence points to pre-allocated memory
     with space for length+1 characters */
  long i;
  for (i = 0; i < length; i++)
    sequence[i] = bases[nrand48(randstate) >> 29];
  sequence[length] = 0;
}

long find_longest_homopolymer(char * sequence)
{
  long length = strlen(sequence);
  long stretch = 0;
  long longest = 0;
  char previous = 0;

  long i;
  for(i = 0; i < length; i++)
    {
      char base = sequence[i];
      if (base != previous)
        stretch = 0;
      stretch++;
      if (stretch > longest)
        longest = stretch;
      previous = base;
    }
  return longest;
}

int main(int argc, char ** argv)
{
  srandom(time(0));

  long overall_longest = 0;
  char sequence[SEQLENGTH+1];
  long rounds = SEQCOUNT;
  unsigned short randstate[3] = {random(), 0, 0};

  long i;
  for(i = 0; i < rounds; i++)
    {
      generate_random_sequence(sequence, SEQLENGTH, randstate);
      long longest = find_longest_homopolymer(sequence);
      if (longest > overall_longest)
        overall_longest = longest;
    }
  
  printf("Length of longest homopolymer: %ld\n", overall_longest);
  return 0;
}
