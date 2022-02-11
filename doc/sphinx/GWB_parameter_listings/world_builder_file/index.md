The World Builder file
======================

```{todo}
This part of the manual will be used to list all the input file parameters of the GWB library
```


::::::{dropdown} Root
:open:
- **type:** object
- **documentation:** Root object
- **additionalProperties:** false
- **required:** [version, features]

:::::{dropdown} Version
- **default value:**
- **type:** string
- **documentation:** The major and minor version number for which the input file was written.
:::::

:::::{dropdown} cross section
- **type:** array
- **minItems:** 2
- **maxItems:** 2
- **uniqueItems:** false
- **documentation:** This is an array of two points along where the cross section is taken


::::{dropdown} items
- **type:** array
- **minItems:** 2
- **maxItems:** 2
- **documentation:**


:::{dropdown} items
- **type:** number
:::

::::

:::::

:::::{dropdown} potential mantle temperature
- **default value:** 1600
- **type:** number
- **documentation:** The potential temperature of the mantle at the surface in Kelvin.
:::::

:::::{dropdown} surface temperature
- **default value:** 293.15
- **type:** number
- **documentation:** The temperature at the surface in Kelvin.
:::::

:::::{dropdown} force surface temperature
- **default value:** false
- **type:** boolean
- **documentation:** Force the provided surface temperature to be set at the surface
:::::

:::::{dropdown} thermal expansion coefficient
- **default value:**  0.000035
- **type:** number
- **documentation:** The thermal expansion coefficient in $K^{−1}$.
:::::


:::::{dropdown} coordinate system
- **default value:**  cartesian
- **type:** object
- **documentation:** A coordinate system. Cartesian or spherical.

::::{dropdown} oneOf/1
- **type:** object
- **documentation:** Coordinate system object
- **additionalProperties:** false
- **required:** [model]

:::{dropdown} model
- **default value:**  
- **type:** string
- **documentation:** The name which the user has given to the feature.
- **enum:** [Cartesian]
:::

::::

::::{dropdown} oneOf/2
- **type:** object
- **documentation:** Coordinate system object
- **additionalProperties:** false
- **required:** [model, depth method]

:::{dropdown} model
- **default value:**  
- **type:** string
- **documentation:** The name which the user has given to the feature.
- **enum:** [spherical]
:::

:::{dropdown} depth method
- **default value:**  
- **type:** string
- **documentation:** Which depth method to use in the spherical case. The available options are ’starting
point’ and ’begin segment’
- **enum:** [[starting point, begin segment, continuous]
:::

::::

:::::

::::::
