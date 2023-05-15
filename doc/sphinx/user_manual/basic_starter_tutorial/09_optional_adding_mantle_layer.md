(part:user_manual:chap:concepts:sec:09_optinal_adding_mantle_layer)=
Optional adding mantle layer
============================

Sometimes you want precice control over what is happening in the mantle. In this case we want to have a specific composition for the mantle. 

```{note}
This is not always needed. For example in the visco-plastic material model in the FEM ASPECT, if no composition is defined, it is assumed to be a background composition. If you don't define a composition in an area in the world builder, the world builder will tell anyone who wants to know the composition at that location that the fraction of every composition is zero there.
```

We are going to add the mantle layer as the first object in the feature list, so that other features can override it if nececary. The model name for a mantle layer is `mantle layer`, and we will name it "upper mantle". Because we will be have already used compositions 0,1 and 3, and will use 2 later, we are going to set this to composition 4. We could also specify a different temperture model for this. The default is `adiabatic`, but you can easily set to to somthing else, or change the parameters of the adiabatic gradient.

```{note}
The default values of for example the [potenial mantle temperature](open_features_items_oneOf_3_temperature-models_items_oneOf_1_potential-mantle-temperature), the [thermal expansion coefficient](open_features_items_oneOf_3_temperature-models_items_oneOf_1_thermal-expansion-coefficient) or the [specific heat](open_features_items_oneOf_3_temperature-models_items_oneOf_1_specific-heat) have a default value of -1. This is, like we have seen before, a special value. In this case it means that the global value is used (as you can read in the documentation of the value). It is generally recommended to only set the global values so that the whole setup remains self-consistent, but if you do want to change it in individual locations, you can!
```

::::::{tab-set}

:::::{tab-item} Important lines
:sync: Partial

```{literalinclude} ../../_static/gwb_input_files/BST_09_adding_mantle_layer.wb
:language: json
:lineno-start: 5
:lines: 5-10
:emphasize-lines: 5
```
::::{grid} 2
:::{grid-item-card}  Download BST_09_adding_mantle_layer.wb
:link: ../../_static/gwb_input_files/BST_09_adding_mantle_layer.wb
:::
:::{grid-item-card}  Download BST_09_adding_mantle_layer.grid
:link: ../../_static/gwb_input_files/BST_09_adding_mantle_layer.grid
:::
::::
:::::

:::::{tab-item} Full file
:sync: Full


```{literalinclude} ../../_static/gwb_input_files/BST_09_adding_mantle_layer.wb
:language: json
:lineno-start: 1
:emphasize-lines: 9
```

::::{grid} 2
:::{grid-item-card}  Download BST_09_adding_mantle_layer.wb
:link: ../../_static/gwb_input_files/BST_09_adding_mantle_layer.wb
:::
:::{grid-item-card}  Download BST_09_adding_mantle_layer.grid
:link: ../../_static/gwb_input_files/BST_09_adding_mantle_layer.grid
:::
::::
:::::

::::::

```{todo}
Add figures
```