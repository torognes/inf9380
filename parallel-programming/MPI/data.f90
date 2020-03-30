module data
  implicit none
! Constants for data types                                                                               
  integer, parameter :: r4  = selected_real_kind(p=6,r=20)
  integer, parameter :: r8  = selected_real_kind(p=15,r=307)
  integer, parameter :: i1 = selected_int_kind(2)
  integer, parameter :: i2 = selected_int_kind(4)
  integer, parameter :: i4 = selected_int_kind(8)
  integer, parameter :: i8 = selected_int_kind(16)

! Math constants                                                                                         
  real(r8), parameter :: pi8 = 4.0_r8 * atan(1.0_r8)
  real(r8), parameter :: e8 = exp(1.0_r8)
end module data

