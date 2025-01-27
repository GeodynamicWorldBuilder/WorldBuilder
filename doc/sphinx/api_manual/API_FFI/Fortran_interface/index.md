(part:API_manual:chap:API_FFI:sec:Fortran_interface:subsec:Fortran_integration)=
FORTRAN interface
===========

The interface functions are defined in the Fortran wrapper module (`source/world_builder/wrapper_fortran.f90`). Here we provide short explanations for each function (subroutines in Fortran).

```{important}
To start the World Builder and create a world from a given `.wb` file, the `create_world` subroutine always has to be called first.
```

```{code}
subroutine create_world(cworld, file_name, has_output_dir, output_dir, random_number_seed)

Interface with the World Builder to create a world with a given `.wb` file

Parameters:
    cworld (C_PTR) - Variable that points to the world pointer of the world builder
    file_name (C_CHAR) - Name of the `.wb` file to be used
    has_output_dir (C_BOOL) - If true, the World Builder gains permission to write files into the given directory (`output_dir`). Currently, these are the World Builder json schema and declarations file.
    output_dir (C_CHAR) - Path to directory where the files should be written into.
    random_number_seed (C_LONG) - Random seed used in creating the world 
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

```{important}
When done with the World Builder, the `release_world` subroutine has to be called to clean up the memory used by the World Builder.
```

```{hint}
A short explanation of how the World Builder is integrated into the Mantle Convection code CHIC can be found on the next page {ref}`part:API_manual:chap:API_FFI:sec:Fortran_interface:subsec:CHIC_integration`
```

<!-- With the sphinx Fortran extension, the following syntax would be possible
PARAMETERS
.. f:subroutine:: temperature_2d(cworld, x, z, depth, temperature)

    Interface with 2D temperature C function of the World builder. Returns 

    :param a: My parameter. -->