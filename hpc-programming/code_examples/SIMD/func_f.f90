
module func

  contains

    function f(x)
!$omp declare simd(f) 
      real(8) :: f
      real(8) :: x
      
      f = (4/(1+x*x))

    end function f
end module func
