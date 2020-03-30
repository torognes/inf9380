function f(x)
  integer, parameter :: r8  = selected_real_kind(p=15,r=307)
  real(r8) :: f
  real(r8) :: x

  f = (4.0_r8/(1.0_r8+x*x))
end function f

