program main
  implicit none
  integer*4        :: myid, nproc, ierr, n, i, j, stat(5), sender, receiver
  include 'mpif.h'

  call mpi_init(ierr)
  call mpi_comm_rank(MPI_COMM_WORLD, myid, ierr)
  call mpi_comm_size(MPI_COMM_WORLD, nproc, ierr)
  receiver = 0
  do i = 1, 10
     sender = mod(i-1,nproc-1)+1 ! skip 0 as sender
     if(myid == receiver) then
        call mpi_recv(j, 1, MPI_INTEGER, sender, 0, MPI_COMM_WORLD, stat, ierr)
        write(*,*) i, j, sender
     elseif(myid == sender) then
        call mpi_send(i, 1, MPI_INTEGER, receiver, 0, MPI_COMM_WORLD, ierr)
     end if
  end do
  call mpi_finalize(ierr);
end program
