
program hello
  use data
  implicit none  
  include "mpif.h"
  integer :: ierr,ranks,myrank
  logical :: master


  call MPI_Init(ierr) 
  if (ierr/=0) call MPI_Abort(MPI_COMM_WORLD, ierr) 
  
  call MPI_Comm_size(MPI_COMM_WORLD,ranks,ierr)
  if (ierr/=0) call MPI_Abort(MPI_COMM_WORLD, ierr) 
  
  call MPI_Comm_rank(MPI_COMM_WORLD,myrank,ierr)
  if (ierr/=0)  call MPI_Abort(MPI_COMM_WORLD, ierr)
  master=(myrank==0)

  if (master) then
     write(*,fmt='(a,1x,i3)') "I am master, and my rank is:",myrank
  endif
  write(*,fmt='(a,1x,i3,1x,a,1x,i3)') "Myrank is number ", &
        myrank,"of total ranks", ranks

  call MPI_Barrier(MPI_COMM_WORLD,ierr)
  if (ierr/=0)  call MPI_Abort(MPI_COMM_WORLD, ierr)

  call MPI_Finalize(ierr)
  if (ierr/=0)  call MPI_Abort(MPI_COMM_WORLD, ierr)

end program hello
