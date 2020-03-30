function f(x)
!$omp declare simd(f)
!This OpenMP-SIMD derective inform the compiler that this is
!a vectorisable function, where x can be a vector register.
 
  integer, parameter :: r8  = selected_real_kind(p=15,r=307)
  real(r8) :: f
  real(r8) :: x

  f = (4.0_r8/(1.0_r8+x*x))
end function f
