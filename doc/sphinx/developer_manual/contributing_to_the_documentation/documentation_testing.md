(part:dev_manual:chap:contribute_to_doc:sec:doc_testing)=
Documentation testing
=====================

```{todo}
Explain how the documentation is tested in each pull request. It is build and (todo) all the world builder input files are run (checking for errors, not comparing output).
```
:::{code-block}
:lineno-start: 1
:emphasize-lines: 3,4
 {
    "version":0.6.0,
    "coordinate system": {"model":"cartesian"}, 
 }
:::

To build the documentation locally, you first need to ensure you have the right python packages installed. The list of packages required to build the documentation is located in `$WORLD_BUILDER_SOURCE_DIR/doc/requirements.txt`, and can be installed using `pip install -r $WORLD_BUILDER_SOURCE_DIR/doc/requirements.txt`.  Now, run `ccmake .` in `$WORLD_BUILDER_SOURCE_DIR/build` and enable `WB_BUILD_DOCUMENTATION`. Compile the GWB normally by running `cmake .. && make` in `$WORLD_BUILDER_SOURCE_DIR/build`. Navigate to `$WORLD_BUILDER_SOURCE_DIR/build/doc` and run `make manual` to compile the sphinx documentation, which will be built in  `$WORLD_BUILDER_SOURCE_DIR/build/doc/sphinx`.