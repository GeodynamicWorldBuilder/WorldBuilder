(part:API_manual:chap:API_FFI:sec:Fortran_interface)=
FORTRAN interface
===========

### General explanation

The interface functions are defined in the Fortran wrapper module (`source/world_builder/wrapper_fortran.f90`). Here we provide short explanations for each function (subroutines in Fortran).

```{important}
To start the World Builder and create a world from a given `.wb` file, the `create_world` subroutine always has to be called first.
```

```{code}
subroutine create_world(cworld, file_name, has_output_dir, output_dir, random_number_seed)
Interface with the World Builder to create a world with a given `.wb` file

PARAMETERS
    cworld (C_PTR) - variable that points to the world pointer of the world builder
    file_name (string) - name of the `.wb` file to be used
    has_output_dir
    output_dir
    random_number_seed (long) - random seed used in creating the world 
```

After calling `create_world`, the world builder can be queried to output temperature and composition values at a given position. The subroutines for temperature and composition, each have a 2D and 3D variant. Each subroutine creates an interface with a c function of the world builder of the same name.

```{code}
temperature_2d(cworld, x, z, depth, temperature)

temperature_3d(cworld, x, y, z, depth, temperature)

composition_2d(cworld, x, z, depth, composition_number, composition)

composition_3d(cworld, x, y, z, depth, composition_number, composition)
```

Input arguments:

1. `cworld`
    - variable that points to the world created by `create_world`
2. `x, y, z, depth`
    - coordinates where the user wants to query the world builder for a value
    - `z` and `depth` need to be consistently given
    - `y` is only used in the 3D variants
3. `composition_number` (only for composition)
    - give the integer that matches a composition, as given in the used `.wb` file
    - explanation on compositions outlined in {ref}`part:user_manual:chap:basic_starter_tutorial:sec:index`

Output:

- value (of temperature or composition) at the position specified with x, (y,) z, depth

### Example (CHIC)

To illustrate how the World Builder interface can be used, the following section outlines what is done in CHIC:

1. **Initialize the World Builder**:
    ```fortran
    call create_world(cworld, 'path/to/world_file.wb', .false., '', 12345)
    ```
    - the path to the `.wb` file has to be passed as an input to CHIC

2. **Query Temperature and Composition**:
    - pass domain boundaries as input to CHIC (analogous to the `.grid` files used for the gwb-grid app {ref}`part:user_manual:chap:how_to_use_the_apps:sec:gwb-grid_app`)
    - parse grid in given boundaries
    - overwrite values of temperature and composition at each position:
    ```fortran
    ! Query temperature at the given position
    call temperature_3d(cworld, x, y, z, depth, temperature)
    ! Query composition at the given position
    call composition_3d(cworld, x, y, z, depth, composition_number, composition)
    ```
3. **Release the World Builder**:
    ```fortran
    call release_world(cworld)
    ```

```{important}
The `release_world` subroutine has to be called afterwards, in order to clean up the memory used by the World Builder.
```

<!-- With the sphinx Fortran extension, the following syntax would be possible
PARAMETERS
.. f:subroutine:: temperature_2d(cworld, x, z, depth, temperature)

    Interface with 2D temperature C function of the World builder. Returns 

    :param a: My parameter. -->