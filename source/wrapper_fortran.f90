MODULE WorldBuilder
USE, INTRINSIC :: ISO_C_BINDING!, ONLY: C_PTR
  IMPLICIT NONE

  ! Create an interface with the create world function
  INTERFACE
    SUBROUTINE create_world(cworld, file_name) BIND(C, NAME='create_world')
      USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR
      IMPLICIT NONE
      ! This argument is a pointer passed by reference.
      TYPE(C_PTR), INTENT(OUT) :: cworld
      character(len=1),  intent(in)  :: file_name
    END SUBROUTINE create_world

    ! Create an interface with the 2d tempearture function of the World builder
    SUBROUTINE temperature_2d(cworld, x, z, depth, gravity, temperature) BIND(C, NAME='temperature_2d')
      USE, INTRINSIC :: ISO_C_BINDING
      IMPLICIT NONE
      ! This argument is a pointer passed by value.
      TYPE(C_PTR), INTENT(IN), VALUE :: cworld
      REAL(C_DOUBLE), intent(in), value :: x
      REAL(C_DOUBLE), intent(in), value :: z
      REAL(C_DOUBLE), intent(in), value :: depth
      REAL(C_DOUBLE), intent(in), value :: gravity
      REAL(C_DOUBLE), intent(out) :: temperature
    END SUBROUTINE temperature_2d

    ! Create an interface with the 3d tempearture function of the World builder
        SUBROUTINE temperature_3d(cworld, x, y, z, depth, gravity, temperature) BIND(C, NAME='temperature_3d')
      USE, INTRINSIC :: ISO_C_BINDING
      IMPLICIT NONE
      ! This argument is a pointer passed by value.
      TYPE(C_PTR), INTENT(IN), VALUE :: cworld
      REAL(C_DOUBLE), intent(in), value :: x
      REAL(C_DOUBLE), intent(in), value :: y
      REAL(C_DOUBLE), intent(in), value :: z
      REAL(C_DOUBLE), intent(in), value :: depth
      REAL(C_DOUBLE), intent(in), value :: gravity
      REAL(C_DOUBLE), intent(out) :: temperature
    END SUBROUTINE temperature_3d

    ! Create an interface with the 2d composition function of the World builder
        SUBROUTINE composition_2d(cworld, x, z, depth, composition_number, composition) BIND(C, NAME='composition_2d')
      USE, INTRINSIC :: ISO_C_BINDING
      IMPLICIT NONE
      ! This argument is a pointer passed by value.
      TYPE(C_PTR), INTENT(IN), VALUE :: cworld
      REAL(C_DOUBLE), intent(in), value :: x
      REAL(C_DOUBLE), intent(in), value :: z
      REAL(C_DOUBLE), intent(in), value :: depth
      INTEGER(C_INT), intent(in), value :: composition_number
      LOGICAL(C_BOOL), intent(out) :: composition
    END SUBROUTINE composition_2d

    ! Create an interface with the 3d composition function of the World builder
      SUBROUTINE composition_3d(cworld, x, y, z, depth, composition_number, composition) BIND(C, NAME='composition_3d')
      USE, INTRINSIC :: ISO_C_BINDING
      IMPLICIT NONE
      ! This argument is a pointer passed by value.
      TYPE(C_PTR), INTENT(IN), VALUE :: cworld
      REAL(C_DOUBLE), intent(in), value :: x
      REAL(C_DOUBLE), intent(in), value :: y
      REAL(C_DOUBLE), intent(in), value :: z
      REAL(C_DOUBLE), intent(in), value :: depth
      INTEGER(C_INT), intent(in), value :: composition_number
      LOGICAL(C_BOOL), intent(out) :: composition
    END SUBROUTINE composition_3d

    ! Create an interface with the release world function.
    SUBROUTINE release_world(cworld) BIND(C, NAME='release_world')
      USE, INTRINSIC :: ISO_C_BINDING, ONLY: C_PTR
      IMPLICIT NONE
      ! This argument is a pointer passed by value.
      TYPE(C_PTR), INTENT(IN), VALUE :: cworld
    END SUBROUTINE release_world
  END INTERFACE

  TYPE(C_PTR) :: cworld
  END MODULE WorldBuilder
