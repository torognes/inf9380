workerFunc <- function(n) { return(n^2) }
values <- 1:100

library(parallel)

## Number of workers (R processes) to use:
numWorkers <- 8

## Set up the 'cluster'
cl <- makeCluster(numWorkers, type = 'MPI')

## Parallel calculation (parLapply):
res <- parLapply(cl, values, workerFunc)

## Shut down cluster
stopCluster(cl)
Rmpi::mpi.finalize()  # or Rmpi::mpi.quit(), which quits R as well

print(unlist(res))
