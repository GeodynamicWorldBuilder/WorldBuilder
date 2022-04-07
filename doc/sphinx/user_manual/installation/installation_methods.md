Other installation methods
=======

Choose how you would like to install: as part of a...

::::::{tab-set}

:::::{tab-item} cpp program

For this case, there are two options. Either compile the library and link it, or directly compile the library source files within your project. Compiling the Geodynamic World Builder separately and linking it through the CPP wrapper is the recommended option.

::::{tab-set}
:::{tab-item} Compile the library and link it
First, follow the instructions from {ref}`sec:stand-alone-install`.
The library can be found in the `build/lib` directory with the name `libWorldBuilder.a`.
Link your project with this file.

The only file you need to include in your code is `world_builder/wrapper_cpp.h`. Initialize a variable of type `wrapper_cpp::WorldBuilderWrapper` with a valid std::string pointing to the world builder file. Then implement in the correct locations the temperature and composition querying functions. It is also possible to directly use the World class in the World Builder, by including `world_builder/world.h`.

:::

:::{tab-item} Directly compile the library source files within your project
(implemented-in-aspect)=

This is the way it is implemented in ASPECT.
Although it is possible to just copy the library source and head directory files, it is recommended to clone the whole world builder project into a directory in your project (possibly as a git submodule).
This way the World Builder can be easily updated. Add the World Builder files to cmake/make/compiler.

The only file you need to include in your code is `world_builder/world.h`.
Initialize a variable of type `WorldBuilder::World` with a valid std::string pointing to the world builder file.
Then implement in the correct locations the temperature and composition querying functions.

:::
::::

:::::

:::::{tab-item} c program
First, follow the instructions from {ref}`sec:stand-alone-install`. The library can be found in the `build/lib/` directory with the name `libWorldBuilder.a`. Link your project with this file.

The only file you need to include in your code is `world_builder/wrapper_c.h`.
Create a void variable which is a pointer to a pointer and set it so NULL (e.g. `void **ptr_ptr_world`), and create a pointer to a c-string (e.g. `char *world_builder_file`).
Pass these variables to the `create_world` function.
This function will create the World Builder world.
Now this pointer can be used to call the temperature and composition quering functions.

```{important}
When done with the World Builder, call the `release_world` function.
This will clean up the memory used by the world builder.
```

:::::

:::::{tab-item} fortran program
First, follow the instructions from {ref}`sec:stand-alone-install`.
The library can be found in the `build/mod/` directory with the name `worldbuilder.mod`.
Link your project with this file.
Include the module into your project. The only thing you need to care for, when creating the world is to provide file name which ends with `//C_NULL_CHAR`.
Then call the `create_world` function with the variable cworld and the file name variable.
The Fortran module takes care of the world pointer internally.
When the World Builder world is created, the temperature and composition functions can be called at will.

```{important}
When done with the World Builder, call the `release_world` function.
This will clean up the memory used by the world builder.
```

To be more clear, we show here an example Fortran program using the Geodynamic World Builder.

```{todo}
add figure and caption pg 20
```
:::::

:::::{tab-item} Python program
You only need to include the module called gwb.
To be more clear, we show here a python example using the Geodynamic World Builder.
```{todo}
add figure and caption pg 21
```

:::::

:::::{tab-item} ASPECT
The Geodynamic World Builder is installed by default in ASPECT starting with the release 2.2.0. If you want to use a newer version of the Geodynamic World Builder in ASPECT you can point ASPECT to the location of the World Builder directory.

```{important}
The Geodynamic World Builder is in version 2.1.0 implemented in ASPECT as a submodule, with a method similar to what is described in under the tab [`cpp program` &#8594; `Directly compile the library source files within your project`](implemented-in-aspect), where ASPECT can be directly compiled with the World Builder source files.
The only thing you have to do is make sure that the submodule is actually loaded.

If the World Builder submodule is initialized, ASPECT's cmake configuration will automatically find it and use it. When cloning the ASPECT repository, add the `-recursive` flag (e.g. `git clone -recursive git@github.com:geodynamics/aspect.git`) to automatically initialize the git submodule.
If you already cloned ASPECT, use the command `git submodule update -init -recursive`.
```

When ASPECT has been successfully compiled with the World Builder, set the ASPECT global input parameter `World builder file` to the World Builder file location.
:::::
::::::
