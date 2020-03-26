## Load packages and data:
library(parallel)
library(pls)
data(gasoline)

## Set up number of workers:
numWorkers <- as.numeric(Sys.getenv("SLURM_NTASKS"))

## Define worker function and vector to apply it to
workerFunc <- function(ind) {
    kernelpls.fit(gasoline$NIR[-ind, ,drop=FALSE],
                  gasoline$octane[-ind], ncomp = 50, stripped = TRUE)
}

indices <- 1:nrow(gasoline)

## Do the calculation
res <- mclapply(indices, workerFunc, mc.cores = numWorkers)

## ... Do some more calculations ...

## Save results:
save.image(file = "par_mclapply.RData")
