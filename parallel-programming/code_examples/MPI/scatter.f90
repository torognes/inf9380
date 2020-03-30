program mpiscatter
  include 'mpif.h'

  integer SIZE
! Set SIZE to number of ranks used, in this example 4 ranks is used,
! if you change SIZE you need to change the format statement also.  
  parameter(SIZE=4)
  integer numtasks, rank, sendcount, recvcount, source, ierr
  real*4 sendbuf(SIZE,SIZE), recvbuf(SIZE)
  
  data sendbuf /1.0,  2.0,  3.0,  4.0,&
                5.0,  6.0,  7.0,  8.0,&
                9.0, 10.0, 11.0, 12.0,& 
               13.0, 14.0, 15.0, 16.0 /
! Only the sendbuffer contains data, recvbuf is empty
  
  call MPI_INIT(ierr)
  call MPI_COMM_RANK(MPI_COMM_WORLD, rank, ierr)
  call MPI_COMM_SIZE(MPI_COMM_WORLD, numtasks, ierr)
  
  if ((numtasks /=  SIZE) .and. (rank==0)) then
     write(*,*) 'Need exactly ',SIZE,' ranks,',' Aborting'
     call MPI_Abort(MPI_COMM_WORLD, ierr)
  endif
  
  source = 0
  sendcount = SIZE
  recvcount = SIZE
  
  call MPI_SCATTER(sendbuf, sendcount, MPI_REAL, recvbuf, recvcount,&
                   MPI_REAL, source, MPI_COMM_WORLD, ierr)

! After scatter all rank's recvbuf is filled with 4 values.     
  write(*,'(a,i2,a,4(f5.1))') 'rank= ',rank,' Results: ',recvbuf 
  
  call MPI_FINALIZE(ierr)

end program mpiscatter
