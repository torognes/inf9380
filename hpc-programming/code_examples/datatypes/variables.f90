
module data 
  implicit none
! Constants for data types
  integer, parameter :: r4  = selected_real_kind(p=6,r=20)
  integer, parameter :: r8  = selected_real_kind(p=15,r=307)
  integer, parameter :: r16 = selected_real_kind(p=27,r=2400)
  integer, parameter :: i1 = selected_int_kind(2)
  integer, parameter :: i2 = selected_int_kind(4)
  integer, parameter :: i4 = selected_int_kind(8)
  integer, parameter :: i8 = selected_int_kind(16)

! Math constants
  real(r8), parameter :: pi8 = 4.0_r8 * atan(1.0_r8)
  real(r8), parameter :: e8 = exp(1.0_r8)
  real(r16),parameter :: pi16 = 4.0_r16 * atan(1.0_r16)
  real(r16),parameter :: e16 = exp(1.0_r16)
  complex(r4),parameter  :: j4  = (0.5,1)
  complex(r8),parameter  :: j8  = (0.5,1)
  complex(r16),parameter :: j16 = (0.5,1)
end module data
 

program variables 
  use data

  logical(i1) :: ll1
  logical(i2) :: ll2
  logical(i4) :: ll4
  logical(i8) :: ll8
  integer(i1) :: ii1
  integer(i2) :: ii2
  integer(i4) :: ii4
  integer(i8) :: ii8
  real(r4)    :: rr4
  real(r8)    :: rr8
  real(r16)   :: rr16
  complex(r4) :: cc4
  complex(r8) :: cc8
  complex(r16):: cc16
  
  character*20   :: form

  print *,pi8
  print *,pi16
  print *,e8
  print *,e16
  print *,j4
  print *,j8

  print *,"i2 i4 i8", i2, i4, i8
  print *,"r4 r8 r16", r4, r8, r16

  form="(a,1x,i2,1x,a)"

  write(*,fmt=form) "Sizeof l1", sizeof(ll1),"bytes"
  write(*,fmt=form) "Sizeof l2", sizeof(ll2),"bytes"
  write(*,fmt=form) "Sizeof l4", sizeof(ll3),"bytes"
  write(*,fmt=form) "Sizeof l8", sizeof(ll8),"bytes"
  write(*,fmt=form) "Sizeof i1", sizeof(ii1),"bytes"
  write(*,fmt=form) "Sizeof i2", sizeof(ii2),"bytes"
  write(*,fmt=form) "Sizeof i4", sizeof(ii4),"bytes"
  write(*,fmt=form) "Sizeof i8", sizeof(ii8),"bytes"
  write(*,fmt=form) "Sizeof r4", sizeof(rr4),"bytes"
  write(*,fmt=form) "Sizeof r8", sizeof(rr8),"bytes"
  write(*,fmt=form) "Sizeof r16",sizeof(rr16),"bytes"
  write(*,fmt=form) "Sizeof c4", sizeof(cc4),"bytes"
  write(*,fmt=form) "Sizeof c8", sizeof(cc8),"bytes"
  write(*,fmt=form) "Sizeof c16",sizeof(cc16),"bytes"

  print *, "maxint i1",huge(ii1)
  print *, "maxint i2",huge(ii2)
  print *, "maxint i4",huge(ii4)
  print *, "maxint i8",huge(ii8)
  
  print *, "Precision and range r4 ",precision(rr4), range(rr4)
  print *, "Precision and range r8 ",precision(rr8), range(rr8)
  print *, "Precision and range r16",precision(rr16), range(rr16)

  print *, "Complex parts"
  print *,REALPART(j4),IMAGPART(j4)
  print *,REALPART(j8),IMAGPART(j8)
  print *,REALPART(j16),IMAGPART(j16)
  
! The following are Fortran 2008 standard.
!  print *,j4%re, j4%im
!  print *,j8%re, j8%im
!  print *,j16%re, j16%im

end program variables 
