(part:user_manual:chap:concepts:sec:02_first_input_file)=
Your first input file
=====================

GWB input files are plain-text files, which generally have the extension `.wb`. At minimum, the world builder input file should contain a object (indicated by `{}` in JSON) containing a version number (JSON key: `version`) and a list (indicated by `[]` in JSON) of features (JSON key: `features`). This results in the following input file:  



```{literalinclude} ../../_static/gwb_input_files/BST_02_minimal_box.wb
:language: json
:lineno-start: 1
```

::::{grid} 3
:::{grid-item-card} BST_1_minimal_box.wb
:link: ../../_static/gwb_input_files/BST_02_minimal_box.wb
:::
:::{grid-item-card} BST_1_minimal_box.grid
:link: ../../_static/gwb_input_files/BST_02_minimal_box.grid
:::
:::{grid-item-card} Pavaview v1 state file 
:link: ../../_static/paraview_state_files/BST_v1.pvsm
:::
::::

```{note}
You can copy the text in the box by selecting it, or by hovering over the box and clicking the copy symbol in the top right corner. The file can also be downloaded by clicking on the `Download BST_1_minimal_box.wb` button below the textbox. 

If you want to inspect the result yourself, you can also download the corresponding `.grid` file and use the `gwb-grid` application (`gwb-grid BST_1_minimal_box.wb BST_1_minimal_box.grid`) to create a vtk output (`BST_1_minimal_box.vtk`) and view it in for example Paraview.
```

Congratulations on creating your first world builder input file! If you visualize the result you will notice that you are getting an adiabatic temperature profile and that every compositional field is zero everywhere. This is the background, or our canvas, if we want to stay in the painting analogy. Now, before we are going to get our brushes, we should first discuss the shape of our canvas a bit more.


```{figure} ../../../../doc/sphinx/_static/images/user_manual/basic_starter_tutorial/BST_02.png
:name: BST_02
:alt: Basic Starter Tutorial section 16 highres result. 
:align: center

Basic Starter Tutorial section 2. The top part of the figure shows any place where the composition is not zero as seen sligthly from above. Currently all compositional fields are zero everywhere, so nothing is shown. The botttom part shows the temperature as seen sligthly from below. This shows an adiabatic temperature gradient.
```