
! Simple program to compute pi using 4 * atan(1) series 
! to illustrate usage of multi thread.

program computepi
  integer, parameter :: r8  = selected_real_kind(p=15,r=307)
  integer, parameter :: i8 = selected_int_kind(16)
  integer(i8) :: n, i
  real(r8) ::  sum, pi, x, h
  real(r8) :: f
  
  n = 5000000000_i8
  h = 1.0_r8/n
  sum = 0.0_r8
!$omp parallel do simd reduction(+:sum)  
!This OpenMP inform the compiler to generate a multi threaded loop
  do i = 1,n
     x = h*(i-0.5_r8)
     sum = sum + (4.0_r8/(1.0_r8+x*x))
  enddo
  pi = h*sum
  write(*,*)"Pi : ",pi
end program computepi

