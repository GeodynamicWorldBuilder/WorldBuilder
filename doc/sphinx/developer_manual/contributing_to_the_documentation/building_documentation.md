(part:dev_manual:chap:contribute_to_doc:sec:building_doc)=
Building documentation

To build the documentation locally, you first need to ensure you have the right python packages installed. The list of packages required to build the documentation is located in `$WORLD_BUILDER_SOURCE_DIR/doc/requirements.txt`, and can be installed using 

:::{code-block}
pip install -r $WORLD_BUILDER_SOURCE_DIR/doc/requirements.txt \\
:::

Now that the python packages are installed, we need to toggle the option in cmake to build the target for the GWB documention, this can be done using `ccmake` and enabling `WB_BUILD_DOCUMENTATION=ON`. Run the following command, and use the interactive GUI to do this:

:::{code-block}
cd $WORLD_BUILDER_SOURCE_DIR/build \\
ccmake .
:::
Now we compile the GWB normally by running the following:

:::{code-block}
cmake .. && make
:::

This adds the target for the documentation in `$WORLD_BUILDER_SOURCE_DIR/build/doc`, to finish building the documentation run the final commands:

:::{code-block}
cd $WORLD_BUILDER_SOURCE_DIR/build/doc \\
make manual
:::

This compiles the sphinx documentation, which will be built in  `$WORLD_BUILDER_SOURCE_DIR/build/doc/sphinx`.
