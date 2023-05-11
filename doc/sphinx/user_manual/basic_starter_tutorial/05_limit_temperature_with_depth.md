(part:user_manual:chap:concepts:sec:05_limit_temperature_with_depth)=
Limit temperature with depth
============================


As you may have noticed, the temperature and compostional field 0 in the model so far, reaches all the way to the bottom of the model. This is far from ideal since our model is 500km deep and the oceanic lithosphere should only reach about 100km and the "crustal composition" should only reach 25km. Luckely, this is easily solved. There are a few ways of doing this, but we can start to set a global maximum depth of 100km for the feature. This is as eash as adding `"max depth":100e3,` to the end of line 7. Please note that units are in meters. Next, we can also do this individually, so let's limit the uniform composition by adding `"max depth":50e3,` to the object. 

```{note}
If we would have set the max depth of the composition to be larger than the feature max depth of the feature, the composition will be be cut off by the feature max depth.
```

::::::{tab-set}

:::::{tab-item} Important lines
:sync: Partial

```{literalinclude} ../../_static/gwb_input_files/BST_3_limit_depth.wb
:language: json
:lineno-start: 5
:lines: 5-15
:emphasize-lines: 3,8
```
::::{grid} 2
:::{grid-item-card}  Download BST_3_limit_depth.wb
:link: ../../_static/gwb_input_files/BST_3_limit_depth.wb
:::
:::{grid-item-card}  Download BST_3_limit_depth.grid
:link: ../../_static/gwb_input_files/BST_3_limit_depth.grid
:::
::::
:::::

:::::{tab-item} Full file
:sync: Full


```{literalinclude} ../../_static/gwb_input_files/BST_3_limit_depth.wb
:language: json
:lineno-start: 1
:emphasize-lines: 7,12
```

::::{grid} 2
:::{grid-item-card}  Download BST_3_limit_depth.wb
:link: ../../_static/gwb_input_files/BST_3_limit_depth.wb
:::
:::{grid-item-card}  Download BST_3_limit_depth.grid
:link: ../../_static/gwb_input_files/BST_3_limit_depth.grid
:::
::::
:::::

::::::



```{todo}
Nice image here of the resulting changed temperature and composition.
```