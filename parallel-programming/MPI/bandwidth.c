#define MODULE    "BANDWIDTH"
#define MODULE_ID "@(#)$Id: bandwidth.c 49904 2006-09-29 23:40:38Z aostvold $"
/* Module --------------------------------------------------------------------
 * 
 * Copyright(c) 1996 Scali AS - All Rights Reserved.
 *
 * $RCSfile$
 * 
 * CREATED
 *   Author: hob (Hakon Ording Bugge)
 *   Date:   2001/02/28 16:28:41
 * 
 * LAST CHANGED
 *   $Author: aostvold $
 *   $Date: 2006-09-30 01:40:38 +0200 (Sat, 30 Sep 2006) $
 * 
 * DESCRIPTION
 *   This program serves the purpose of assessing point-to-point performance
 *    of an MPI implementation.
 *   
 * WARNING
 *   Scali considers this program not to be the *best* program for measuring
 *   performance, but it is included for its simplicity. Scali considers
 *   collective operations to be more valuable in assesing a clusters'
 *   performance, hence, use Pallas PMB or mpptest instead.
 *   
 *   
 * -------------------------------------------------------------------------*/

/* Dependencies ------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mpi.h"
#include <malloc.h>

/* Constants ---------------------------------------------------------------*/

#define TAG 123

#define DO_PING_PING 1
#define DO_PING_PONG 2
#define DO_EXCHANGE  4

/* Typedefs ----------------------------------------------------------------*/

typedef void (*bench_rout_t)(int, char *, char *, int);
typedef struct {
      bench_rout_t bench;
      char *mnem;
      int scale;
      int mask;
} bench_desc_t;
      

/* Prototypes --------------------------------------------------------------*/
void ping_ping(int iter, char *buf1, char *buf2, int len);
void ping_pong(int iter, char *buf1, char *buf2, int len);
void exchange (int iter, char *buf1, char *buf2, int len);
void print_header(char *str);
void measure(bench_rout_t rout, int scale, int sz);
void do_usage(void);
void do_help(void);
void do_exit(int exit_code);


/* Globals -----------------------------------------------------------------*/

int        rank,
           size,
           root = 0,
           touch_tx_initially=1,
           touch_tx_always=0,
           touch_rx=0,
           align=0,
           reverse=0;
const int  min_iter=2;
double     tgoal=0.1,
	   tbar=0.0,
	   tres;

MPI_Comm   comm = MPI_COMM_WORLD;


void ping_ping(int iter, char *buf, char *spare, int len) {
   MPI_Status sts;
   int i, other = (rank^1) & 1;
   
   for (i=0; i < iter; i++) {
      if (rank & 1) {
	 MPI_Recv(buf, len, MPI_BYTE, other, TAG, comm, &sts);
	 if (touch_rx) memcmp((void *)buf, (void *)buf, len);
      } else {
         if (touch_tx_always) memset((void *)buf, 0, len);
	 MPI_Send(buf, len, MPI_BYTE, other, TAG, comm);
      }
   }
}

void ping_pong(int iter, char *buf, char *spare, int len) {
   MPI_Status sts;
   int i,  other = (rank^1) & 1;

   for (i=0; i < iter; i++) {
      if (rank & 1) {
	 MPI_Recv(buf, len, MPI_BYTE, other, TAG, comm, &sts);
	 if (touch_rx) memcmp((void *)buf, (void *)buf, len);
         if (touch_tx_always) memset((void *)buf, 0, len);
	 MPI_Send(buf, len, MPI_BYTE, other, TAG, comm);
      } else {
         if (touch_tx_always) memset((void *)buf, 0, len);
	 MPI_Send(buf, len, MPI_BYTE, other, TAG, comm);
	 MPI_Recv(buf, len, MPI_BYTE, other, TAG, comm, &sts);
	 if (touch_rx) memcmp((void *)buf, (void *)buf, len);
      }
   }
}

void exchange(int iter, char *buf1, char *buf2, int len) {
   MPI_Status sts;
   int i,  other = (rank^1) & 1;

   for (i=0; i < iter; i++) {
      if (touch_tx_always) memset((void *)buf1, 0, len);
      MPI_Sendrecv(buf1, len, MPI_BYTE, other, TAG,
		   buf2, len, MPI_BYTE, other, TAG,
		   comm, &sts);
      if (touch_rx) memcmp((void *)buf2, (void *)buf2, len);
   }
}

void print_header(char *str) {
   int i;
   const char leadtxt[] = "Benchmark ";
   if (rank == root) {
      printf("%s%s\n", leadtxt, str);
      for (i=0; i < strlen(str)+strlen(leadtxt); ++i) printf("=");
      printf("\n");


      if (size == 2) {
	 printf("%14s %14s %14s %14s %14s\n",
		"lenght", "iterations", "elapsed time", "transfer rate", "latency");
	 printf("%14s %14s %14s %14s %14s\n",
		"(bytes)", "(count)", "(seconds)", "(Mbytes/s)", "(usec)");
	 printf("--------------------------------------------------------------------------\n");
      } else {
	 printf("%14s", "lenght");
	 for (i = 0; i < size; i += 2) printf(" %14s %14s",
					      "transfer rate", "latency");
	 printf("\n%14s", "(bytes)");
	 for (i = 0; i < size; i += 2) printf(" %14s %14s",
					      "(Mbytes/s)", "(usec)");

	 printf("\n");
	 for (i=0; i < 14 + 15*size; ++i) printf("-");
      printf("\n");
      }
      fflush(stdout);
   }
}

void measure(bench_rout_t rout, int scale, int sz) {
   int i, iter=0;
   static int          prev_iter = 0;
   static bench_rout_t prev_rout = NULL;
   char               *p1 = (char *)(align ? valloc : malloc)(sz),
		      *p2 = (char *)(align ? valloc : malloc)(sz);
   double              t,
                       tmin,
                       *s = (double *)malloc(size*sizeof(double));
   
   if (touch_tx_initially) memset((void *)p1, 0, sz);

   /*
    * Only spend time calculating a new iteration count if
    *    1) we have a new test function
    *       or
    *    2) we are not down to min_iter number of iterations yet
    *       or
    *    3) we are running with reverse lengths
    */

   if (prev_rout != rout || prev_iter > min_iter || reverse) {

      /* quick evaluation to find the correct no of iterations */
      for (iter=1; 1; iter+= iter) {
	 tmin = 9999.;
	 for (i=0; i < 3; ++i) {         /* minimum of 3 times seems good */
	    double ts;
	    rout(1, p1, p2, sz);
            /* Avoid distortion due to Nagel's algorithm */
	    MPI_Barrier(comm); 
	    t = MPI_Wtime();
	    rout(iter, p1, p2, sz);
	    MPI_Barrier(comm); 
	    t = MPI_Wtime() - t - tbar;
	    
	    MPI_Allreduce(&t, &ts, 1, MPI_DOUBLE, MPI_SUM, comm);
	    
	    t = 0.5*ts; /* MPI_Comm_size(comm) == 2 */
	    tmin =  (t < tmin) ? t : tmin;
	 }
	 if (tmin > 10.0*tres) break;
      }

      /* compute estimated no iterations to reach tgoal */
      iter = iter * (tgoal / tmin);
   }
   
   if (iter < min_iter) iter = min_iter; /* at least min_iter iterations */
   
   /* measure with one dry run first to get things settled in cache */
   rout(1 , p1, p2, sz);
   t = MPI_Wtime();
   rout(iter, p1, p2, sz);
   MPI_Barrier(comm);

   /* Normalize timing */
   t = (MPI_Wtime() - t) / (double)iter;
   
   MPI_Gather(&t, 1, MPI_DOUBLE, s, 1, MPI_DOUBLE, root, MPI_COMM_WORLD);
   
   if (rank == root) {
      if (size == 2) {
	 t = 0.5*(s[0] + s[1]);
	 if (t <= 0.0) t = tres;  /* avoid crash ... */
	 printf("%14d %14d %14.3f %14.3f %14.3f\n",
		sz, iter, t*iter,
		sz*1e-6*(double)scale/t,
		t*1e6/(double)scale);
      } else {
	 printf("%14d", sz);
	 for (i = 0; i < size; i += 2) {
	    t = 0.5*(s[i] + s[i+1]) / (double)scale;
	    if (t <= 0.0) t = tres;  /* avoid crash ... */
	    printf(" %14.3f %14.3f", sz*1e-6/t, t*1e6);
	 }
	 printf("\n");
      }
      fflush(stdout);

   }

   free(s);
   free(p2);
   free(p1);
   prev_iter = iter;
   prev_rout = rout;
}

      
bench_desc_t bench[] = { 
   { ping_ping, "ping-ping",              1, DO_PING_PING },
   { ping_pong, "ping-pong",              2, DO_PING_PONG },
   { exchange,  "exchange(MPI_Sendrecv)", 2, DO_EXCHANGE  },
   { NULL,      "none",      0 },
};


void do_usage(void) {
   if (rank == root) {
      printf("Usage: mpimon <mpimon switches> bandwidth [-a] [-b i|o|x] [-h] "
	     "[-m min] [-M max] [-t tgoal] [-V] [-z mask] -- hosta hostb\n");
   }
}

void do_help(void) {
  if (rank == root) {
    do_usage();
    printf(
"      -a           : Force page-aligned buffer allocation. Default off.\n"
"      -b <i|o|x>   : Benchmark to run, pIng-pIng, ping-pOng, or eXchange.\n"
"                     Default all.\n"
"      -h           : Prints this message\n"
"      -m <int>     : Minimum message length. Default 0.\n"
"      -M <int>     : Maximum message length. Default 16M.\n"
"      -r           : Reverse message lengths.\n"
"      -t <float>   : Specifies the goal for the elapsed time used per test.\n"
"                     Default %.1f\n"
"      -V           : Prints version.\n"
"      -z <mask>    : Buffer initialization/touch. Default 1.\n"
"          mask&1   : Initial initialization of send buffer.\n"
"          mask&2   : Initialization of send buffer before every send.\n"
"          mask&4   : Read receive buffer after message has been received.\n"
     , tgoal);
  }
}

      
void do_exit(int exit_code) {
   
   if (exit_code) {
      MPI_Abort(MPI_COMM_WORLD, exit_code);
   }
   MPI_Finalize();
   exit(exit_code);
}
      
   
int main(int argc, char **argv) {
#if defined(i386) || defined(x86_64)
   volatile unsigned long long register rsp __asm__("rsp"), xrsp;
   char *align = alloca(rsp % (1024*1024));
#endif
   extern char *optarg;
   int c, i, j, inc, bench_mask=0;
   int sz, msg_min=0, msg_max=16*1024*1024;
   double t1, t2, extreme;

   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   MPI_Comm_size(MPI_COMM_WORLD, &size);

   if (size & 1) {
      if (rank == root) fprintf(stderr,
         "Expected even number of processes, found oddly %d\n", size);
      do_exit(1);
   }
   if (size > 2) {
      MPI_Comm_split(MPI_COMM_WORLD, rank >> 1, rank & 1, &comm);
   }
   
   while ((c = getopt(argc, argv, "ab:hm:M:rt:Vz:")) != EOF) {
      switch (c) {
	 case 'b':
	    switch (*optarg) {
	       case 'i':
		  bench_mask|= DO_PING_PING;
		  break;
		  
	       case 'o':
		  bench_mask|= DO_PING_PONG;
		  break;
		  
	       case 'x':
		  bench_mask|= DO_EXCHANGE;
		  break;
		  
	       case '?':
		  do_usage();
		  do_exit(3);
		  break;
	    }
	    break;
	       
	 case 'h':
	    do_help();
	    do_exit(0);
	    break;

	 case 'm':
	    msg_min = atoi(optarg);
	    break;
	    
	 case 'M':
	    msg_max = atoi(optarg);
	    break;
	    
	 case 'r':
	    reverse = 1;
	    break;

	 case 't':
	    tgoal = atof(optarg);
	    break;

	 case 'V':
	    if (rank == root) printf("%s\n", MODULE_ID);
	    break;

	 case 'z':
	    touch_tx_initially = atoi(optarg) & 1;
	    touch_tx_always    = atoi(optarg) & 2;
	    touch_rx           = atoi(optarg) & 4;
	    break;

	 case '?':
	    do_usage();
	    do_exit(2);
      }
   }

   
   /* if no benchmarks have been specified, we default to all */
   if (!bench_mask) bench_mask = DO_PING_PING|DO_PING_PONG|DO_EXCHANGE;
   
   /* find clock resolution */
   for (j=0, extreme=-1; j<10; ++j) {
      for (t1=MPI_Wtime(); (t2=MPI_Wtime()) == t1;);
      t2 = t2 - t1;
      MPI_Allreduce(&t2, &tres, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
      if (tres > extreme) extreme=tres;
   }
   tres = extreme;

   
   for (j=1, tbar = -1; 1; j+= j) {
      MPI_Barrier(comm); 
      t1 = MPI_Wtime();
      for (i=0; i < j; ++i) {         /* minimum of 3 times seems good */
	 MPI_Barrier(comm);
      }
      t1 = MPI_Wtime() - t1;   
      MPI_Allreduce(&t1, &t2, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
      if (t2 > tbar) tbar=t2;
      if (t2 > 10.0*tres) break;
   }
   tbar = tbar / (double)j;
   
   if (rank == root) {
      printf("Resolution (usec): %f\n", tres*1e6);
   }
   
   for (j=0; bench[j].bench; ++j) {

      if (!(bench_mask & bench[j].mask)) continue;
	  
      print_header(bench[j].mnem);
      inc = reverse ? (msg_max>>1) : (msg_min>>2);
      for (sz=reverse?msg_max:msg_min;
	   reverse ? (sz >= msg_min) : (sz <= msg_max);
	   sz+= reverse ? -inc : inc) {

	 if (sz < 4) {
	    inc = 1;
	 } else {
	    if (!(sz & inc)) inc+= reverse ? -(inc/2) : inc; /* Got this? */
	 }

	 measure(bench[j].bench, bench[j].scale, sz);
      }
      if (rank == root) printf("\n\n");
   }
   do_exit(0);
   
   return 0;
}

	 
	 
	 
	 
	 
	 

