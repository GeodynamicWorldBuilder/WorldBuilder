(part:API_manual:chap:API_FFI:sec:python_interface)=
Python interface
===========

Before you can use the Python Interface, make sure your installation of the Worldbuilder includes SWIG and Python. See {ref}`part:user_manual:chap:installation:sec:installing_prerequisites`
for details.

The Python wrapper can be installed by running `python3 setup.py install --user` in the `lib` folder inside your build directory. It is helpful to install this into a virtual environment.

Afterwards, you should be able to `from gwb import WorldBuilderWrapper` inside a Python script.

See https://github.com/GeodynamicWorldBuilder/WorldBuilder/blob/main/examples/python/python_example.py for an example:

:::{literalinclude} ../../../../examples/python/python_example.py
:language: Python
:::
