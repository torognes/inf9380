## Load packages and data:
library(parallel)
library(pls)
data(gasoline)

## Set up number of workers:
numWorkers <- as.numeric(Sys.getenv("SLURM_NTASKS")) - 1

## Start and prepare cluster
cl <- makeCluster(numWorkers, type = "MPI")
ignore <- clusterEvalQ(cl, library(pls)) # Runs library(pls) on all workers
clusterExport(cl, list("gasoline"))

## Define worker function and vector to apply it to
workerFunc <- function(ind) {
    kernelpls.fit(gasoline$NIR[-ind, ,drop=FALSE],
                  gasoline$octane[-ind], ncomp = 50, stripped = TRUE)
}

indices <- 1:nrow(gasoline)

## Do the calculation
res <- parLapply(cl, indices, workerFunc)

## ... Do some more calculations ...

## Stop the cluster:
stopCluster(cl)
Rmpi::mpi.finalize()

## Save results:
save.image(file = "par_mpi.RData")
