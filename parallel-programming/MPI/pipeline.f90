program pipeline
  implicit none  
  include "mpif.h"
  integer :: ierr, stat(5), ranks, myrank
  integer :: tag=1, send_to, rec_from
  logical :: master
  character(len=32) :: arg
  character(len=32) :: form
  real :: a
  
  
  call MPI_Init(ierr) 
  if (ierr/=0) call MPI_Abort(MPI_COMM_WORLD, ierr) 

  call MPI_Comm_size(MPI_COMM_WORLD,ranks,ierr)
  if (ierr/=0) call MPI_Abort(MPI_COMM_WORLD, ierr) 
  
  call MPI_Comm_rank(MPI_COMM_WORLD,myrank,ierr)
  if (ierr/=0)  call MPI_Abort(MPI_COMM_WORLD, ierr)
  master=(myrank==0)

  if (master) then
     if (ranks<4) then
        write(*,*) "To few ranks, 4 is minimum"
        call mpi_abort(MPI_COMM_WORLD, ierr)
     endif
     if (command_argument_count() < 1) then
        write(*,*) "Missing argument"
        call mpi_abort(MPI_COMM_WORLD, ierr)
     endif
  endif

! Check the variable a which is private to each rank, no sharing
    
  form='(a,1x,i3,1x,a,1x,f8.2)'
  select case (myrank)
    case (0)
      call get_command_argument(1, arg)
      read(arg,*) a
      write(*,*) "Master read number, sending a=",a
      send_to=1
      call mpi_send(a, 1, MPI_REAL, send_to, tag, MPI_COMM_WORLD, ierr)
      
   case (1)
      rec_from=0
      call mpi_recv(a, 1, MPI_REAL, rec_from, tag, MPI_COMM_WORLD, stat, ierr)
      write(*,fmt=form) "myrank ",myrank," Received a=", a
      a=a/2.0
      send_to=2
      call mpi_send(a, 1, MPI_REAL, send_to, tag, MPI_COMM_WORLD, ierr)
      
   case (2)
       rec_from=1
       call mpi_recv(a, 1, MPI_REAL, rec_from, tag, MPI_COMM_WORLD, stat, ierr)
       write(*,fmt=form) "myrank ",myrank," Received a= ", a
       a=sqrt(a)
       send_to=3
       call mpi_send(a, 1, MPI_REAL, send_to, tag, MPI_COMM_WORLD, ierr)
       
    case (3)
       rec_from=2
       call mpi_recv(a, 1, MPI_REAL, rec_from, tag, MPI_COMM_WORLD, stat, ierr)
       write(*,fmt=form) "myrank ",myrank," Received a= ",a       
       
    case default
      write(*,fmt='(a,1x,i3,1x,a)') "myrank ",myrank," no work for me"
  end select


  call MPI_Barrier(MPI_COMM_WORLD,ierr)
  if (ierr/=0)  call MPI_Abort(MPI_COMM_WORLD, ierr)

  call MPI_Finalize(ierr)
  if (ierr/=0)  call MPI_Abort(MPI_COMM_WORLD, ierr)

end program pipeline

