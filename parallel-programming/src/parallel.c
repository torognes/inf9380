#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#define SEQLENGTH 100
#define SEQCOUNT 20000000
#define THREADS 1

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
  /* calculate length of longest homopolymer in the sequence.
     homopolymer = stretch of identical bases */

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

void * worker(void * t)
{
  long overall_longest = 0;
  char sequence[SEQLENGTH+1];
  long rounds = SEQCOUNT / THREADS;
  unsigned short randstate[3] = {random(), 0, 0};

  /* generate SEQCOUNT/THREADS sequences of length SEQLENGTH each,
     and find the longest homopolymer */

  printf("Thread %ld processing %ld sequences.\n", (long) t, rounds);
  long i;
  for(i = 0; i < rounds; i++)
    {
      generate_random_sequence(sequence, SEQLENGTH, randstate);
      long longest = find_longest_homopolymer(sequence);
      if (longest > overall_longest)
        overall_longest = longest;
    }
  return (long *) overall_longest;
}

int main(int argc, char ** argv)
{
  long total_longest = 0;
  long result[THREADS];
  pthread_attr_t attr;
  pthread_t thread[THREADS];

  /* initialise random number generator with a "random" seed */

  srandom(time(0));

  /* Initialize thread attributes with defaults */
  if (pthread_attr_init(&attr))
    {
    fprintf(stderr, "Fatal error: Unable to initialize thread attributes\n");
      exit(1);
    }


  /* create THREAD threads */

  long t;
  for (t = 0; t < THREADS; t++)
    {
      long r = pthread_create(&thread[t], & attr, & worker, (void*) t);
      if (r == 0)
        fprintf(stderr, "Thread %ld started successfully.\n", t);
      else
        {
          fprintf(stderr, "Fatal error: Unable to start thread.");
          exit(1);
        }
    }

  /* wait for the threads to finish, and collect results */

  for (t = 0; t < THREADS; t++)
    {
      long r = pthread_join(thread[t], (void**)&result[t]);
      
      if (r == 0)
        fprintf(stderr, "Thread %ld successfully joined with result %ld.\n", t, result[t]);
      else
        {
          fprintf(stderr, "Fatal error: Unable to join thread.");
          exit(1);
        }
      
      if (result[t] > total_longest)
        total_longest = result[t];
    }
  
  printf("Length of longest homopolymer: %ld\n", total_longest);

  return 0;
}
