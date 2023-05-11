(part:user_manual:chap:concepts:sec:07_subducting_plate_oceanic_part)=
Subducting plate oceanic part
========================

Now that we have made the overring (Carribean) plate, it is time to add the oceanic part of the subducting plate (the Atlantic). To do this, we just need to add another object to the feature list. For this plate we will assume the oceanic lithosphere is really old, and the temperature gradient in the lithosphere is therefore linear between a top temperature of 293.15K and the adiabatic temeprature a the max depth of the plate. Luckely for us, this is the default in the world builder, so we will only need to provide the `model` (`linear`) and the `max depth`, which we will set to 100km.

For the composition, we are going to do something a bit more special. A commen thing you will probably want to do with compositional fields is have multiple layers of them within a feature. This is very easy to do in the GWB. If you remember from the last section, both the `temperature models` and `compositional models` are a list of objects. Making layers is thus as easy as adding multiple compositional models with each their own `min depth` and `max depth`. 

::::::{tab-set}

:::::{tab-item} Important lines
:sync: Partial

```{literalinclude} ../../_static/gwb_input_files/BST_5_subducting_plate_oceanic_part.wb
:language: json
:lineno-start: 18
:lines: 18-25
:emphasize-lines: 5,6,7
```
::::{grid} 2
:::{grid-item-card}  Download BST_5_subducting_plate_oceanic_part.wb
:link: ../../_static/gwb_input_files/BST_5_subducting_plate_oceanic_part.wb
:::
:::{grid-item-card}  Download BST_5_subducting_plate_oceanic_part.grid
:link: ../../_static/gwb_input_files/BST_5_subducting_plate_oceanic_part.grid
:::
::::
:::::

:::::{tab-item} Full file
:sync: Full


```{literalinclude} ../../_static/gwb_input_files/BST_5_subducting_plate_oceanic_part.wb
:language: json
:lineno-start: 1
:emphasize-lines: 22,23,24
```

::::{grid} 2
:::{grid-item-card}  Download BST_5_subducting_plate_oceanic_part.wb
:link: ../../_static/gwb_input_files/BST_5_subducting_plate_oceanic_part.wb
:::
:::{grid-item-card}  Download BST_5_subducting_plate_oceanic_part.grid
:link: ../../_static/gwb_input_files/BST_5_subducting_plate_oceanic_part.grid
:::
::::
:::::

::::::


```{todo}
Add figures
```