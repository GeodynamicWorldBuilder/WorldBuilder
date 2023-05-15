(part:user_manual:chap:concepts:sec:08_passive_marging_variable_depth)=
Adding a passive marging with variable depth
============================

Although we have hinted towards it before, there is also an [area feature](part:user_manual:chap:concepts:sec:area_features) called `continental plate`. It works as you would expect based on what you have seen with the oceanice plate, but we are going to use it to do something very cool in our model: have a variable depths of a feature or even of layers within a given feature!

To showcase this, we will be adding a passive marging to our model at the overriding plate side of our model. To start out, we need to know that both the `min depth` and `max depth` accept two types of values: a number and an array of values at points. 

```{note}
For an example of what this looks like in techical terms, see {ref}`open_features_items_oneOf_1_max-depth`.
```

The number input we have seen before, and just sets the maximum depth to a single value. The value at points system works a bit differently, but can be used to achieve the same result. To start out easy, this is exactly what we are going to to for the `max depth` of the continetal feature. 

The value at points system  creates a list of points, which at least include all the edge points and assigns a value to them. Then to get the actual value, `max depth` in our case, an interpolation between some close points is performed to get the value at that point. 

Below are two examples of how to first set the depth of two points to 200, and then an example of how to also set a third point to a value of 100.

```{code-block} json
---
lineno-start: 1
---
    "max depth":[[200e3, [[10e3,0],[20e3,10e3]]]],
    
    "max depth":[[200e3, [[10e3,0],[20e3,10e3]]],[100e3,[[15e3,0]]]]
```

A common operation is to set all the corners to a single value, and then maybe overwrite individual ones. To make life easy, there is a quick way to (re)set the value at the corner points: just pass a value with no points. This is what is done in the code sample below at the empasised lines. The first entry is a value without points. That means that all the coner points are set to that value.

```{note}
If you provide a point twice, explicitly, or implicity through the use of the corner values shortcut (e.g. `[200]`), the last defined value is used. This follows the painting analogy used before, where you overpaint older values.
```

::::::{tab-set}

:::::{tab-item} Important lines
:sync: Partial

```{literalinclude} ../../_static/gwb_input_files/BST_08_passive_margin_variable_depth.wb
:language: json
:lineno-start: 16
:lines: 16-30
:emphasize-lines: 2,6,11,13
```
::::{grid} 2
:::{grid-item-card}  Download BST_08_passive_margin_variable_depth.wb
:link: ../../_static/gwb_input_files/BST_08_passive_margin_variable_depth.wb
:::
:::{grid-item-card}  Download BST_08_passive_margin_variable_depth.grid
:link: ../../_static/gwb_input_files/BST_08_passive_margin_variable_depth.grid
:::
::::
:::::

:::::{tab-item} Full file
:sync: Full


```{literalinclude} ../../_static/gwb_input_files/BST_08_passive_margin_variable_depth.wb
:language: json
:lineno-start: 1
:emphasize-lines: 17,21,26,28
```

::::{grid} 2
:::{grid-item-card}  Download BST_08_passive_margin_variable_depth.wb
:link: ../../_static/gwb_input_files/BST_08_passive_margin_variable_depth.wb
:::
:::{grid-item-card}  Download BST_08_passive_margin_variable_depth.grid
:link: ../../_static/gwb_input_files/BST_08_passive_margin_variable_depth.grid
:::
::::
:::::

::::::

```{todo}
Add figures
```