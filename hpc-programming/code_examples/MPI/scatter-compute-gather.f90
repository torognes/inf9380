program compute
  include 'mpif.h'

  integer, parameter :: SIZE=1000000, NRANKS=4
  integer :: myrank, ranks, ierr
  integer :: i, j
  real :: a(NRANKS,SIZE), b(SIZE)
  logical :: master 

  call MPI_Init(ierr)
  call MPI_Comm_Rank(MPI_COMM_WORLD, myrank, ierr)
  call MPI_Comm_Size(MPI_COMM_WORLD, ranks, ierr)
  master = (myrank == 0)
  if ((ranks /= NRANKS) .and. master) then
     write(*,*) 'Need exactly ',NRANKS,' ranks, Aborting'
     call MPI_Abort(MPI_COMM_WORLD, ierr)
  endif


  if (master) then
     call srand(171717)
     do i=1,NRANKS
        do j=1,SIZE
           a(i,j)=rand()
        enddo
     enddo
     write(*,*) 'Initial data, to be distributed to all ranks'
     write(*,fmt='(4(F10.6))') a(1:4,1:SIZE:SIZE/NRANKS)
  endif

! Scatter the data to all ranks from master (rank 0)
  call MPI_Scatter(a, SIZE, MPI_REAL, b, SIZE, MPI_REAL, 0, MPI_COMM_WORLD, ierr)


! Do some distributed work, on each rank, each rank to this computation
! totally independent of the other ranks.

! Data for rank 0 should be the same as line 1 in the a matrix  
  write(*,'(a,i2,a,4(f9.6))') 'Myrank',myrank,' b(1:4)',b(1:4)

! This is vector syntax, do like j=1,10 b(j)=b(j)*10 . This is the parallel work.
  b=b*10

! Collect the distributed parallel computation back to master (rank 0).  
  call MPI_Gather(b, SIZE, MPI_REAL, a, SIZE, MPI_REAL, 0, MPI_COMM_WORLD, ierr)

  if (master) then
     write(*,*) 'Received processed data from all ranks'
     write(*,fmt='(4(F10.6))') a(1:4,1:SIZE:SIZE/NRANKS)
  endif
  

  call MPI_Finalize(ierr)
end program compute

