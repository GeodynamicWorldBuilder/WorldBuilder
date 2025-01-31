(part:API_manual:chap:API_FFI:sec:Fortran_interface:subsec:CHIC_integration)=
CHIC Integration
=================

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