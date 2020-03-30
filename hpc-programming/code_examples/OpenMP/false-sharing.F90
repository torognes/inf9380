!
! Program to show some false sharing problems
! Written by Ole W. Saastad, USIT
!
!
! types and data
!

  module data 
! types 
! Static allocation for simplicity.
!
    implicit none
    integer, parameter :: r8 = selected_real_kind(15,307)
    integer, parameter :: i8 = selected_int_kind(12)
  end module data 


  program sharing
    use data
    implicit none
    integer(i8) :: i,j,k
    integer(i8), parameter ::m=THREADS, n=100000 !  -DTHREADS=4 at compile time
    real(r8) :: a(m,n), s(m)
    real(r8) :: t0
    real(r8), external :: mysecond

    a=0.0_r8


    t0=mysecond()
    do k=1,1000

!$omp parallel private(i,j) shared(a,s) 
       do i=1,m
          s(i)=0.0
          do j=1,n
             s(i)=s(i)+a(i,j)
          enddo
       enddo
!$omp end parallel 

    enddo
    
    write(*,*) "It took",mysecond()-t0,"seconds, ","s(1):",s(1)



  end program sharing

