(part:user_manual:chap:concepts:sec:06_oceanic_plate_temperature)=
Oceanic plate temperature
=========================

Using uniform temperatures will only allow you to build very simple models. So lets make the model a bit more interesting adding a half space model to the overriding plate. This allows us to add a temperature structure based the distance from a ridge. 

To be able to do this, we need to provide the following information to the model: `model`, `ridge coordianates`, `spreading velocity` and `max depth`. Please see {ref}`open_features_items_oneOf_4_temperature-models_items_oneOf_2` for more info.

We are already familair with `max depth`, but `spreading velocity` and `ridge coordinates` are new. The [spreading velocity](open_features_items_oneOf_4_temperature-models_items_oneOf_2_spreading-velocity) is simply the velocity of how fast one part of the plate is moving away from the ridge. The [ridge coordinates](open_features_items_oneOf_4_temperature-models_items_oneOf_2_ridge-coordinates) are a list of spreading ridges, where each ridge is a list of 2D points. The half space is computed with the distance to the closest point on any ridge and the spreading velocity. 

If you are familair with the half space cooling model, you would also think some kind of top and bottom/mantle temperature is required. These values can be provided, but are by default they are set to resonable values. The top temperature is set to 193.15K and the bottom temperature is set to -1K. For this model, if the temperature is set  to a negative value, it automatically uses the computed adiabatic temperature at that depth. This will be good enough for our tutorial, so we won't be changing those values here, but you are free to do so!


::::::{tab-set}

:::::{tab-item} Important lines
:sync: Partial

```{literalinclude} ../../_static/gwb_input_files/BST_4_oceanic_plate_temperature.wb
:language: json
:lineno-start: 6
:lines: 6-14
:emphasize-lines: 6,7
```
::::{grid} 2
:::{grid-item-card}  Download BST_4_oceanic_plate_temperature.wb
:link: ../../_static/gwb_input_files/BST_4_oceanic_plate_temperature.wb
:::
:::{grid-item-card}  Download BST_4_oceanic_plate_temperature.grid
:link: ../../_static/gwb_input_files/BST_4_oceanic_plate_temperature.grid
:::
::::
:::::

:::::{tab-item} Full file
:sync: Full


```{literalinclude} ../../_static/gwb_input_files/BST_4_oceanic_plate_temperature.wb
:language: json
:lineno-start: 1
:emphasize-lines: 11,12
```

::::{grid} 2
:::{grid-item-card}  Download BST_4_oceanic_plate_temperature.wb
:link: ../../_static/gwb_input_files/BST_4_oceanic_plate_temperature.wb
:::
:::{grid-item-card}  Download BST_4_oceanic_plate_temperature.grid
:link: ../../_static/gwb_input_files/BST_4_oceanic_plate_temperature.grid
:::
::::
:::::

::::::


```{todo}
Add figures of model
```