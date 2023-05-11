(part:user_manual:chap:concepts:sec:04_first_tectonic_feature)=
Adding your first tectonic feature
========================

Now we are finely ready to get out our brushes and start coloring in the world. We are going to start by adding an oceanic plate to the feature list. 

Each feature is an object, which means we need to enclose it in curly braces (`{}`). You will need to specify the following keys:

1. The `model` key always defines which feature is set, which is "oceanic plate" in our case. 
2. The `name` key should contain a descriptive name. The name can be anything, and is mostly to help keep the input file readable. 
3. The `coordinates` key is a list of 2D coordianates: x,y in Cartesian and long,lat in spherical. A list in JSON is indicated by angle brackets (`[]`) and the items are seperated by comma's.
4. (Optional) The `temperature models` key is a list of temperature model. Each temperature model is an object. For now we will just chose the simplest one: `uniform` where we set the temperature to `293`K.
5. (Optional) The `compositional models` key is a list of compositional models. Each compositional model is an object. Like with the temeprature model, we will just chose the simples one: `uniform` and set the composition in compositonal field 0. We will go into more detail how this works later in the tutorial.

There are some other options, which we will cover later. The file below shows the result. To focus your attention, by default only the lines of interest are shown, but you can always view the full file by clicking on the `Full file` tab.


::::::{tab-set}

:::::{tab-item} Important lines
:sync: Partial

```{literalinclude} ../../_static/gwb_input_files/BST_2_overriding_plate.wb
:language: json
:lineno-start: 4
:lines: 4-12
```
::::{grid} 2
:::{grid-item-card}  Download BST_2_overriding_plate.wb
:link: ../../_static/gwb_input_files/BST_2_overriding_plate.wb
:::
:::{grid-item-card}  Download BST_2_overriding_plate.grid
:link: ../../_static/gwb_input_files/BST_2_overriding_plate.grid
:::
::::
:::::

:::::{tab-item} Full file
:sync: Full


```{literalinclude} ../../_static/gwb_input_files/BST_2_overriding_plate.wb
:language: json
:lineno-start: 1
```

::::{grid} 2
:::{grid-item-card}  Download BST_2_overriding_plate.wb
:link: ../../_static/gwb_input_files/BST_2_overriding_plate.wb
:::
:::{grid-item-card}  Download BST_2_overriding_plate.grid
:link: ../../_static/gwb_input_files/BST_2_overriding_plate.grid
:::
::::
:::::

::::::


```{todo}
Nice image here of the resulting changed temperature and composition.
```
