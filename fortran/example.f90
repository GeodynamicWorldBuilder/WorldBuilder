program test
  USE, INTRINSIC :: ISO_C_BINDING
  IMPLICIT NONE
  INTERFACE
    SUBROUTINE create_world(file_name, world) BIND(C, NAME='create_world')
    USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR
      IMPLICIT NONE
      character(len=1),  intent(in)  :: file_name
      TYPE(C_PTR),  intent(out) :: world
    END SUBROUTINE
  END INTERFACE
    INTERFACE
    SUBROUTINE release_world(world) BIND(C, NAME='release_world')
    USE, INTRINSIC :: ISO_C_BINDING
      IMPLICIT NONE
      TYPE(C_PTR),  intent(in), value :: world
    END SUBROUTINE
  END INTERFACE
      INTERFACE
    SUBROUTINE temperature_2d(x,z,depth,gravity,temperature,world) BIND(C, NAME='temperature_2d')
    USE, INTRINSIC :: ISO_C_BINDING
      IMPLICIT NONE
      REAL(C_DOUBLE), intent(in), value :: x
      REAL(C_DOUBLE), intent(in), value :: z
      REAL(C_DOUBLE), intent(in), value :: depth
      REAL(C_DOUBLE), intent(in), value :: gravity
      REAL(C_DOUBLE), intent(out) :: temperature
      TYPE(C_PTR),  intent(in), value :: world
    END SUBROUTINE
  END INTERFACE

  TYPE(C_PTR) :: cworld
  !procedure (func), pointer :: f_ptr
  !CHARACTER(C_CHAR), POINTER :: fchararray(:)
  character(len=256) :: file_name = "../build/wb5.json"//C_NULL_CHAR
    REAL*8 :: temperature,x,z,depth,gravity = 2

  write(*, *) 'point in fortan 1 = ', cworld
  CALL create_world( file_name, cworld )
  write(*, *) 'point in fortan 2 = ', cworld

  CALL temperature_2d(x,z,depth,gravity,temperature,cworld)
  write(*, *) 'temp 2 = ', temperature
  !CALL C_F_POINTER( cworld, fworld)
  CALL release_world(cworld)

  !WRITE (*, *) 'fchararray',fchararray
END program
