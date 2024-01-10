:::::::::::::::::::::::::{dropdown} /
:open:
:name: closed_

- **type**:object
- **documentation**:Root object
- **additionalProperties**:false
- **required**:[version, features]

::::::::::::::::::::::::{dropdown} /version
:name: closed_version

- **default value**:
- **type**:string
- **documentation**:The major and minor version number for which the input file was written.
::::::::::::::::::::::::

::::::::::::::::::::::::{dropdown} /cross section
:name: closed_cross-section

- **type**:array
- **minItems**:2
- **maxItems**:2
- **uniqueItems**:false
- **documentation**:This is an array of two points along where the cross section is taken
:::::::::::::::::::::::{dropdown} /cross section/items
:name: closed_cross-section_items

- **type**:array
- **minItems**:2
- **maxItems**:2
- **documentation**:
::::::::::::::::::::::{dropdown} /cross section/items/items
:name: closed_cross-section_items_items

- **type**:number
::::::::::::::::::::::

:::::::::::::::::::::::

::::::::::::::::::::::::

::::::::::::::::::::::::{dropdown} /potential mantle temperature
:name: closed_potential-mantle-temperature

- **default value**:1600.0
- **type**:number
- **documentation**:The potential temperature of the mantle at the surface in Kelvin.
::::::::::::::::::::::::

::::::::::::::::::::::::{dropdown} /surface temperature
:name: closed_surface-temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature at the surface in Kelvin.
::::::::::::::::::::::::

::::::::::::::::::::::::{dropdown} /force surface temperature
:name: closed_force-surface-temperature

- **default value**:false
- **type**:boolean
- **documentation**:Force the provided surface temperature to be set at the surface
::::::::::::::::::::::::

::::::::::::::::::::::::{dropdown} /thermal expansion coefficient
:name: closed_thermal-expansion-coefficient

- **default value**:0.000035
- **type**:number
- **documentation**:The thermal expansion coefficient in $K^{-1}$.
::::::::::::::::::::::::

::::::::::::::::::::::::{dropdown} /specific heat
:name: closed_specific-heat

- **default value**:1250.0
- **type**:number
- **documentation**:The specific heat in $J kg^{-1} K^{-1}.$
::::::::::::::::::::::::

::::::::::::::::::::::::{dropdown} /thermal diffusivity
:name: closed_thermal-diffusivity

- **default value**:8.04e-7
- **type**:number
- **documentation**:The thermal diffusivity in $m^{2} s^{-1}$.
::::::::::::::::::::::::

::::::::::::::::::::::::{dropdown} /maximum distance between coordinates
:name: closed_maximum-distance-between-coordinates

- **default value**:0.0
- **type**:number
- **documentation**:This enforces a maximum distance (in degree for spherical coordinates or meter in cartesian coordinates) between coordinates in the model. If the distance is larger, extra points are added by interpolation. Requires interpolation to be not 'none'.
::::::::::::::::::::::::

::::::::::::::::::::::::{dropdown} /interpolation
:name: closed_interpolation

- **default value**:continuous monotone spline
- **type**:string
- **documentation**:What type of interpolation should be used to enforce the minimum points per distance parameter. Options are none, linear, monotone spline and continuous monotone spline interpolation.
::::::::::::::::::::::::

::::::::::::::::::::::::{dropdown} /coordinate system
:name: closed_coordinate-system

- **documentation**:A coordinate system. Cartesian or spherical.
- **default value**:cartesian
- **type**:object
:::::::::::::::::::::::{dropdown} /coordinate system/oneOf
:name: closed_coordinate-system_oneOf

::::::::::::::::::::::{dropdown} /coordinate system/oneOf/1
:name: closed_coordinate-system_oneOf_1

- **type**:object
- **documentation**:A Cartesian coordinate system. Coordinates are (x,y,z) and extend infintly in all directions.
- **additionalProperties**:false
- **required**:[model]

:::::::::::::::::::::{dropdown} /coordinate system/oneOf/1/model
:name: closed_coordinate-system_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name which the user has given to the feature.
- **enum**:[cartesian]
:::::::::::::::::::::



::::::::::::::::::::::

::::::::::::::::::::::{dropdown} /coordinate system/oneOf/2
:name: closed_coordinate-system_oneOf_2

- **type**:object
- **documentation**:A spherical coordinate system. The coordinates are (radius, longitude, latitude). The radius is set in this plugin, the longitude extends at least from -360 to 360 degrees, and the latitude extends from -90 to 90. It is required to choose a depth method. Please see the manual for more information.
- **additionalProperties**:false
- **required**:[model, depth method]

:::::::::::::::::::::{dropdown} /coordinate system/oneOf/2/model
:name: closed_coordinate-system_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name which the user has given to the feature.
- **enum**:[spherical]
:::::::::::::::::::::

:::::::::::::::::::::{dropdown} /coordinate system/oneOf/2/depth method
:name: closed_coordinate-system_oneOf_2_depth-method

- **default value**:
- **type**:string
- **documentation**:Which depth method to use in the spherical case. The available options are 'starting point', 'begin segment' and 'begin at end segment'. See the manual section on coordinate systems for more info.
- **enum**:[starting point, begin segment, begin at end segment, continuous]
:::::::::::::::::::::

:::::::::::::::::::::{dropdown} /coordinate system/oneOf/2/radius
:name: closed_coordinate-system_oneOf_2_radius

- **default value**:6371000.0
- **type**:number
- **documentation**:The radius of the sphere.
:::::::::::::::::::::



::::::::::::::::::::::


::::::::::::::::::::::::

::::::::::::::::::::::::{dropdown} /gravity model
:name: closed_gravity-model

- **documentation**:A gravity model for the world.
- **default value**:uniform
- **type**:object
:::::::::::::::::::::::{dropdown} /gravity model/oneOf
:name: closed_gravity-model_oneOf

::::::::::::::::::::::{dropdown} /gravity model/oneOf/1
:name: closed_gravity-model_oneOf_1

- **type**:object
- **documentation**:Uniform gravity model. It returns the gravity vector in a Cartesian coordinate system at a given position, which has a constant magitude for the whole domain. The vector points down in cartesian coordinates and to the center of the sphere in spherical coordinates.
- **additionalProperties**:false
- **required**:[model]

:::::::::::::::::::::{dropdown} /gravity model/oneOf/1/model
:name: closed_gravity-model_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name which the user has given to the feature.
- **enum**:[uniform]
:::::::::::::::::::::

:::::::::::::::::::::{dropdown} /gravity model/oneOf/1/magnitude
:name: closed_gravity-model_oneOf_1_magnitude

- **default value**:9.81
- **type**:number
- **documentation**:The magnitude of the gravity.
:::::::::::::::::::::



::::::::::::::::::::::


::::::::::::::::::::::::

::::::::::::::::::::::::{dropdown} /features
:name: closed_features

- **documentation**:A list of features.
- **default value**:
- **type**:array
:::::::::::::::::::::::{dropdown} /features/items
:name: closed_features_items

::::::::::::::::::::::{dropdown} /features/items/oneOf
:name: closed_features_items_oneOf

:::::::::::::::::::::{dropdown} /features/items/oneOf/1
:name: closed_features_items_oneOf_1

- **type**:object
- **documentation**:Continental plate object. Requires properties `model` and `coordinates`.
- **additionalProperties**:false
- **required**:[model]

::::::::::::::::::::{dropdown} /features/items/oneOf/1/model
:name: closed_features_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name which the user has given to the feature.
- **enum**:[continental plate]
::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/1/name
:name: closed_features_items_oneOf_1_name

- **default value**:
- **type**:string
- **documentation**:The name which the user has given to the feature.
::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/1/coordinates
:name: closed_features_items_oneOf_1_coordinates

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:An array of 2d Points representing an array of coordinates where the feature is located.
:::::::::::::::::::{dropdown} /features/items/oneOf/1/coordinates/items
:name: closed_features_items_oneOf_1_coordinates_items

- **type**:array
- **minItems**:2
- **maxItems**:2
- **documentation**:
::::::::::::::::::{dropdown} /features/items/oneOf/1/coordinates/items/items
:name: closed_features_items_oneOf_1_coordinates_items_items

- **type**:number
::::::::::::::::::

:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/1/interpolation
:name: closed_features_items_oneOf_1_interpolation

- **default value**:global
- **type**:string
- **documentation**:What type of interpolation should be used to enforce the minimum points per distance parameter. Options are global, none, linear, monotone spline and continuous monotone spline interpolation. If this value is set to global, the global value for interpolation is used.
::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/1/min depth
:name: closed_features_items_oneOf_1_min-depth

- **documentation**:The depth from which this feature is present
:::::::::::::::::::{dropdown} /features/items/oneOf/1/min depth/oneOf
:name: closed_features_items_oneOf_1_min-depth_oneOf

::::::::::::::::::{dropdown} /features/items/oneOf/1/min depth/oneOf/1
:name: closed_features_items_oneOf_1_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/1/min depth/oneOf/2
:name: closed_features_items_oneOf_1_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::::::{dropdown} /features/items/oneOf/1/min depth/oneOf/2/items
:name: closed_features_items_oneOf_1_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::::::{dropdown} /features/items/oneOf/1/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_1_min-depth_oneOf_2_items_items

:::::::::::::::{dropdown} /features/items/oneOf/1/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf

::::::::::::::{dropdown} /features/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::::::{dropdown} /features/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::::::{dropdown} /features/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

:::::::::::::::::

::::::::::::::::::


::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/1/max depth
:name: closed_features_items_oneOf_1_max-depth

- **documentation**:The depth to which this feature is present
:::::::::::::::::::{dropdown} /features/items/oneOf/1/max depth/oneOf
:name: closed_features_items_oneOf_1_max-depth_oneOf

::::::::::::::::::{dropdown} /features/items/oneOf/1/max depth/oneOf/1
:name: closed_features_items_oneOf_1_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/1/max depth/oneOf/2
:name: closed_features_items_oneOf_1_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::::::{dropdown} /features/items/oneOf/1/max depth/oneOf/2/items
:name: closed_features_items_oneOf_1_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::::::{dropdown} /features/items/oneOf/1/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_1_max-depth_oneOf_2_items_items

:::::::::::::::{dropdown} /features/items/oneOf/1/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf

::::::::::::::{dropdown} /features/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::::::{dropdown} /features/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::::::{dropdown} /features/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

:::::::::::::::::

::::::::::::::::::


::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models
:name: closed_features_items_oneOf_1_temperature-models

- **documentation**:A list of temperature models.
- **default value**:
- **type**:array
:::::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items
:name: closed_features_items_oneOf_1_temperature-models_items

::::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf

:::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1

- **type**:object
- **documentation**:Adiabatic temperature model. Uses global values by default.
- **additionalProperties**:false
- **required**:[model]

::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/model
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[adiabatic]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/operation
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/min depth
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_min-depth

- **documentation**:The depth in meters from which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/min depth/oneOf
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/min depth/oneOf/1
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/min depth/oneOf/2
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/min depth/oneOf/2/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/max depth
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_max-depth

- **documentation**:The depth in meters to which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/max depth/oneOf
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/max depth/oneOf/1
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/max depth/oneOf/2
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/max depth/oneOf/2/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/potential mantle temperature
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_potential-mantle-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The potential temperature of the mantle at the surface in Kelvin. If the value is lower then zero, the global value is used.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/thermal expansion coefficient
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_thermal-expansion-coefficient

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal expansion coefficient in $K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/1/specific heat
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_1_specific-heat

- **default value**:-1.0
- **type**:number
- **documentation**:The specific heat in $J kg^{-1} K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2

- **type**:object
- **documentation**:Linear temperature model. Can be set to use an adiabatic temperature at the boundaries.
- **additionalProperties**:false
- **required**:[model, max depth]

::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/model
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[linear]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/operation
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/min depth
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_min-depth

- **documentation**:The depth in meters from which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/min depth/oneOf
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/min depth/oneOf/1
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/min depth/oneOf/2
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/min depth/oneOf/2/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/max depth
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_max-depth

- **documentation**:The depth in meters to which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/max depth/oneOf
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/max depth/oneOf/1
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/max depth/oneOf/2
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/max depth/oneOf/2/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/top temperature
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_top-temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature at the top in degree Kelvin of this feature.If the value is below zero, the an adiabatic temperature is used.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/2/bottom temperature
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_2_bottom-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The temperature at the top in degree Kelvin of this feature. If the value is below zero, an adiabatic temperature is used.
::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3

- **type**:object
- **documentation**:Uniform temperature model. Set the temperature to a constan value.
- **additionalProperties**:false
- **required**:[model, temperature]

::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/model
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[uniform]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/operation
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/min depth
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_min-depth

- **documentation**:The depth in meters from which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/min depth/oneOf
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/min depth/oneOf/1
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/min depth/oneOf/2
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/min depth/oneOf/2/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/max depth
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_max-depth

- **documentation**:The depth in meters to which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/max depth/oneOf
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/max depth/oneOf/1
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/max depth/oneOf/2
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/max depth/oneOf/2/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/temperature models/items/oneOf/3/temperature
:name: closed_features_items_oneOf_1_temperature-models_items_oneOf_3_temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature in degree Kelvin which this feature should have
::::::::::::::::



:::::::::::::::::


:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/1/composition models
:name: closed_features_items_oneOf_1_composition-models

- **documentation**:A list of composition models.
- **default value**:
- **type**:array
:::::::::::::::::::{dropdown} /features/items/oneOf/1/composition models/items
:name: closed_features_items_oneOf_1_composition-models_items

::::::::::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf
:name: closed_features_items_oneOf_1_composition-models_items_oneOf

:::::::::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1

- **type**:object
- **documentation**:Uniform compositional model. Sets constant compositional field.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/model
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the composition model.
- **enum**:[uniform]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/min depth
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_min-depth

- **documentation**:The depth in meters from which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/min depth/oneOf
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/min depth/oneOf/1
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/min depth/oneOf/2
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/min depth/oneOf/2/items
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/max depth
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_max-depth

- **documentation**:The depth in meters to which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/max depth/oneOf
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/max depth/oneOf/1
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/max depth/oneOf/2
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/max depth/oneOf/2/items
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the composition which are present there.
:::::::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/fractions
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:TA list of compositional fractions corresponding to the compositions list.
:::::::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/fractions/items
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_fractions_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/composition models/items/oneOf/1/operation
:name: closed_features_items_oneOf_1_composition-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value. Replacing implies that all compositions not explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.
- **enum**:[replace, replace defined only, add, subtract]
::::::::::::::::



:::::::::::::::::


:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/1/grains models
:name: closed_features_items_oneOf_1_grains-models

- **documentation**:A list of grains models.
- **default value**:
- **type**:array
:::::::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items
:name: closed_features_items_oneOf_1_grains-models_items

::::::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf
:name: closed_features_items_oneOf_1_grains-models_items_oneOf

:::::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1

- **type**:object
- **documentation**:Random uniform distribution grains model. The size of the grains can be independently set to a single value or to a random distribution.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/model
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[random uniform distribution]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/min depth
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_min-depth

- **documentation**:The depth in meters from which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/min depth/oneOf
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/min depth/oneOf/1
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/min depth/oneOf/2
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/min depth/oneOf/2/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/max depth
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_max-depth

- **documentation**:The depth in meters to which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/max depth/oneOf
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/max depth/oneOf/1
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/max depth/oneOf/2
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/max depth/oneOf/2/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/orientation operation
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/grain sizes
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be randomized between 0 and 1.
:::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/grain sizes/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_grain-sizes_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/normalize grain sizes
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_normalize-grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of whether the sizes of the grains should be normalized or not. If normalized, the total of the grains of a composition will be equal to 1.
:::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/1/normalize grain sizes/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_1_normalize-grain-sizes_items

- **default value**:true
- **type**:boolean
- **documentation**:
:::::::::::::::

::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2

- **type**:object
- **documentation**:Uniform grains model. All grains start exactly the same.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/model
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[uniform]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/min depth
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_min-depth

- **documentation**:The depth in meters from which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/min depth/oneOf
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/min depth/oneOf/1
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/min depth/oneOf/2
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/min depth/oneOf/2/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/max depth
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_max-depth

- **documentation**:The depth in meters to which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/max depth/oneOf
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/max depth/oneOf/1
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/max depth/oneOf/2
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/max depth/oneOf/2/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/compositions
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/compositions/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/rotation matrices
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_rotation-matrices

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the rotation matrices of the grains which are present there for each compositions.
:::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/rotation matrices/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_rotation-matrices_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/rotation matrices/items/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_rotation-matrices_items_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/rotation matrices/items/items/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_rotation-matrices_items_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/Euler angles z-x-z
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_Euler-angles-z-x-z

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the z-x-z Euler angles of the grains which are present there for each compositions.
:::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/Euler angles z-x-z/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_Euler-angles-z-x-z_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/Euler angles z-x-z/items/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_Euler-angles-z-x-z_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/orientation operation
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace, multiply]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/grain sizes
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be set so that the total is equal to 1.
:::::::::::::::{dropdown} /features/items/oneOf/1/grains models/items/oneOf/2/grain sizes/items
:name: closed_features_items_oneOf_1_grains-models_items_oneOf_2_grain-sizes_items

- **default value**:-1.0
- **type**:number
- **documentation**:
:::::::::::::::

::::::::::::::::



:::::::::::::::::


:::::::::::::::::::

::::::::::::::::::::



:::::::::::::::::::::

:::::::::::::::::::::{dropdown} /features/items/oneOf/2
:name: closed_features_items_oneOf_2

- **type**:object
- **documentation**:Fault object. Requires properties `model` and `coordinates`.
- **additionalProperties**:false
- **required**:[model]

::::::::::::::::::::{dropdown} /features/items/oneOf/2/model
:name: closed_features_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name which the user has given to the feature.
- **enum**:[fault]
::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/2/name
:name: closed_features_items_oneOf_2_name

- **default value**:
- **type**:string
- **documentation**:The name which the user has given to the feature.
::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/2/coordinates
:name: closed_features_items_oneOf_2_coordinates

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:An array of 2d Points representing an array of coordinates where the feature is located.
:::::::::::::::::::{dropdown} /features/items/oneOf/2/coordinates/items
:name: closed_features_items_oneOf_2_coordinates_items

- **type**:array
- **minItems**:2
- **maxItems**:2
- **documentation**:
::::::::::::::::::{dropdown} /features/items/oneOf/2/coordinates/items/items
:name: closed_features_items_oneOf_2_coordinates_items_items

- **type**:number
::::::::::::::::::

:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/2/interpolation
:name: closed_features_items_oneOf_2_interpolation

- **default value**:global
- **type**:string
- **documentation**:What type of interpolation should be used to enforce the minimum points per distance parameter. Options are global, none, linear, monotone spline and continuous monotone spline interpolation. If this value is set to global, the global value for interpolation is used.
::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/2/min depth
:name: closed_features_items_oneOf_2_min-depth

- **default value**:0.0
- **type**:number
- **documentation**:The depth to which this feature is present
::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/2/max depth
:name: closed_features_items_oneOf_2_max-depth

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The depth to which this feature is present
::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/2/dip point
:name: closed_features_items_oneOf_2_dip-point

- **type**:array
- **minItems**:2
- **maxItems**:2
- **documentation**:The depth to which this feature is present
:::::::::::::::::::{dropdown} /features/items/oneOf/2/dip point/items
:name: closed_features_items_oneOf_2_dip-point_items

- **type**:number
:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/2/segments
:name: closed_features_items_oneOf_2_segments

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:The depth to which this feature is present
:::::::::::::::::::{dropdown} /features/items/oneOf/2/segments/items
:name: closed_features_items_oneOf_2_segments_items

- **type**:object
- **additionalProperties**:false
- **documentation**:
- **required**:[length, thickness, angle]

::::::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/length
:name: closed_features_items_oneOf_2_segments_items_length

- **type**:number
::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/thickness
:name: closed_features_items_oneOf_2_segments_items_thickness

- **type**:array
- **minItems**:1
- **maxItems**:2
:::::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/thickness/items
:name: closed_features_items_oneOf_2_segments_items_thickness_items

- **type**:number
:::::::::::::::::

::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/top truncation
:name: closed_features_items_oneOf_2_segments_items_top-truncation

- **type**:array
- **minItems**:1
- **maxItems**:2
:::::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/top truncation/items
:name: closed_features_items_oneOf_2_segments_items_top-truncation_items

- **type**:number
:::::::::::::::::

::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/angle
:name: closed_features_items_oneOf_2_segments_items_angle

- **type**:array
- **minItems**:1
- **maxItems**:2
:::::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/angle/items
:name: closed_features_items_oneOf_2_segments_items_angle_items

- **type**:number
:::::::::::::::::

::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models
:name: closed_features_items_oneOf_2_segments_items_temperature-models

- **documentation**:
- **default value**:
- **type**:array
:::::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items

::::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf

:::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/1
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_1

- **type**:object
- **documentation**:Adiabatic temperature model. Uses global values by default.
- **additionalProperties**:false
- **required**:[model]

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/1/model
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[adiabatic]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/1/operation
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/1/min distance fault center
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_1_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/1/max distance fault center
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_1_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/1/potential mantle temperature
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_1_potential-mantle-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The potential temperature of the mantle at the surface in Kelvin. If the value is lower then zero, the global value is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/1/thermal expansion coefficient
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_1_thermal-expansion-coefficient

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal expansion coefficient in $K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/1/specific heat
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_1_specific-heat

- **default value**:-1.0
- **type**:number
- **documentation**:The specific heat in $J kg^{-1} K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::::



:::::::::::::::

:::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/2
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_2

- **type**:object
- **documentation**:Linear temperature model. Can be set to use an adiabatic temperature at the boundaries.
- **additionalProperties**:false
- **required**:[model, max distance fault center]

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/2/model
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[linear]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/2/operation
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_2_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/2/min distance fault center
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_2_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The minimum distance to the center of the fault. This determines where the linear temperature starts.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/2/max distance fault center
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_2_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The minimum distance to the center of the fault. This determines where the linear temperature end.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/2/center temperature
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_2_center-temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature at the center of this feature in degree Kelvin.If the value is below zero, the an adiabatic temperature is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/2/side temperature
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_2_side-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The temperature at the sides of this feature in degree Kelvin. If the value is below zero, an adiabatic temperature is used.
::::::::::::::



:::::::::::::::

:::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/3
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_3

- **type**:object
- **documentation**:Uniform temperature model. Set the temperature to a constan value.
- **additionalProperties**:false
- **required**:[model, temperature]

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/3/model
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_3_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[uniform]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/3/operation
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_3_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/3/min distance fault center
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_3_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/3/max distance fault center
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_3_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/temperature models/items/oneOf/3/temperature
:name: closed_features_items_oneOf_2_segments_items_temperature-models_items_oneOf_3_temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature in degree Kelvin which this feature should have
::::::::::::::



:::::::::::::::


:::::::::::::::::

::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models
:name: closed_features_items_oneOf_2_segments_items_composition-models

- **documentation**:
- **default value**:
- **type**:array
:::::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items
:name: closed_features_items_oneOf_2_segments_items_composition-models_items

::::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf

:::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf/1
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf_1

- **type**:object
- **documentation**:Compositional model object
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf/1/model
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the composition model.
- **enum**:[smooth]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf/1/min distance fault center
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf_1_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf/1/side distance fault center
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf_1_side-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance over which the composition is reduced from 1 to 0.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf/1/center fractions
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf_1_center-fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:The composition fraction at the center of the fault.
:::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf/1/center fractions/items
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf_1_center-fractions_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf/1/side fractions
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf_1_side-fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:The composition fraction at the sides of this feature.
:::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf/1/side fractions/items
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf_1_side-fractions_items

- **default value**:0.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the composition which are present there.
:::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf/1/operation
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value. Replacing implies that all compositions not explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.
- **enum**:[replace, replace defined only, add, subtract]
::::::::::::::



:::::::::::::::

:::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf/2
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf_2

- **type**:object
- **documentation**:Uniform compositional model. Sets constant compositional field.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf/2/model
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the composition model.
- **enum**:[uniform]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf/2/min distance fault center
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf_2_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf/2/max distance fault center
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf_2_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf/2/compositions
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf_2_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the composition which are present there.
:::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf/2/compositions/items
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf_2_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf/2/fractions
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf_2_fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:TA list of compositional fractions corresponding to the compositions list.
:::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf/2/fractions/items
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf_2_fractions_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/composition models/items/oneOf/2/operation
:name: closed_features_items_oneOf_2_segments_items_composition-models_items_oneOf_2_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value. Replacing implies that all compositions not explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.
- **enum**:[replace, replace defined only, add, subtract]
::::::::::::::



:::::::::::::::


:::::::::::::::::

::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models
:name: closed_features_items_oneOf_2_segments_items_grains-models

- **documentation**:
- **default value**:
- **type**:array
:::::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items
:name: closed_features_items_oneOf_2_segments_items_grains-models_items

::::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf

:::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/1
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_1

- **type**:object
- **documentation**:Random uniform distribution grains model. The size of the grains can be independently set to a single value or to a random distribution.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/1/model
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[random uniform distribution]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/1/min distance fault center
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_1_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The distance from the fault center in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/1/max distance fault center
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_1_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance from the fault in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/1/orientation operation
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_1_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/1/grain sizes
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_1_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be randomized between 0 and 1.
:::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/1/grain sizes/items
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_1_grain-sizes_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/1/normalize grain sizes
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_1_normalize-grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of whether the sizes of the grains should be normalized or not. If normalized, the total of the grains of a composition will be equal to 1.
:::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/1/normalize grain sizes/items
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_1_normalize-grain-sizes_items

- **default value**:true
- **type**:boolean
- **documentation**:
:::::::::::::

::::::::::::::



:::::::::::::::

:::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/2
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_2

- **type**:object
- **documentation**:Uniform grains model. All grains start exactly the same.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/2/model
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[uniform]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/2/min distance fault center
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_2_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The distance from the fault center in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/2/max distance fault center
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_2_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance from the fault in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/2/compositions
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_2_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/2/compositions/items
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_2_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/2/rotation matrices
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_2_rotation-matrices

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the grains which are present there for each compositions.
:::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/2/rotation matrices/items
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_2_rotation-matrices_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/2/rotation matrices/items/items
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_2_rotation-matrices_items_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
:::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/2/rotation matrices/items/items/items
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_2_rotation-matrices_items_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
:::::::::::

::::::::::::

:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/2/Euler angles z-x-z
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_2_Euler-angles-z-x-z

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the z-x-z Euler angles of the grains which are present there for each compositions.
:::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/2/Euler angles z-x-z/items
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_2_Euler-angles-z-x-z_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/2/Euler angles z-x-z/items/items
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_2_Euler-angles-z-x-z_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::

:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/2/orientation operation
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_2_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/2/grain sizes
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_2_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be set so that the total is equal to 1.
:::::::::::::{dropdown} /features/items/oneOf/2/segments/items/grains models/items/oneOf/2/grain sizes/items
:name: closed_features_items_oneOf_2_segments_items_grains-models_items_oneOf_2_grain-sizes_items

- **default value**:-1.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::



:::::::::::::::


:::::::::::::::::

::::::::::::::::::



:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models
:name: closed_features_items_oneOf_2_temperature-models

- **documentation**:A list of temperature models.
- **default value**:
- **type**:array
:::::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items
:name: closed_features_items_oneOf_2_temperature-models_items

::::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf

:::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/1
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_1

- **type**:object
- **documentation**:Adiabatic temperature model. Uses global values by default.
- **additionalProperties**:false
- **required**:[model]

::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/1/model
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[adiabatic]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/1/operation
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/1/min distance fault center
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_1_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/1/max distance fault center
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_1_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/1/potential mantle temperature
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_1_potential-mantle-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The potential temperature of the mantle at the surface in Kelvin. If the value is lower then zero, the global value is used.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/1/thermal expansion coefficient
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_1_thermal-expansion-coefficient

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal expansion coefficient in $K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/1/specific heat
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_1_specific-heat

- **default value**:-1.0
- **type**:number
- **documentation**:The specific heat in $J kg^{-1} K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/2
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_2

- **type**:object
- **documentation**:Linear temperature model. Can be set to use an adiabatic temperature at the boundaries.
- **additionalProperties**:false
- **required**:[model, max distance fault center]

::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/2/model
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[linear]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/2/operation
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_2_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/2/min distance fault center
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_2_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The minimum distance to the center of the fault. This determines where the linear temperature starts.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/2/max distance fault center
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_2_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The minimum distance to the center of the fault. This determines where the linear temperature end.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/2/center temperature
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_2_center-temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature at the center of this feature in degree Kelvin.If the value is below zero, the an adiabatic temperature is used.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/2/side temperature
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_2_side-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The temperature at the sides of this feature in degree Kelvin. If the value is below zero, an adiabatic temperature is used.
::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/3
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_3

- **type**:object
- **documentation**:Uniform temperature model. Set the temperature to a constan value.
- **additionalProperties**:false
- **required**:[model, temperature]

::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/3/model
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_3_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[uniform]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/3/operation
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_3_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/3/min distance fault center
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_3_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/3/max distance fault center
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_3_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance in meters to which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/temperature models/items/oneOf/3/temperature
:name: closed_features_items_oneOf_2_temperature-models_items_oneOf_3_temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature in degree Kelvin which this feature should have
::::::::::::::::



:::::::::::::::::


:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/2/composition models
:name: closed_features_items_oneOf_2_composition-models

- **documentation**:A list of composition models.
- **default value**:
- **type**:array
:::::::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items
:name: closed_features_items_oneOf_2_composition-models_items

::::::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf
:name: closed_features_items_oneOf_2_composition-models_items_oneOf

:::::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf/1
:name: closed_features_items_oneOf_2_composition-models_items_oneOf_1

- **type**:object
- **documentation**:Compositional model object
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf/1/model
:name: closed_features_items_oneOf_2_composition-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the composition model.
- **enum**:[smooth]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf/1/min distance fault center
:name: closed_features_items_oneOf_2_composition-models_items_oneOf_1_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf/1/side distance fault center
:name: closed_features_items_oneOf_2_composition-models_items_oneOf_1_side-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance over which the composition is reduced from 1 to 0.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf/1/center fractions
:name: closed_features_items_oneOf_2_composition-models_items_oneOf_1_center-fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:The composition fraction at the center of the fault.
:::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf/1/center fractions/items
:name: closed_features_items_oneOf_2_composition-models_items_oneOf_1_center-fractions_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf/1/side fractions
:name: closed_features_items_oneOf_2_composition-models_items_oneOf_1_side-fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:The composition fraction at the sides of this feature.
:::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf/1/side fractions/items
:name: closed_features_items_oneOf_2_composition-models_items_oneOf_1_side-fractions_items

- **default value**:0.0
- **type**:number
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_2_composition-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the composition which are present there.
:::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_2_composition-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf/1/operation
:name: closed_features_items_oneOf_2_composition-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value. Replacing implies that all compositions not explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.
- **enum**:[replace, replace defined only, add, subtract]
::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf/2
:name: closed_features_items_oneOf_2_composition-models_items_oneOf_2

- **type**:object
- **documentation**:Uniform compositional model. Sets constant compositional field.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf/2/model
:name: closed_features_items_oneOf_2_composition-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the composition model.
- **enum**:[uniform]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf/2/min distance fault center
:name: closed_features_items_oneOf_2_composition-models_items_oneOf_2_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf/2/max distance fault center
:name: closed_features_items_oneOf_2_composition-models_items_oneOf_2_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance in meters to which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf/2/compositions
:name: closed_features_items_oneOf_2_composition-models_items_oneOf_2_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the composition which are present there.
:::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf/2/compositions/items
:name: closed_features_items_oneOf_2_composition-models_items_oneOf_2_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf/2/fractions
:name: closed_features_items_oneOf_2_composition-models_items_oneOf_2_fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:TA list of compositional fractions corresponding to the compositions list.
:::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf/2/fractions/items
:name: closed_features_items_oneOf_2_composition-models_items_oneOf_2_fractions_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/composition models/items/oneOf/2/operation
:name: closed_features_items_oneOf_2_composition-models_items_oneOf_2_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value. Replacing implies that all compositions not explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.
- **enum**:[replace, replace defined only, add, subtract]
::::::::::::::::



:::::::::::::::::


:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/2/grains models
:name: closed_features_items_oneOf_2_grains-models

- **documentation**:A list of grains models.
- **default value**:
- **type**:array
:::::::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items
:name: closed_features_items_oneOf_2_grains-models_items

::::::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf
:name: closed_features_items_oneOf_2_grains-models_items_oneOf

:::::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/1
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_1

- **type**:object
- **documentation**:Random uniform distribution grains model. The size of the grains can be independently set to a single value or to a random distribution.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/1/model
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[random uniform distribution]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/1/min distance fault center
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_1_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The distance from the fault center in meters from which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/1/max distance fault center
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_1_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance from the fault in meters to which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/1/orientation operation
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_1_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/1/grain sizes
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_1_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be randomized between 0 and 1.
:::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/1/grain sizes/items
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_1_grain-sizes_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/1/normalize grain sizes
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_1_normalize-grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of whether the sizes of the grains should be normalized or not. If normalized, the total of the grains of a composition will be equal to 1.
:::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/1/normalize grain sizes/items
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_1_normalize-grain-sizes_items

- **default value**:true
- **type**:boolean
- **documentation**:
:::::::::::::::

::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/2
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_2

- **type**:object
- **documentation**:Uniform grains model. All grains start exactly the same.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/2/model
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[uniform]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/2/min distance fault center
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_2_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The distance from the fault center in meters from which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/2/max distance fault center
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_2_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance from the fault in meters to which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/2/compositions
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_2_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/2/compositions/items
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_2_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/2/rotation matrices
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_2_rotation-matrices

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the grains which are present there for each compositions.
:::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/2/rotation matrices/items
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_2_rotation-matrices_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/2/rotation matrices/items/items
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_2_rotation-matrices_items_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/2/rotation matrices/items/items/items
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_2_rotation-matrices_items_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/2/Euler angles z-x-z
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_2_Euler-angles-z-x-z

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the z-x-z Euler angles of the grains which are present there for each compositions.
:::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/2/Euler angles z-x-z/items
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_2_Euler-angles-z-x-z_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/2/Euler angles z-x-z/items/items
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_2_Euler-angles-z-x-z_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/2/orientation operation
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_2_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/2/grain sizes
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_2_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be set so that the total is equal to 1.
:::::::::::::::{dropdown} /features/items/oneOf/2/grains models/items/oneOf/2/grain sizes/items
:name: closed_features_items_oneOf_2_grains-models_items_oneOf_2_grain-sizes_items

- **default value**:-1.0
- **type**:number
- **documentation**:
:::::::::::::::

::::::::::::::::



:::::::::::::::::


:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/2/sections
:name: closed_features_items_oneOf_2_sections

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of feature properties for a coordinate.
:::::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items
:name: closed_features_items_oneOf_2_sections_items

- **documentation**:
- **default value**:
- **type**:object

::::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/min depth
:name: closed_features_items_oneOf_2_sections_items_min-depth

- **default value**:0.0
- **type**:number
- **documentation**:The depth to which this feature is present
::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/max depth
:name: closed_features_items_oneOf_2_sections_items_max-depth

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The depth to which this feature is present
::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/dip point
:name: closed_features_items_oneOf_2_sections_items_dip-point

- **type**:array
- **minItems**:2
- **maxItems**:2
- **documentation**:The depth to which this feature is present
:::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/dip point/items
:name: closed_features_items_oneOf_2_sections_items_dip-point_items

- **type**:number
:::::::::::::::::

::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments
:name: closed_features_items_oneOf_2_sections_items_segments

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:The depth to which this feature is present
:::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items
:name: closed_features_items_oneOf_2_sections_items_segments_items

- **type**:object
- **additionalProperties**:false
- **documentation**:
- **required**:[length, thickness, angle]

::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/length
:name: closed_features_items_oneOf_2_sections_items_segments_items_length

- **type**:number
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/thickness
:name: closed_features_items_oneOf_2_sections_items_segments_items_thickness

- **type**:array
- **minItems**:1
- **maxItems**:2
:::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/thickness/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_thickness_items

- **type**:number
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/top truncation
:name: closed_features_items_oneOf_2_sections_items_segments_items_top-truncation

- **type**:array
- **minItems**:1
- **maxItems**:2
:::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/top truncation/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_top-truncation_items

- **type**:number
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/angle
:name: closed_features_items_oneOf_2_sections_items_segments_items_angle

- **type**:array
- **minItems**:1
- **maxItems**:2
:::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/angle/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_angle_items

- **type**:number
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models

- **documentation**:
- **default value**:
- **type**:array
:::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf

:::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/1
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_1

- **type**:object
- **documentation**:Adiabatic temperature model. Uses global values by default.
- **additionalProperties**:false
- **required**:[model]

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/1/model
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[adiabatic]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/1/operation
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/1/min distance fault center
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_1_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/1/max distance fault center
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_1_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/1/potential mantle temperature
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_1_potential-mantle-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The potential temperature of the mantle at the surface in Kelvin. If the value is lower then zero, the global value is used.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/1/thermal expansion coefficient
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_1_thermal-expansion-coefficient

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal expansion coefficient in $K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/1/specific heat
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_1_specific-heat

- **default value**:-1.0
- **type**:number
- **documentation**:The specific heat in $J kg^{-1} K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::



:::::::::::::

:::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/2
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_2

- **type**:object
- **documentation**:Linear temperature model. Can be set to use an adiabatic temperature at the boundaries.
- **additionalProperties**:false
- **required**:[model, max distance fault center]

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/2/model
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[linear]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/2/operation
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_2_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/2/min distance fault center
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_2_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The minimum distance to the center of the fault. This determines where the linear temperature starts.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/2/max distance fault center
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_2_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The minimum distance to the center of the fault. This determines where the linear temperature end.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/2/center temperature
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_2_center-temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature at the center of this feature in degree Kelvin.If the value is below zero, the an adiabatic temperature is used.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/2/side temperature
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_2_side-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The temperature at the sides of this feature in degree Kelvin. If the value is below zero, an adiabatic temperature is used.
::::::::::::



:::::::::::::

:::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/3
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_3

- **type**:object
- **documentation**:Uniform temperature model. Set the temperature to a constan value.
- **additionalProperties**:false
- **required**:[model, temperature]

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/3/model
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_3_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[uniform]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/3/operation
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_3_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/3/min distance fault center
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_3_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/3/max distance fault center
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_3_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance in meters to which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/temperature models/items/oneOf/3/temperature
:name: closed_features_items_oneOf_2_sections_items_segments_items_temperature-models_items_oneOf_3_temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature in degree Kelvin which this feature should have
::::::::::::



:::::::::::::


:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models

- **documentation**:
- **default value**:
- **type**:array
:::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf

:::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf/1
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf_1

- **type**:object
- **documentation**:Compositional model object
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf/1/model
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the composition model.
- **enum**:[smooth]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf/1/min distance fault center
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf_1_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf/1/side distance fault center
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf_1_side-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance over which the composition is reduced from 1 to 0.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf/1/center fractions
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf_1_center-fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:The composition fraction at the center of the fault.
:::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf/1/center fractions/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf_1_center-fractions_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf/1/side fractions
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf_1_side-fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:The composition fraction at the sides of this feature.
:::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf/1/side fractions/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf_1_side-fractions_items

- **default value**:0.0
- **type**:number
- **documentation**:
:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the composition which are present there.
:::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf/1/operation
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value. Replacing implies that all compositions not explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.
- **enum**:[replace, replace defined only, add, subtract]
::::::::::::



:::::::::::::

:::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf/2
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf_2

- **type**:object
- **documentation**:Uniform compositional model. Sets constant compositional field.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf/2/model
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the composition model.
- **enum**:[uniform]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf/2/min distance fault center
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf_2_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf/2/max distance fault center
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf_2_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance in meters to which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf/2/compositions
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf_2_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the composition which are present there.
:::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf/2/compositions/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf_2_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf/2/fractions
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf_2_fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:TA list of compositional fractions corresponding to the compositions list.
:::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf/2/fractions/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf_2_fractions_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/composition models/items/oneOf/2/operation
:name: closed_features_items_oneOf_2_sections_items_segments_items_composition-models_items_oneOf_2_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value. Replacing implies that all compositions not explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.
- **enum**:[replace, replace defined only, add, subtract]
::::::::::::



:::::::::::::


:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models

- **documentation**:
- **default value**:
- **type**:array
:::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf

:::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/1
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_1

- **type**:object
- **documentation**:Random uniform distribution grains model. The size of the grains can be independently set to a single value or to a random distribution.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/1/model
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[random uniform distribution]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/1/min distance fault center
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_1_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The distance from the fault center in meters from which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/1/max distance fault center
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_1_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance from the fault in meters to which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/1/orientation operation
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_1_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/1/grain sizes
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_1_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be randomized between 0 and 1.
:::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/1/grain sizes/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_1_grain-sizes_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/1/normalize grain sizes
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_1_normalize-grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of whether the sizes of the grains should be normalized or not. If normalized, the total of the grains of a composition will be equal to 1.
:::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/1/normalize grain sizes/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_1_normalize-grain-sizes_items

- **default value**:true
- **type**:boolean
- **documentation**:
:::::::::::

::::::::::::



:::::::::::::

:::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/2
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_2

- **type**:object
- **documentation**:Uniform grains model. All grains start exactly the same.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/2/model
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[uniform]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/2/min distance fault center
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_2_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The distance from the fault center in meters from which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/2/max distance fault center
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_2_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance from the fault in meters to which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/2/compositions
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_2_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/2/compositions/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_2_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/2/rotation matrices
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_2_rotation-matrices

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the grains which are present there for each compositions.
:::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/2/rotation matrices/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_2_rotation-matrices_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/2/rotation matrices/items/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_2_rotation-matrices_items_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
:::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/2/rotation matrices/items/items/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_2_rotation-matrices_items_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
:::::::::

::::::::::

:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/2/Euler angles z-x-z
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_2_Euler-angles-z-x-z

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the z-x-z Euler angles of the grains which are present there for each compositions.
:::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/2/Euler angles z-x-z/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_2_Euler-angles-z-x-z_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/2/Euler angles z-x-z/items/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_2_Euler-angles-z-x-z_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::

:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/2/orientation operation
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_2_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/2/grain sizes
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_2_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be set so that the total is equal to 1.
:::::::::::{dropdown} /features/items/oneOf/2/sections/items/segments/items/grains models/items/oneOf/2/grain sizes/items
:name: closed_features_items_oneOf_2_sections_items_segments_items_grains-models_items_oneOf_2_grain-sizes_items

- **default value**:-1.0
- **type**:number
- **documentation**:
:::::::::::

::::::::::::



:::::::::::::


:::::::::::::::

::::::::::::::::



:::::::::::::::::

::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models
:name: closed_features_items_oneOf_2_sections_items_temperature-models

- **documentation**:A list of temperature models.
- **default value**:
- **type**:array
:::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items

::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf

:::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/1
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_1

- **type**:object
- **documentation**:Adiabatic temperature model. Uses global values by default.
- **additionalProperties**:false
- **required**:[model]

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/1/model
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[adiabatic]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/1/operation
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/1/min distance fault center
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_1_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/1/max distance fault center
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_1_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/1/potential mantle temperature
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_1_potential-mantle-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The potential temperature of the mantle at the surface in Kelvin. If the value is lower then zero, the global value is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/1/thermal expansion coefficient
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_1_thermal-expansion-coefficient

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal expansion coefficient in $K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/1/specific heat
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_1_specific-heat

- **default value**:-1.0
- **type**:number
- **documentation**:The specific heat in $J kg^{-1} K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::::



:::::::::::::::

:::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/2
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_2

- **type**:object
- **documentation**:Linear temperature model. Can be set to use an adiabatic temperature at the boundaries.
- **additionalProperties**:false
- **required**:[model, max distance fault center]

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/2/model
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[linear]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/2/operation
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_2_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/2/min distance fault center
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_2_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The minimum distance to the center of the fault. This determines where the linear temperature starts.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/2/max distance fault center
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_2_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The minimum distance to the center of the fault. This determines where the linear temperature end.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/2/center temperature
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_2_center-temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature at the center of this feature in degree Kelvin.If the value is below zero, the an adiabatic temperature is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/2/side temperature
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_2_side-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The temperature at the sides of this feature in degree Kelvin. If the value is below zero, an adiabatic temperature is used.
::::::::::::::



:::::::::::::::

:::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/3
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_3

- **type**:object
- **documentation**:Uniform temperature model. Set the temperature to a constan value.
- **additionalProperties**:false
- **required**:[model, temperature]

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/3/model
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_3_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[uniform]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/3/operation
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_3_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/3/min distance fault center
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_3_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/3/max distance fault center
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_3_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/temperature models/items/oneOf/3/temperature
:name: closed_features_items_oneOf_2_sections_items_temperature-models_items_oneOf_3_temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature in degree Kelvin which this feature should have
::::::::::::::



:::::::::::::::


:::::::::::::::::

::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models
:name: closed_features_items_oneOf_2_sections_items_composition-models

- **documentation**:A list of composition models.
- **default value**:
- **type**:array
:::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items
:name: closed_features_items_oneOf_2_sections_items_composition-models_items

::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf

:::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf/1
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf_1

- **type**:object
- **documentation**:Compositional model object
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf/1/model
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the composition model.
- **enum**:[smooth]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf/1/min distance fault center
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf_1_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf/1/side distance fault center
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf_1_side-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance over which the composition is reduced from 1 to 0.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf/1/center fractions
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf_1_center-fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:The composition fraction at the center of the fault.
:::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf/1/center fractions/items
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf_1_center-fractions_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf/1/side fractions
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf_1_side-fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:The composition fraction at the sides of this feature.
:::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf/1/side fractions/items
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf_1_side-fractions_items

- **default value**:0.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the composition which are present there.
:::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf/1/operation
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value. Replacing implies that all compositions not explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.
- **enum**:[replace, replace defined only, add, subtract]
::::::::::::::



:::::::::::::::

:::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf/2
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf_2

- **type**:object
- **documentation**:Uniform compositional model. Sets constant compositional field.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf/2/model
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the composition model.
- **enum**:[uniform]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf/2/min distance fault center
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf_2_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf/2/max distance fault center
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf_2_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf/2/compositions
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf_2_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the composition which are present there.
:::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf/2/compositions/items
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf_2_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf/2/fractions
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf_2_fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:TA list of compositional fractions corresponding to the compositions list.
:::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf/2/fractions/items
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf_2_fractions_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/composition models/items/oneOf/2/operation
:name: closed_features_items_oneOf_2_sections_items_composition-models_items_oneOf_2_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value. Replacing implies that all compositions not explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.
- **enum**:[replace, replace defined only, add, subtract]
::::::::::::::



:::::::::::::::


:::::::::::::::::

::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models
:name: closed_features_items_oneOf_2_sections_items_grains-models

- **documentation**:A list of grains models.
- **default value**:
- **type**:array
:::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items
:name: closed_features_items_oneOf_2_sections_items_grains-models_items

::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf

:::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/1
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_1

- **type**:object
- **documentation**:Random uniform distribution grains model. The size of the grains can be independently set to a single value or to a random distribution.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/1/model
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[random uniform distribution]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/1/min distance fault center
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_1_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The distance from the fault center in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/1/max distance fault center
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_1_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance from the fault in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/1/orientation operation
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_1_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/1/grain sizes
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_1_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be randomized between 0 and 1.
:::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/1/grain sizes/items
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_1_grain-sizes_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/1/normalize grain sizes
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_1_normalize-grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of whether the sizes of the grains should be normalized or not. If normalized, the total of the grains of a composition will be equal to 1.
:::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/1/normalize grain sizes/items
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_1_normalize-grain-sizes_items

- **default value**:true
- **type**:boolean
- **documentation**:
:::::::::::::

::::::::::::::



:::::::::::::::

:::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/2
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_2

- **type**:object
- **documentation**:Uniform grains model. All grains start exactly the same.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/2/model
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[uniform]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/2/min distance fault center
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_2_min-distance-fault-center

- **default value**:0.0
- **type**:number
- **documentation**:The distance from the fault center in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/2/max distance fault center
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_2_max-distance-fault-center

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance from the fault in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/2/compositions
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_2_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/2/compositions/items
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_2_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/2/rotation matrices
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_2_rotation-matrices

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the grains which are present there for each compositions.
:::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/2/rotation matrices/items
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_2_rotation-matrices_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/2/rotation matrices/items/items
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_2_rotation-matrices_items_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
:::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/2/rotation matrices/items/items/items
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_2_rotation-matrices_items_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
:::::::::::

::::::::::::

:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/2/Euler angles z-x-z
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_2_Euler-angles-z-x-z

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the z-x-z Euler angles of the grains which are present there for each compositions.
:::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/2/Euler angles z-x-z/items
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_2_Euler-angles-z-x-z_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/2/Euler angles z-x-z/items/items
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_2_Euler-angles-z-x-z_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::

:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/2/orientation operation
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_2_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/2/grain sizes
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_2_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be set so that the total is equal to 1.
:::::::::::::{dropdown} /features/items/oneOf/2/sections/items/grains models/items/oneOf/2/grain sizes/items
:name: closed_features_items_oneOf_2_sections_items_grains-models_items_oneOf_2_grain-sizes_items

- **default value**:-1.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::



:::::::::::::::


:::::::::::::::::

::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/2/sections/items/coordinate
:name: closed_features_items_oneOf_2_sections_items_coordinate

- **default value**:0
- **type**:integer
- **documentation**:The coordinate which should be overwritten
::::::::::::::::::



:::::::::::::::::::

::::::::::::::::::::



:::::::::::::::::::::

:::::::::::::::::::::{dropdown} /features/items/oneOf/3
:name: closed_features_items_oneOf_3

- **type**:object
- **documentation**:Mantle layer object. Requires properties `model` and `coordinates`.
- **additionalProperties**:false
- **required**:[model]

::::::::::::::::::::{dropdown} /features/items/oneOf/3/model
:name: closed_features_items_oneOf_3_model

- **default value**:
- **type**:string
- **documentation**:The name which the user has given to the feature.
- **enum**:[mantle layer]
::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/3/name
:name: closed_features_items_oneOf_3_name

- **default value**:
- **type**:string
- **documentation**:The name which the user has given to the feature.
::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/3/coordinates
:name: closed_features_items_oneOf_3_coordinates

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:An array of 2d Points representing an array of coordinates where the feature is located.
:::::::::::::::::::{dropdown} /features/items/oneOf/3/coordinates/items
:name: closed_features_items_oneOf_3_coordinates_items

- **type**:array
- **minItems**:2
- **maxItems**:2
- **documentation**:
::::::::::::::::::{dropdown} /features/items/oneOf/3/coordinates/items/items
:name: closed_features_items_oneOf_3_coordinates_items_items

- **type**:number
::::::::::::::::::

:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/3/interpolation
:name: closed_features_items_oneOf_3_interpolation

- **default value**:global
- **type**:string
- **documentation**:What type of interpolation should be used to enforce the minimum points per distance parameter. Options are global, none, linear, monotone spline and continuous monotone spline interpolation. If this value is set to global, the global value for interpolation is used.
::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/3/min depth
:name: closed_features_items_oneOf_3_min-depth

- **documentation**:The depth from which this feature is present
:::::::::::::::::::{dropdown} /features/items/oneOf/3/min depth/oneOf
:name: closed_features_items_oneOf_3_min-depth_oneOf

::::::::::::::::::{dropdown} /features/items/oneOf/3/min depth/oneOf/1
:name: closed_features_items_oneOf_3_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/3/min depth/oneOf/2
:name: closed_features_items_oneOf_3_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::::::{dropdown} /features/items/oneOf/3/min depth/oneOf/2/items
:name: closed_features_items_oneOf_3_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::::::{dropdown} /features/items/oneOf/3/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_3_min-depth_oneOf_2_items_items

:::::::::::::::{dropdown} /features/items/oneOf/3/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_3_min-depth_oneOf_2_items_items_anyOf

::::::::::::::{dropdown} /features/items/oneOf/3/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_3_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/3/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_3_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::::::{dropdown} /features/items/oneOf/3/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_3_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::::::{dropdown} /features/items/oneOf/3/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_3_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

:::::::::::::::::

::::::::::::::::::


::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/3/max depth
:name: closed_features_items_oneOf_3_max-depth

- **documentation**:The depth to which this feature is present
:::::::::::::::::::{dropdown} /features/items/oneOf/3/max depth/oneOf
:name: closed_features_items_oneOf_3_max-depth_oneOf

::::::::::::::::::{dropdown} /features/items/oneOf/3/max depth/oneOf/1
:name: closed_features_items_oneOf_3_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/3/max depth/oneOf/2
:name: closed_features_items_oneOf_3_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::::::{dropdown} /features/items/oneOf/3/max depth/oneOf/2/items
:name: closed_features_items_oneOf_3_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::::::{dropdown} /features/items/oneOf/3/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_3_max-depth_oneOf_2_items_items

:::::::::::::::{dropdown} /features/items/oneOf/3/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_3_max-depth_oneOf_2_items_items_anyOf

::::::::::::::{dropdown} /features/items/oneOf/3/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_3_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/3/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_3_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::::::{dropdown} /features/items/oneOf/3/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_3_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::::::{dropdown} /features/items/oneOf/3/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_3_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

:::::::::::::::::

::::::::::::::::::


::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models
:name: closed_features_items_oneOf_3_temperature-models

- **documentation**:A list of temperature models.
- **default value**:
- **type**:array
:::::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items
:name: closed_features_items_oneOf_3_temperature-models_items

::::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf

:::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1

- **type**:object
- **documentation**:Adiabatic temperature model. Uses global values by default.
- **additionalProperties**:false
- **required**:[model]

::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/model
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[adiabatic]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/operation
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/min depth
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_min-depth

- **documentation**:The depth in meters from which the temperature of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/min depth/oneOf
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/min depth/oneOf/1
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/min depth/oneOf/2
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/min depth/oneOf/2/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/max depth
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_max-depth

- **documentation**:The depth in meters to which the temperature of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/max depth/oneOf
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/max depth/oneOf/1
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/max depth/oneOf/2
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/max depth/oneOf/2/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/potential mantle temperature
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_potential-mantle-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The potential temperature of the mantle at the surface in Kelvin. If the value is lower then zero, the global value is used.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/thermal expansion coefficient
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_thermal-expansion-coefficient

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal expansion coefficient in $K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/1/specific heat
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_1_specific-heat

- **default value**:-1.0
- **type**:number
- **documentation**:The specific heat in $J kg^{-1} K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2

- **type**:object
- **documentation**:Linear temperature model. Can be set to use an adiabatic temperature at the boundaries.
- **additionalProperties**:false
- **required**:[model, max depth]

::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/model
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[linear]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/operation
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/min depth
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_min-depth

- **documentation**:The depth in meters from which the temperature of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/min depth/oneOf
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/min depth/oneOf/1
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/min depth/oneOf/2
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/min depth/oneOf/2/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/max depth
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_max-depth

- **documentation**:The depth in meters to which the temperature of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/max depth/oneOf
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/max depth/oneOf/1
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/max depth/oneOf/2
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/max depth/oneOf/2/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/top temperature
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_top-temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature at the top in degree Kelvin of this feature.If the value is below zero, the an adiabatic temperature is used.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/2/bottom temperature
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_2_bottom-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The temperature at the top in degree Kelvin of this feature. If the value is below zero, an adiabatic temperature is used.
::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3

- **type**:object
- **documentation**:Uniform temperature model. Set the temperature to a constan value.
- **additionalProperties**:false
- **required**:[model, temperature]

::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/model
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[uniform]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/operation
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/min depth
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_min-depth

- **documentation**:The depth in meters from which the temperature of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/min depth/oneOf
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/min depth/oneOf/1
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/min depth/oneOf/2
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/min depth/oneOf/2/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/max depth
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_max-depth

- **documentation**:The depth in meters to which the temperature of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/max depth/oneOf
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/max depth/oneOf/1
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/max depth/oneOf/2
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/max depth/oneOf/2/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/temperature models/items/oneOf/3/temperature
:name: closed_features_items_oneOf_3_temperature-models_items_oneOf_3_temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature in degree Kelvin which this feature should have
::::::::::::::::



:::::::::::::::::


:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/3/composition models
:name: closed_features_items_oneOf_3_composition-models

- **documentation**:A list of composition models.
- **default value**:
- **type**:array
:::::::::::::::::::{dropdown} /features/items/oneOf/3/composition models/items
:name: closed_features_items_oneOf_3_composition-models_items

::::::::::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf
:name: closed_features_items_oneOf_3_composition-models_items_oneOf

:::::::::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1

- **type**:object
- **documentation**:Uniform compositional model. Sets constant compositional field.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/model
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the composition model.
- **enum**:[uniform]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/min depth
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_min-depth

- **documentation**:The depth in meters from which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/min depth/oneOf
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/min depth/oneOf/1
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/min depth/oneOf/2
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/min depth/oneOf/2/items
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/max depth
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_max-depth

- **documentation**:The depth in meters to which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/max depth/oneOf
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/max depth/oneOf/1
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/max depth/oneOf/2
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/max depth/oneOf/2/items
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the composition which are present there.
:::::::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/fractions
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:TA list of compositional fractions corresponding to the compositions list.
:::::::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/fractions/items
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_fractions_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/composition models/items/oneOf/1/operation
:name: closed_features_items_oneOf_3_composition-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value. Replacing implies that all compositions not explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.
- **enum**:[replace, replace defined only, add, subtract]
::::::::::::::::



:::::::::::::::::


:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/3/grains models
:name: closed_features_items_oneOf_3_grains-models

- **documentation**:A list of grains models.
- **default value**:
- **type**:array
:::::::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items
:name: closed_features_items_oneOf_3_grains-models_items

::::::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf
:name: closed_features_items_oneOf_3_grains-models_items_oneOf

:::::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1

- **type**:object
- **documentation**:Random uniform distribution grains model. The size of the grains can be independently set to a single value or to a random distribution.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/model
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[random uniform distribution]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/min depth
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_min-depth

- **documentation**:The depth in meters from which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/min depth/oneOf
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/min depth/oneOf/1
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/min depth/oneOf/2
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/min depth/oneOf/2/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/max depth
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_max-depth

- **documentation**:The depth in meters to which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/max depth/oneOf
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/max depth/oneOf/1
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/max depth/oneOf/2
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/max depth/oneOf/2/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/orientation operation
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/grain sizes
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be randomized between 0 and 1.
:::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/grain sizes/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_grain-sizes_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/normalize grain sizes
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_normalize-grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of whether the sizes of the grains should be normalized or not. If normalized, the total of the grains of a composition will be equal to 1.
:::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/1/normalize grain sizes/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_1_normalize-grain-sizes_items

- **default value**:true
- **type**:boolean
- **documentation**:
:::::::::::::::

::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2

- **type**:object
- **documentation**:Uniform grains model. All grains start exactly the same.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/model
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[uniform]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/min depth
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_min-depth

- **documentation**:The depth in meters from which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/min depth/oneOf
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/min depth/oneOf/1
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/min depth/oneOf/2
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/min depth/oneOf/2/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/max depth
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_max-depth

- **documentation**:The depth in meters to which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/max depth/oneOf
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/max depth/oneOf/1
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/max depth/oneOf/2
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/max depth/oneOf/2/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/compositions
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/compositions/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/rotation matrices
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_rotation-matrices

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the grains which are present there for each compositions.
:::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/rotation matrices/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_rotation-matrices_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/rotation matrices/items/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_rotation-matrices_items_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/rotation matrices/items/items/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_rotation-matrices_items_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/Euler angles z-x-z
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_Euler-angles-z-x-z

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the z-x-z Euler angles of the grains which are present there for each compositions.
:::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/Euler angles z-x-z/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_Euler-angles-z-x-z_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/Euler angles z-x-z/items/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_Euler-angles-z-x-z_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/orientation operation
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/grain sizes
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be set so that the total is equal to 1.
:::::::::::::::{dropdown} /features/items/oneOf/3/grains models/items/oneOf/2/grain sizes/items
:name: closed_features_items_oneOf_3_grains-models_items_oneOf_2_grain-sizes_items

- **default value**:-1.0
- **type**:number
- **documentation**:
:::::::::::::::

::::::::::::::::



:::::::::::::::::


:::::::::::::::::::

::::::::::::::::::::



:::::::::::::::::::::

:::::::::::::::::::::{dropdown} /features/items/oneOf/4
:name: closed_features_items_oneOf_4

- **type**:object
- **documentation**:Oceanic plate object. Requires properties `model` and `coordinates`.
- **additionalProperties**:false
- **required**:[model]

::::::::::::::::::::{dropdown} /features/items/oneOf/4/model
:name: closed_features_items_oneOf_4_model

- **default value**:
- **type**:string
- **documentation**:The name which the user has given to the feature.
- **enum**:[oceanic plate]
::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/4/name
:name: closed_features_items_oneOf_4_name

- **default value**:
- **type**:string
- **documentation**:The name which the user has given to the feature.
::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/4/coordinates
:name: closed_features_items_oneOf_4_coordinates

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:An array of 2d Points representing an array of coordinates where the feature is located.
:::::::::::::::::::{dropdown} /features/items/oneOf/4/coordinates/items
:name: closed_features_items_oneOf_4_coordinates_items

- **type**:array
- **minItems**:2
- **maxItems**:2
- **documentation**:
::::::::::::::::::{dropdown} /features/items/oneOf/4/coordinates/items/items
:name: closed_features_items_oneOf_4_coordinates_items_items

- **type**:number
::::::::::::::::::

:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/4/interpolation
:name: closed_features_items_oneOf_4_interpolation

- **default value**:global
- **type**:string
- **documentation**:What type of interpolation should be used to enforce the minimum points per distance parameter. Options are global, none, linear, monotone spline and continuous monotone spline interpolation. If this value is set to global, the global value for interpolation is used.
::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/4/min depth
:name: closed_features_items_oneOf_4_min-depth

- **documentation**:The depth from which this feature is present
:::::::::::::::::::{dropdown} /features/items/oneOf/4/min depth/oneOf
:name: closed_features_items_oneOf_4_min-depth_oneOf

::::::::::::::::::{dropdown} /features/items/oneOf/4/min depth/oneOf/1
:name: closed_features_items_oneOf_4_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/4/min depth/oneOf/2
:name: closed_features_items_oneOf_4_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::::::{dropdown} /features/items/oneOf/4/min depth/oneOf/2/items
:name: closed_features_items_oneOf_4_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::::::{dropdown} /features/items/oneOf/4/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_4_min-depth_oneOf_2_items_items

:::::::::::::::{dropdown} /features/items/oneOf/4/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_4_min-depth_oneOf_2_items_items_anyOf

::::::::::::::{dropdown} /features/items/oneOf/4/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_4_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/4/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_4_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::::::{dropdown} /features/items/oneOf/4/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_4_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::::::{dropdown} /features/items/oneOf/4/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_4_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

:::::::::::::::::

::::::::::::::::::


::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/4/max depth
:name: closed_features_items_oneOf_4_max-depth

- **documentation**:The depth to which this feature is present
:::::::::::::::::::{dropdown} /features/items/oneOf/4/max depth/oneOf
:name: closed_features_items_oneOf_4_max-depth_oneOf

::::::::::::::::::{dropdown} /features/items/oneOf/4/max depth/oneOf/1
:name: closed_features_items_oneOf_4_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/4/max depth/oneOf/2
:name: closed_features_items_oneOf_4_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::::::{dropdown} /features/items/oneOf/4/max depth/oneOf/2/items
:name: closed_features_items_oneOf_4_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::::::{dropdown} /features/items/oneOf/4/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_4_max-depth_oneOf_2_items_items

:::::::::::::::{dropdown} /features/items/oneOf/4/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_4_max-depth_oneOf_2_items_items_anyOf

::::::::::::::{dropdown} /features/items/oneOf/4/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_4_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/4/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_4_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::::::{dropdown} /features/items/oneOf/4/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_4_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::::::{dropdown} /features/items/oneOf/4/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_4_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

:::::::::::::::::

::::::::::::::::::


::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models
:name: closed_features_items_oneOf_4_temperature-models

- **documentation**:A list of temperature models.
- **default value**:
- **type**:array
:::::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items
:name: closed_features_items_oneOf_4_temperature-models_items

::::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf

:::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1

- **type**:object
- **documentation**:Adiabatic temperature model. Uses global values by default.
- **additionalProperties**:false
- **required**:[model]

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/model
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[adiabatic]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/operation
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/min depth
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_min-depth

- **documentation**:The depth in meters from which the temperature of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/min depth/oneOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/min depth/oneOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/min depth/oneOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/min depth/oneOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/max depth
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_max-depth

- **documentation**:The depth in meters to which the temperature of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/max depth/oneOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/max depth/oneOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/max depth/oneOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/max depth/oneOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/potential mantle temperature
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_potential-mantle-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The potential temperature of the mantle at the surface in Kelvin. If the value is lower then zero, the global value is used.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/thermal expansion coefficient
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_thermal-expansion-coefficient

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal expansion coefficient in $K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/1/specific heat
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_1_specific-heat

- **default value**:-1.0
- **type**:number
- **documentation**:The specific heat in $J kg^{-1} K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2

- **type**:object
- **documentation**:Half space cooling mode
- **additionalProperties**:false
- **required**:[model, ridge coordinates, spreading velocity, max depth]

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/model
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[half space model]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/operation
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/min depth
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_min-depth

- **documentation**:The depth in meters from which the temperature of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/min depth/oneOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/min depth/oneOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/min depth/oneOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/min depth/oneOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/max depth
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_max-depth

- **documentation**:The depth in meters to which the temperature of this feature is present.Because half-space reaches background temperature asymptotically, this value should be ~2 times the nominal plate thickness of 100 km
:::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/max depth/oneOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/max depth/oneOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/max depth/oneOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/max depth/oneOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/top temperature
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_top-temperature

- **default value**:293.15
- **type**:number
- **documentation**:The actual surface temperature in degree Kelvin for this feature.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/bottom temperature
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_bottom-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The mantle temperature for the half-space cooling modelin degree Kelvin for this feature. If the model has an adiabatic gradientthis should be the mantle potential temperature, and T = Tad + Thalf. 
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/spreading velocity
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_spreading-velocity

- **default value**:-1.0
- **type**:number
- **documentation**:The spreading velocity of the plate in meter per year. This is the velocity with which one side moves away from the ridge.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/ridge coordinates
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_ridge-coordinates

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:An list of ridges. Each ridge is a lists of at least 2 2d points which define the location of the ridge. You need to define at least one ridge.So the an example with two ridges is [[[10,20],[20,30],[10,40]],[[50,10],[60,10]]].
:::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/ridge coordinates/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_ridge-coordinates_items

- **type**:array
- **minItems**:2
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/ridge coordinates/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_ridge-coordinates_items_items

- **type**:array
- **minItems**:2
- **maxItems**:2
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/2/ridge coordinates/items/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_2_ridge-coordinates_items_items_items

- **type**:number
:::::::::::::

::::::::::::::

:::::::::::::::

::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3

- **type**:object
- **documentation**:Linear temperature model. Can be set to use an adiabatic temperature at the boundaries.
- **additionalProperties**:false
- **required**:[model, max depth]

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/model
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[linear]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/operation
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/min depth
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_min-depth

- **documentation**:The depth in meters from which the temperature of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/min depth/oneOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/min depth/oneOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/min depth/oneOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/min depth/oneOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/max depth
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_max-depth

- **documentation**:The depth in meters to which the temperature of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/max depth/oneOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/max depth/oneOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/max depth/oneOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/max depth/oneOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/top temperature
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_top-temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature at the top in degree Kelvin of this feature.If the value is below zero, the an adiabatic temperature is used.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/3/bottom temperature
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_3_bottom-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The temperature at the top in degree Kelvin of this feature. If the value is below zero, an adiabatic temperature is used.
::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4

- **type**:object
- **documentation**:Plate model.
- **additionalProperties**:false
- **required**:[model, max depth]

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/model
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[plate model]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/operation
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/min depth
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_min-depth

- **documentation**:The depth in meters from which the temperature of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/min depth/oneOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/min depth/oneOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/min depth/oneOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/min depth/oneOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/max depth
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_max-depth

- **documentation**:The depth in meters to which the temperature of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/max depth/oneOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/max depth/oneOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/max depth/oneOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/max depth/oneOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/top temperature
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_top-temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature in degree Kelvin which this feature should have
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/bottom temperature
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_bottom-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The temperature in degree Kelvin which this feature should have
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/spreading velocity
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_spreading-velocity

- **default value**:-1.0
- **type**:number
- **documentation**:The spreading velocity of the plate in meter per year. This is the velocity with which one side moves away from the ridge.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/ridge coordinates
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_ridge-coordinates

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:An list of ridges. Each ridge is a lists of at least 2 2d points which define the location of the ridge. You need to define at least one ridge.So the an example with two ridges is [[[10,20],[20,30],[10,40]],[[50,10],[60,10]]].
:::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/ridge coordinates/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_ridge-coordinates_items

- **type**:array
- **minItems**:2
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/ridge coordinates/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_ridge-coordinates_items_items

- **type**:array
- **minItems**:2
- **maxItems**:2
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/4/ridge coordinates/items/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_4_ridge-coordinates_items_items_items

- **type**:number
:::::::::::::

::::::::::::::

:::::::::::::::

::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5

- **type**:object
- **documentation**:Plate model, but with a fixed age.
- **additionalProperties**:false
- **required**:[model, max depth]

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/model
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[plate model constant age]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/operation
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/min depth
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_min-depth

- **documentation**:The depth in meters from which the temperature of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/min depth/oneOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/min depth/oneOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/min depth/oneOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/min depth/oneOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/max depth
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_max-depth

- **documentation**:The depth in meters to which the temperature of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/max depth/oneOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/max depth/oneOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/max depth/oneOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/max depth/oneOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/top temperature
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_top-temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature in degree Kelvin which this feature should have
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/bottom temperature
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_bottom-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The temperature in degree Kelvin which this feature should have
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/5/plate age
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_5_plate-age

- **default value**:80000.0
- **type**:number
- **documentation**:The age of the plate in year. This age is assigned to the whole plate. 
::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6

- **type**:object
- **documentation**:Uniform temperature model. Set the temperature to a constan value.
- **additionalProperties**:false
- **required**:[model, temperature]

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/model
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[uniform]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/operation
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/min depth
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_min-depth

- **documentation**:The depth in meters from which the temperature of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/min depth/oneOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/min depth/oneOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/min depth/oneOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/min depth/oneOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/max depth
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_max-depth

- **documentation**:The depth in meters to which the temperature of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/max depth/oneOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/max depth/oneOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/max depth/oneOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/max depth/oneOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/temperature models/items/oneOf/6/temperature
:name: closed_features_items_oneOf_4_temperature-models_items_oneOf_6_temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature in degree Kelvin which this feature should have
::::::::::::::::



:::::::::::::::::


:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/4/composition models
:name: closed_features_items_oneOf_4_composition-models

- **documentation**:A list of composition models.
- **default value**:
- **type**:array
:::::::::::::::::::{dropdown} /features/items/oneOf/4/composition models/items
:name: closed_features_items_oneOf_4_composition-models_items

::::::::::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf
:name: closed_features_items_oneOf_4_composition-models_items_oneOf

:::::::::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1

- **type**:object
- **documentation**:Uniform compositional model. Sets constant compositional field.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/model
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the composition model.
- **enum**:[uniform]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/min depth
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_min-depth

- **documentation**:The depth in meters from which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/min depth/oneOf
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/min depth/oneOf/1
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/min depth/oneOf/2
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/min depth/oneOf/2/items
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/max depth
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_max-depth

- **documentation**:The depth in meters to which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/max depth/oneOf
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/max depth/oneOf/1
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/max depth/oneOf/2
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/max depth/oneOf/2/items
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the composition which are present there.
:::::::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/fractions
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:TA list of compositional fractions corresponding to the compositions list.
:::::::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/fractions/items
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_fractions_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/composition models/items/oneOf/1/operation
:name: closed_features_items_oneOf_4_composition-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value. Replacing implies that all compositions not explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.
- **enum**:[replace, replace defined only, add, subtract]
::::::::::::::::



:::::::::::::::::


:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/4/grains models
:name: closed_features_items_oneOf_4_grains-models

- **documentation**:A list of grains models.
- **default value**:
- **type**:array
:::::::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items
:name: closed_features_items_oneOf_4_grains-models_items

::::::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf
:name: closed_features_items_oneOf_4_grains-models_items_oneOf

:::::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1

- **type**:object
- **documentation**:Random uniform distribution grains model. The size of the grains can be independently set to a single value or to a random distribution.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/model
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[random uniform distribution]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/min depth
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_min-depth

- **documentation**:The depth in meters from which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/min depth/oneOf
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/min depth/oneOf/1
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/min depth/oneOf/2
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/min depth/oneOf/2/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/max depth
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_max-depth

- **documentation**:The depth in meters to which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/max depth/oneOf
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/max depth/oneOf/1
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/max depth/oneOf/2
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/max depth/oneOf/2/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/orientation operation
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/grain sizes
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be randomized between 0 and 1.
:::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/grain sizes/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_grain-sizes_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/normalize grain sizes
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_normalize-grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of whether the sizes of the grains should be normalized or not. If normalized, the total of the grains of a composition will be equal to 1.
:::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/1/normalize grain sizes/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_1_normalize-grain-sizes_items

- **default value**:true
- **type**:boolean
- **documentation**:
:::::::::::::::

::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2

- **type**:object
- **documentation**:Uniform grains model. All grains start exactly the same.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/model
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[uniform]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/min depth
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_min-depth

- **documentation**:The depth in meters from which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/min depth/oneOf
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_min-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/min depth/oneOf/1
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_min-depth_oneOf_1

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/min depth/oneOf/2
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_min-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/min depth/oneOf/2/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_min-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/min depth/oneOf/2/items/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_min-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:0.0
::::::::::

::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/min depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_min-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/max depth
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_max-depth

- **documentation**:The depth in meters to which the composition of this feature is present.
:::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/max depth/oneOf
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_max-depth_oneOf

::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/max depth/oneOf/1
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_max-depth_oneOf_1

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/max depth/oneOf/2
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_max-depth_oneOf_2

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/max depth/oneOf/2/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_max-depth_oneOf_2_items

- **type**:array
- **additionalProperties**:false
- **minItems**:1
- **maxItems**:2
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/max depth/oneOf/2/items/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_max-depth_oneOf_2_items_items

:::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf

::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/1
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_1

- **type**:number
- **default value**:1.7976931348623157e308
::::::::::

::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/2
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_2

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
:::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/2/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_2_items

- **type**:array
- **minItems**:1
- **maxItems**:2
::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/max depth/oneOf/2/items/items/anyOf/2/items/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_max-depth_oneOf_2_items_items_anyOf_2_items_items

- **type**:number
::::::::

:::::::::

::::::::::


::::::::::::

:::::::::::::

::::::::::::::


::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/compositions
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/compositions/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/rotation matrices
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_rotation-matrices

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the grains which are present there for each compositions.
:::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/rotation matrices/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_rotation-matrices_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/rotation matrices/items/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_rotation-matrices_items_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/rotation matrices/items/items/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_rotation-matrices_items_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/Euler angles z-x-z
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_Euler-angles-z-x-z

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the z-x-z Euler angles of the grains which are present there for each compositions.
:::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/Euler angles z-x-z/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_Euler-angles-z-x-z_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/Euler angles z-x-z/items/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_Euler-angles-z-x-z_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/orientation operation
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/grain sizes
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be set so that the total is equal to 1.
:::::::::::::::{dropdown} /features/items/oneOf/4/grains models/items/oneOf/2/grain sizes/items
:name: closed_features_items_oneOf_4_grains-models_items_oneOf_2_grain-sizes_items

- **default value**:-1.0
- **type**:number
- **documentation**:
:::::::::::::::

::::::::::::::::



:::::::::::::::::


:::::::::::::::::::

::::::::::::::::::::



:::::::::::::::::::::

:::::::::::::::::::::{dropdown} /features/items/oneOf/5
:name: closed_features_items_oneOf_5

- **type**:object
- **documentation**:Subducting slab object. Requires properties `model` and `coordinates`.
- **additionalProperties**:false
- **required**:[model]

::::::::::::::::::::{dropdown} /features/items/oneOf/5/model
:name: closed_features_items_oneOf_5_model

- **default value**:
- **type**:string
- **documentation**:The name which the user has given to the feature.
- **enum**:[subducting plate]
::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/5/name
:name: closed_features_items_oneOf_5_name

- **default value**:
- **type**:string
- **documentation**:The name which the user has given to the feature.
::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/5/coordinates
:name: closed_features_items_oneOf_5_coordinates

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:An array of 2d Points representing an array of coordinates where the feature is located.
:::::::::::::::::::{dropdown} /features/items/oneOf/5/coordinates/items
:name: closed_features_items_oneOf_5_coordinates_items

- **type**:array
- **minItems**:2
- **maxItems**:2
- **documentation**:
::::::::::::::::::{dropdown} /features/items/oneOf/5/coordinates/items/items
:name: closed_features_items_oneOf_5_coordinates_items_items

- **type**:number
::::::::::::::::::

:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/5/interpolation
:name: closed_features_items_oneOf_5_interpolation

- **default value**:global
- **type**:string
- **documentation**:What type of interpolation should be used to enforce the minimum points per distance parameter. Options are global, none, linear, monotone spline and continuous monotone spline interpolation. If this value is set to global, the global value for interpolation is used.
::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/5/min depth
:name: closed_features_items_oneOf_5_min-depth

- **default value**:0.0
- **type**:number
- **documentation**:The depth to which this feature is present
::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/5/max depth
:name: closed_features_items_oneOf_5_max-depth

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The depth to which this feature is present
::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/5/dip point
:name: closed_features_items_oneOf_5_dip-point

- **type**:array
- **minItems**:2
- **maxItems**:2
- **documentation**:The depth to which this feature is present
:::::::::::::::::::{dropdown} /features/items/oneOf/5/dip point/items
:name: closed_features_items_oneOf_5_dip-point_items

- **type**:number
:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/5/segments
:name: closed_features_items_oneOf_5_segments

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:The depth to which this feature is present
:::::::::::::::::::{dropdown} /features/items/oneOf/5/segments/items
:name: closed_features_items_oneOf_5_segments_items

- **type**:object
- **additionalProperties**:false
- **documentation**:
- **required**:[length, thickness, angle]

::::::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/length
:name: closed_features_items_oneOf_5_segments_items_length

- **type**:number
::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/thickness
:name: closed_features_items_oneOf_5_segments_items_thickness

- **type**:array
- **minItems**:1
- **maxItems**:2
:::::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/thickness/items
:name: closed_features_items_oneOf_5_segments_items_thickness_items

- **type**:number
:::::::::::::::::

::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/top truncation
:name: closed_features_items_oneOf_5_segments_items_top-truncation

- **type**:array
- **minItems**:1
- **maxItems**:2
:::::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/top truncation/items
:name: closed_features_items_oneOf_5_segments_items_top-truncation_items

- **type**:number
:::::::::::::::::

::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/angle
:name: closed_features_items_oneOf_5_segments_items_angle

- **type**:array
- **minItems**:1
- **maxItems**:2
:::::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/angle/items
:name: closed_features_items_oneOf_5_segments_items_angle_items

- **type**:number
:::::::::::::::::

::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models
:name: closed_features_items_oneOf_5_segments_items_temperature-models

- **documentation**:
- **default value**:
- **type**:array
:::::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items

::::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf

:::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/1
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_1

- **type**:object
- **documentation**:Adiabatic temperature model. Uses global values by default.
- **additionalProperties**:false
- **required**:[model]

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/1/model
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[adiabatic]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/1/operation
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/1/min distance slab top
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_1_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/1/max distance slab top
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_1_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/1/potential mantle temperature
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_1_potential-mantle-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The potential temperature of the mantle at the surface in Kelvin. If the value is lower then zero, the global value is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/1/thermal expansion coefficient
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_1_thermal-expansion-coefficient

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal expansion coefficient in $K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/1/specific heat
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_1_specific-heat

- **default value**:-1.0
- **type**:number
- **documentation**:The specific heat in $J kg^{-1} K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::::



:::::::::::::::

:::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/2
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_2

- **type**:object
- **documentation**:Linear temperature model. Can be set to use an adiabatic temperature at the boundaries.
- **additionalProperties**:false
- **required**:[model, max distance slab top]

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/2/model
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[linear]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/2/operation
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_2_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/2/min distance slab top
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_2_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/2/max distance slab top
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_2_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/2/top temperature
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_2_top-temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature at the top in degree Kelvin of this feature.If the value is below zero, the an adiabatic temperature is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/2/bottom temperature
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_2_bottom-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The temperature at the bottom in degree Kelvin of this feature. If the value is below zero, an adiabatic temperature is used.
::::::::::::::



:::::::::::::::

:::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3

- **type**:object
- **documentation**:Mass conserving temperature model. The temperature model uses the heat content (proportional to to thermal mass anomaly) to define a smooth temperature profile that conserves mass along the slab length. An empirical model, using error functions for smooth transitions, is used to  define how the minimum temperature increases with depth and how the location of the minimum temperature shifts into the slab interior. The slab is divided into top and bottom parts, which meet at the location where the minimum temperature occurs in the slab. For the bottom slab, the temperature is defined by a half-space cooling model. For the top of the slab the temperature is defined by one side of a 1D infinite space cooling model: this function was chosen to have a smoother temperature function across the minimum temperature position. The age of the overriding plate is used so the slab temperature at shallow depth smoothly transitions to the temperature of the overriding plate: this is not perfect, and is affected by the value of "top truncation" parameter subducting plate. Notes:1) the parameter "thickness" for the subducting plate segments needs to be defined but is not used. 2) because we use a negative truncation for distance above the slab, it is recommended to usedepth method:begin at end segment, in the main part of the world-builder file.Other methods may lead to gpas in temperatures at the segment boundaries.3)the empirical model used to define how Tmin increases with depth and how the position of Tmin shift with depth is expected to change somewhat after better calibrating with further tests.
- **additionalProperties**:false
- **required**:[model, plate velocity]

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3/model
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[mass conserving]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3/operation
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3/min distance slab top
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from the top surface of the slab over which the temperature is determined by this feature. This parameter should be negative and should be 1.5-2 times larger than the nominal slab thickness to allow the diffusion of cold temperatures from in the slab into the mantle above the slab surface. Also note that the top truncation value for the slab segment needs to have a value of -1, otherwise the temperature above the slab will be cut off at a distance less than the value set here.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3/max distance slab top
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance in meters from the top surface of the slab over which the temperature is determined by this feature. This parameter should be positive and approximately 2.5-3.0 times larger than the nominal slab thickness to allow the diffusion of coldtemperatures from in the slab into the mantle below the slab surface.For example if the slab starts with cold temperatures over a 100 km wide region, thisparameters should be about 250 km.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3/density
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3_density

- **default value**:3300.0
- **type**:number
- **documentation**:The reference density of the subducting plate in $kg/m^3$
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3/plate velocity
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3_plate-velocity

- **default value**:0.05
- **type**:number
- **documentation**:The velocity with which the plate subducts in meters per year. Default is 5 cm/yr
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3/coupling depth
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3_coupling-depth

- **default value**:100000.0
- **type**:number
- **documentation**:The depth at which the slab surface first comes in contact with the hot mantle wedge in meters. Default is 100 km.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3/forearc cooling factor
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3_forearc-cooling-factor

- **default value**:1.0
- **type**:number
- **documentation**:Increase the value to create thin (~2 km) cold thermal boundary layer above the slab.Any value greater than 1 does NOT meet the instantaneous conservation of mass, but does allow one to account for the history of insulating the forearc from heating up to this point in time. Note younger subducting lithosphere provides less insulation, while thicker, older slabs provide more insulation. Values up to 10 to 30 have been tested and don't cause any other extraneous effects. The larger th value the more you are not meeting the mass conserving criteria, so you don't want to see this affecting the temperature beyond the coupling depth as it will increase the mass of the slab and affect how it sinks.  If you use higher values, you will start to see that this creates a very thick cool layer above the entire slab - if you see this extending beyond the coupling zone reduce the value. You should use a value of 1 first and then only increase as little as possible to cool just the forearc region. Please examine the output temperature carefully. 
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3/thermal conductivity
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3_thermal-conductivity

- **default value**:3.3
- **type**:number
- **documentation**:The thermal conductivity of the subducting plate material in $W m^{-1} K^{-1}$.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3/thermal expansion coefficient
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3_thermal-expansion-coefficient

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal expansivity of the subducting plate material in $K^{-1}$. If smaller than zero, the global value is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3/specific heat
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3_specific-heat

- **default value**:-1.0
- **type**:number
- **documentation**:The specific heat of the subducting plate material in $J kg^{-1} K^{-1}$. If smaller than zero, the global value is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3/thermal diffusivity
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3_thermal-diffusivity

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal conductivity of the subducting plate material in $W m^{-1} K^{-1}$.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3/adiabatic heating
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3_adiabatic-heating

- **default value**:true
- **type**:boolean
- **documentation**:Whether adiabatic heating should be used for the slab.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3/taper distance
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3_taper-distance

- **default value**:100000.0
- **type**:number
- **documentation**:Distance over which to taper the slab tip.tapers the initial heat content to zero and the minimum temperature to the background temperature.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3/potential mantle temperature
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3_potential-mantle-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The potential temperature of the mantle at the surface in Kelvin. If smaller than zero, the global value is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3/ridge coordinates
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3_ridge-coordinates

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:An list of ridges. Each ridge is a lists of at least 2 2d points which define the location of the ridge. You need to define at least one ridge.So the an example with two ridges is [[[10,20],[20,30],[10,40]],[[50,10],[60,10]]].
:::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3/ridge coordinates/items
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3_ridge-coordinates_items

- **type**:array
- **minItems**:2
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3/ridge coordinates/items/items
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3_ridge-coordinates_items_items

- **type**:array
- **minItems**:2
- **maxItems**:2
- **documentation**:
:::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3/ridge coordinates/items/items/items
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3_ridge-coordinates_items_items_items

- **type**:number
:::::::::::

::::::::::::

:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/3/reference model name
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_3_reference-model-name

- **default value**:half space model
- **type**:string
- **documentation**:The type of thermal model to use in the mass conserving model of slab temperature. Options are half space model and plate model
::::::::::::::



:::::::::::::::

:::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/4
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_4

- **type**:object
- **documentation**:Plate model (based on McKenzie, 1970).
- **additionalProperties**:false
- **required**:[model, plate velocity]

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/4/model
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_4_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[plate model]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/4/operation
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_4_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/4/min distance slab top
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_4_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/4/max distance slab top
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_4_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/4/density
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_4_density

- **default value**:3300.0
- **type**:number
- **documentation**:The reference density of the subducting plate in $kg/m^3$
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/4/plate velocity
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_4_plate-velocity

- **default value**:NaN
- **type**:number
- **documentation**:The velocity in meters per year with which the plate subducts in meters per year.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/4/thermal conductivity
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_4_thermal-conductivity

- **default value**:2.0
- **type**:number
- **documentation**:The thermal conductivity of the subducting plate material in $W m^{-1} K^{-1}$.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/4/thermal expansion coefficient
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_4_thermal-expansion-coefficient

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal expansivity of the subducting plate material in $K^{-1}$. If smaller than zero, the global value is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/4/specific heat
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_4_specific-heat

- **default value**:-1.0
- **type**:number
- **documentation**:The specific heat of the subducting plate material in $J kg^{-1} K^{-1}$. If smaller than zero, the global value is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/4/adiabatic heating
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_4_adiabatic-heating

- **default value**:true
- **type**:boolean
- **documentation**:Whether adiabatic heating should be used for the slab. Setting the parameter to false leads to equation 26 from McKenzie (1970),which is the result obtained from McKenzie 1969.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/4/potential mantle temperature
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_4_potential-mantle-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The potential temperature of the mantle at the surface in Kelvin. If smaller than zero, the global value is used.
::::::::::::::



:::::::::::::::

:::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/5
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_5

- **type**:object
- **documentation**:Uniform temperature model. Set the temperature to a constan value.
- **additionalProperties**:false
- **required**:[model, temperature]

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/5/model
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_5_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[uniform]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/5/operation
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_5_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/5/min distance slab top
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_5_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/5/max distance slab top
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_5_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/temperature models/items/oneOf/5/temperature
:name: closed_features_items_oneOf_5_segments_items_temperature-models_items_oneOf_5_temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature in degree Kelvin which this feature should have
::::::::::::::



:::::::::::::::


:::::::::::::::::

::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models
:name: closed_features_items_oneOf_5_segments_items_composition-models

- **documentation**:
- **default value**:
- **type**:array
:::::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items
:name: closed_features_items_oneOf_5_segments_items_composition-models_items

::::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf

:::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf/1
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf_1

- **type**:object
- **documentation**:Compositional model object
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf/1/model
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the composition model.
- **enum**:[smooth]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf/1/min distance slab top
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf_1_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from which the composition of this layer is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf/1/max distance slab top
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf_1_max-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from which the composition of this layer is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf/1/top fractions
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf_1_top-fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:The composition fraction at the top of the slab (layer).
:::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf/1/top fractions/items
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf_1_top-fractions_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf/1/bottom fractions
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf_1_bottom-fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:The composition fraction at the bottom of the slab (layer).
:::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf/1/bottom fractions/items
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf_1_bottom-fractions_items

- **default value**:0.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the composition which are present there.
:::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf/1/operation
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value. Replacing implies that all compositions not explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.
- **enum**:[replace, replace defined only, add, subtract]
::::::::::::::



:::::::::::::::

:::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf/2
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf_2

- **type**:object
- **documentation**:Uniform compositional model. Sets constant compositional field.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf/2/model
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the composition model.
- **enum**:[uniform]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf/2/min distance slab top
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf_2_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf/2/max distance slab top
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf_2_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf/2/compositions
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf_2_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the composition which are present there.
:::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf/2/compositions/items
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf_2_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf/2/fractions
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf_2_fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:TA list of compositional fractions corresponding to the compositions list.
:::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf/2/fractions/items
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf_2_fractions_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/composition models/items/oneOf/2/operation
:name: closed_features_items_oneOf_5_segments_items_composition-models_items_oneOf_2_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value. Replacing implies that all compositions not explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.
- **enum**:[replace, replace defined only, add, subtract]
::::::::::::::



:::::::::::::::


:::::::::::::::::

::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models
:name: closed_features_items_oneOf_5_segments_items_grains-models

- **documentation**:
- **default value**:
- **type**:array
:::::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items
:name: closed_features_items_oneOf_5_segments_items_grains-models_items

::::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf

:::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/1
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_1

- **type**:object
- **documentation**:Random uniform distribution grains model. The size of the grains can be independently set to a single value or to a random distribution.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/1/model
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[random uniform distribution]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/1/min distance slab top
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_1_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:The distance from the slab top in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/1/max distance slab top
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_1_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance from the slab top in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/1/orientation operation
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_1_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/1/grain sizes
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_1_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be randomized between 0 and 1.
:::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/1/grain sizes/items
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_1_grain-sizes_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/1/normalize grain sizes
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_1_normalize-grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of whether the sizes of the grains should be normalized or not. If normalized, the total of the grains of a composition will be equal to 1.
:::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/1/normalize grain sizes/items
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_1_normalize-grain-sizes_items

- **default value**:true
- **type**:boolean
- **documentation**:
:::::::::::::

::::::::::::::



:::::::::::::::

:::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/2
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_2

- **type**:object
- **documentation**:Uniform grains model. All grains start exactly the same.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/2/model
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[uniform]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/2/min distance slab top
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_2_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:The distance from the slab top in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/2/max distance slab top
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_2_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance from the slab top in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/2/compositions
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_2_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/2/compositions/items
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_2_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/2/rotation matrices
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_2_rotation-matrices

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the grains which are present there for each compositions.
:::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/2/rotation matrices/items
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_2_rotation-matrices_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/2/rotation matrices/items/items
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_2_rotation-matrices_items_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
:::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/2/rotation matrices/items/items/items
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_2_rotation-matrices_items_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
:::::::::::

::::::::::::

:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/2/Euler angles z-x-z
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_2_Euler-angles-z-x-z

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the z-x-z Euler angles of the grains which are present there for each compositions.
:::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/2/Euler angles z-x-z/items
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_2_Euler-angles-z-x-z_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/2/Euler angles z-x-z/items/items
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_2_Euler-angles-z-x-z_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::

:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/2/orientation operation
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_2_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/2/grain sizes
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_2_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be set so that the total is equal to 1.
:::::::::::::{dropdown} /features/items/oneOf/5/segments/items/grains models/items/oneOf/2/grain sizes/items
:name: closed_features_items_oneOf_5_segments_items_grains-models_items_oneOf_2_grain-sizes_items

- **default value**:-1.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::



:::::::::::::::


:::::::::::::::::

::::::::::::::::::



:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models
:name: closed_features_items_oneOf_5_temperature-models

- **documentation**:A list of temperature models.
- **default value**:
- **type**:array
:::::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items
:name: closed_features_items_oneOf_5_temperature-models_items

::::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf

:::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/1
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_1

- **type**:object
- **documentation**:Adiabatic temperature model. Uses global values by default.
- **additionalProperties**:false
- **required**:[model]

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/1/model
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[adiabatic]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/1/operation
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/1/min distance slab top
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_1_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/1/max distance slab top
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_1_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/1/potential mantle temperature
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_1_potential-mantle-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The potential temperature of the mantle at the surface in Kelvin. If the value is lower then zero, the global value is used.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/1/thermal expansion coefficient
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_1_thermal-expansion-coefficient

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal expansion coefficient in $K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/1/specific heat
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_1_specific-heat

- **default value**:-1.0
- **type**:number
- **documentation**:The specific heat in $J kg^{-1} K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/2
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_2

- **type**:object
- **documentation**:Linear temperature model. Can be set to use an adiabatic temperature at the boundaries.
- **additionalProperties**:false
- **required**:[model, max distance slab top]

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/2/model
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[linear]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/2/operation
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_2_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/2/min distance slab top
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_2_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/2/max distance slab top
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_2_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/2/top temperature
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_2_top-temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature at the top in degree Kelvin of this feature.If the value is below zero, the an adiabatic temperature is used.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/2/bottom temperature
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_2_bottom-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The temperature at the bottom in degree Kelvin of this feature. If the value is below zero, an adiabatic temperature is used.
::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3

- **type**:object
- **documentation**:Mass conserving temperature model. The temperature model uses the heat content (proportional to to thermal mass anomaly) to define a smooth temperature profile that conserves mass along the slab length. An empirical model, using error functions for smooth transitions, is used to  define how the minimum temperature increases with depth and how the location of the minimum temperature shifts into the slab interior. The slab is divided into top and bottom parts, which meet at the location where the minimum temperature occurs in the slab. For the bottom slab, the temperature is defined by a half-space cooling model. For the top of the slab the temperature is defined by one side of a 1D infinite space cooling model: this function was chosen to have a smoother temperature function across the minimum temperature position. The age of the overriding plate is used so the slab temperature at shallow depth smoothly transitions to the temperature of the overriding plate: this is not perfect, and is affected by the value of "top truncation" parameter subducting plate. Notes:1) the parameter "thickness" for the subducting plate segments needs to be defined but is not used. 2) because we use a negative truncation for distance above the slab, it is recommended to usedepth method:begin at end segment, in the main part of the world-builder file.Other methods may lead to gpas in temperatures at the segment boundaries.3)the empirical model used to define how Tmin increases with depth and how the position of Tmin shift with depth is expected to change somewhat after better calibrating with further tests.
- **additionalProperties**:false
- **required**:[model, plate velocity]

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3/model
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[mass conserving]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3/operation
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3/min distance slab top
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from the top surface of the slab over which the temperature is determined by this feature. This parameter should be negative and should be 1.5-2 times larger than the nominal slab thickness to allow the diffusion of cold temperatures from in the slab into the mantle above the slab surface. Also note that the top truncation value for the slab segment needs to have a value of -1, otherwise the temperature above the slab will be cut off at a distance less than the value set here.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3/max distance slab top
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance in meters from the top surface of the slab over which the temperature is determined by this feature. This parameter should be positive and approximately 2.5-3.0 times larger than the nominal slab thickness to allow the diffusion of coldtemperatures from in the slab into the mantle below the slab surface.For example if the slab starts with cold temperatures over a 100 km wide region, thisparameters should be about 250 km.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3/density
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3_density

- **default value**:3300.0
- **type**:number
- **documentation**:The reference density of the subducting plate in $kg/m^3$
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3/plate velocity
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3_plate-velocity

- **default value**:0.05
- **type**:number
- **documentation**:The velocity with which the plate subducts in meters per year. Default is 5 cm/yr
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3/coupling depth
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3_coupling-depth

- **default value**:100000.0
- **type**:number
- **documentation**:The depth at which the slab surface first comes in contact with the hot mantle wedge in meters. Default is 100 km.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3/forearc cooling factor
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3_forearc-cooling-factor

- **default value**:1.0
- **type**:number
- **documentation**:Increase the value to create thin (~2 km) cold thermal boundary layer above the slab.Any value greater than 1 does NOT meet the instantaneous conservation of mass, but does allow one to account for the history of insulating the forearc from heating up to this point in time. Note younger subducting lithosphere provides less insulation, while thicker, older slabs provide more insulation. Values up to 10 to 30 have been tested and don't cause any other extraneous effects. The larger th value the more you are not meeting the mass conserving criteria, so you don't want to see this affecting the temperature beyond the coupling depth as it will increase the mass of the slab and affect how it sinks.  If you use higher values, you will start to see that this creates a very thick cool layer above the entire slab - if you see this extending beyond the coupling zone reduce the value. You should use a value of 1 first and then only increase as little as possible to cool just the forearc region. Please examine the output temperature carefully. 
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3/thermal conductivity
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3_thermal-conductivity

- **default value**:3.3
- **type**:number
- **documentation**:The thermal conductivity of the subducting plate material in $W m^{-1} K^{-1}$.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3/thermal expansion coefficient
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3_thermal-expansion-coefficient

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal expansivity of the subducting plate material in $K^{-1}$. If smaller than zero, the global value is used.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3/specific heat
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3_specific-heat

- **default value**:-1.0
- **type**:number
- **documentation**:The specific heat of the subducting plate material in $J kg^{-1} K^{-1}$. If smaller than zero, the global value is used.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3/thermal diffusivity
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3_thermal-diffusivity

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal conductivity of the subducting plate material in $W m^{-1} K^{-1}$.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3/adiabatic heating
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3_adiabatic-heating

- **default value**:true
- **type**:boolean
- **documentation**:Whether adiabatic heating should be used for the slab.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3/taper distance
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3_taper-distance

- **default value**:100000.0
- **type**:number
- **documentation**:Distance over which to taper the slab tip.tapers the initial heat content to zero and the minimum temperature to the background temperature.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3/potential mantle temperature
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3_potential-mantle-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The potential temperature of the mantle at the surface in Kelvin. If smaller than zero, the global value is used.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3/ridge coordinates
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3_ridge-coordinates

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:An list of ridges. Each ridge is a lists of at least 2 2d points which define the location of the ridge. You need to define at least one ridge.So the an example with two ridges is [[[10,20],[20,30],[10,40]],[[50,10],[60,10]]].
:::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3/ridge coordinates/items
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3_ridge-coordinates_items

- **type**:array
- **minItems**:2
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3/ridge coordinates/items/items
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3_ridge-coordinates_items_items

- **type**:array
- **minItems**:2
- **maxItems**:2
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3/ridge coordinates/items/items/items
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3_ridge-coordinates_items_items_items

- **type**:number
:::::::::::::

::::::::::::::

:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/3/reference model name
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_3_reference-model-name

- **default value**:half space model
- **type**:string
- **documentation**:The type of thermal model to use in the mass conserving model of slab temperature. Options are half space model and plate model
::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/4
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_4

- **type**:object
- **documentation**:Plate model (based on McKenzie, 1970).
- **additionalProperties**:false
- **required**:[model, plate velocity]

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/4/model
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_4_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[plate model]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/4/operation
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_4_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/4/min distance slab top
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_4_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/4/max distance slab top
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_4_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/4/density
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_4_density

- **default value**:3300.0
- **type**:number
- **documentation**:The reference density of the subducting plate in $kg/m^3$
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/4/plate velocity
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_4_plate-velocity

- **default value**:NaN
- **type**:number
- **documentation**:The velocity in meters per year with which the plate subducts in meters per year.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/4/thermal conductivity
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_4_thermal-conductivity

- **default value**:2.0
- **type**:number
- **documentation**:The thermal conductivity of the subducting plate material in $W m^{-1} K^{-1}$.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/4/thermal expansion coefficient
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_4_thermal-expansion-coefficient

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal expansivity of the subducting plate material in $K^{-1}$. If smaller than zero, the global value is used.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/4/specific heat
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_4_specific-heat

- **default value**:-1.0
- **type**:number
- **documentation**:The specific heat of the subducting plate material in $J kg^{-1} K^{-1}$. If smaller than zero, the global value is used.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/4/adiabatic heating
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_4_adiabatic-heating

- **default value**:true
- **type**:boolean
- **documentation**:Whether adiabatic heating should be used for the slab. Setting the parameter to false leads to equation 26 from McKenzie (1970),which is the result obtained from McKenzie 1969.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/4/potential mantle temperature
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_4_potential-mantle-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The potential temperature of the mantle at the surface in Kelvin. If smaller than zero, the global value is used.
::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/5
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_5

- **type**:object
- **documentation**:Uniform temperature model. Set the temperature to a constan value.
- **additionalProperties**:false
- **required**:[model, temperature]

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/5/model
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_5_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[uniform]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/5/operation
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_5_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/5/min distance slab top
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_5_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/5/max distance slab top
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_5_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/temperature models/items/oneOf/5/temperature
:name: closed_features_items_oneOf_5_temperature-models_items_oneOf_5_temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature in degree Kelvin which this feature should have
::::::::::::::::



:::::::::::::::::


:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/5/composition models
:name: closed_features_items_oneOf_5_composition-models

- **documentation**:A list of composition models.
- **default value**:
- **type**:array
:::::::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items
:name: closed_features_items_oneOf_5_composition-models_items

::::::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf
:name: closed_features_items_oneOf_5_composition-models_items_oneOf

:::::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf/1
:name: closed_features_items_oneOf_5_composition-models_items_oneOf_1

- **type**:object
- **documentation**:Compositional model object
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf/1/model
:name: closed_features_items_oneOf_5_composition-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the composition model.
- **enum**:[smooth]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf/1/min distance slab top
:name: closed_features_items_oneOf_5_composition-models_items_oneOf_1_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from which the composition of this layer is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf/1/max distance slab top
:name: closed_features_items_oneOf_5_composition-models_items_oneOf_1_max-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from which the composition of this layer is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf/1/top fractions
:name: closed_features_items_oneOf_5_composition-models_items_oneOf_1_top-fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:The composition fraction at the top of the slab (layer).
:::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf/1/top fractions/items
:name: closed_features_items_oneOf_5_composition-models_items_oneOf_1_top-fractions_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf/1/bottom fractions
:name: closed_features_items_oneOf_5_composition-models_items_oneOf_1_bottom-fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:The composition fraction at the bottom of the slab (layer).
:::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf/1/bottom fractions/items
:name: closed_features_items_oneOf_5_composition-models_items_oneOf_1_bottom-fractions_items

- **default value**:0.0
- **type**:number
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_5_composition-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the composition which are present there.
:::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_5_composition-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf/1/operation
:name: closed_features_items_oneOf_5_composition-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value. Replacing implies that all compositions not explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.
- **enum**:[replace, replace defined only, add, subtract]
::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf/2
:name: closed_features_items_oneOf_5_composition-models_items_oneOf_2

- **type**:object
- **documentation**:Uniform compositional model. Sets constant compositional field.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf/2/model
:name: closed_features_items_oneOf_5_composition-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the composition model.
- **enum**:[uniform]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf/2/min distance slab top
:name: closed_features_items_oneOf_5_composition-models_items_oneOf_2_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf/2/max distance slab top
:name: closed_features_items_oneOf_5_composition-models_items_oneOf_2_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf/2/compositions
:name: closed_features_items_oneOf_5_composition-models_items_oneOf_2_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the composition which are present there.
:::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf/2/compositions/items
:name: closed_features_items_oneOf_5_composition-models_items_oneOf_2_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf/2/fractions
:name: closed_features_items_oneOf_5_composition-models_items_oneOf_2_fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:TA list of compositional fractions corresponding to the compositions list.
:::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf/2/fractions/items
:name: closed_features_items_oneOf_5_composition-models_items_oneOf_2_fractions_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/composition models/items/oneOf/2/operation
:name: closed_features_items_oneOf_5_composition-models_items_oneOf_2_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value. Replacing implies that all compositions not explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.
- **enum**:[replace, replace defined only, add, subtract]
::::::::::::::::



:::::::::::::::::


:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/5/grains models
:name: closed_features_items_oneOf_5_grains-models

- **documentation**:A list of grains models.
- **default value**:
- **type**:array
:::::::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items
:name: closed_features_items_oneOf_5_grains-models_items

::::::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf
:name: closed_features_items_oneOf_5_grains-models_items_oneOf

:::::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/1
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_1

- **type**:object
- **documentation**:Random uniform distribution grains model. The size of the grains can be independently set to a single value or to a random distribution.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/1/model
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[random uniform distribution]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/1/min distance slab top
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_1_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:The distance from the slab top in meters from which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/1/max distance slab top
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_1_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance from the slab top in meters to which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/1/orientation operation
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_1_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/1/grain sizes
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_1_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be randomized between 0 and 1.
:::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/1/grain sizes/items
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_1_grain-sizes_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/1/normalize grain sizes
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_1_normalize-grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of whether the sizes of the grains should be normalized or not. If normalized, the total of the grains of a composition will be equal to 1.
:::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/1/normalize grain sizes/items
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_1_normalize-grain-sizes_items

- **default value**:true
- **type**:boolean
- **documentation**:
:::::::::::::::

::::::::::::::::



:::::::::::::::::

:::::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/2
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_2

- **type**:object
- **documentation**:Uniform grains model. All grains start exactly the same.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/2/model
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[uniform]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/2/min distance slab top
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_2_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:The distance from the slab top in meters from which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/2/max distance slab top
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_2_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance from the slab top in meters to which the composition of this feature is present.
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/2/compositions
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_2_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/2/compositions/items
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_2_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/2/rotation matrices
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_2_rotation-matrices

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the grains which are present there for each compositions.
:::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/2/rotation matrices/items
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_2_rotation-matrices_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/2/rotation matrices/items/items
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_2_rotation-matrices_items_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
:::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/2/rotation matrices/items/items/items
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_2_rotation-matrices_items_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/2/Euler angles z-x-z
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_2_Euler-angles-z-x-z

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the z-x-z Euler angles of the grains which are present there for each compositions.
:::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/2/Euler angles z-x-z/items
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_2_Euler-angles-z-x-z_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/2/Euler angles z-x-z/items/items
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_2_Euler-angles-z-x-z_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::::

:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/2/orientation operation
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_2_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/2/grain sizes
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_2_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be set so that the total is equal to 1.
:::::::::::::::{dropdown} /features/items/oneOf/5/grains models/items/oneOf/2/grain sizes/items
:name: closed_features_items_oneOf_5_grains-models_items_oneOf_2_grain-sizes_items

- **default value**:-1.0
- **type**:number
- **documentation**:
:::::::::::::::

::::::::::::::::



:::::::::::::::::


:::::::::::::::::::

::::::::::::::::::::

::::::::::::::::::::{dropdown} /features/items/oneOf/5/sections
:name: closed_features_items_oneOf_5_sections

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of feature properties for a coordinate.
:::::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items
:name: closed_features_items_oneOf_5_sections_items

- **documentation**:
- **default value**:
- **type**:object

::::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/min depth
:name: closed_features_items_oneOf_5_sections_items_min-depth

- **default value**:0.0
- **type**:number
- **documentation**:The depth to which this feature is present
::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/max depth
:name: closed_features_items_oneOf_5_sections_items_max-depth

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The depth to which this feature is present
::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/dip point
:name: closed_features_items_oneOf_5_sections_items_dip-point

- **type**:array
- **minItems**:2
- **maxItems**:2
- **documentation**:The depth to which this feature is present
:::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/dip point/items
:name: closed_features_items_oneOf_5_sections_items_dip-point_items

- **type**:number
:::::::::::::::::

::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments
:name: closed_features_items_oneOf_5_sections_items_segments

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:The depth to which this feature is present
:::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items
:name: closed_features_items_oneOf_5_sections_items_segments_items

- **type**:object
- **additionalProperties**:false
- **documentation**:
- **required**:[length, thickness, angle]

::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/length
:name: closed_features_items_oneOf_5_sections_items_segments_items_length

- **type**:number
::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/thickness
:name: closed_features_items_oneOf_5_sections_items_segments_items_thickness

- **type**:array
- **minItems**:1
- **maxItems**:2
:::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/thickness/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_thickness_items

- **type**:number
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/top truncation
:name: closed_features_items_oneOf_5_sections_items_segments_items_top-truncation

- **type**:array
- **minItems**:1
- **maxItems**:2
:::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/top truncation/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_top-truncation_items

- **type**:number
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/angle
:name: closed_features_items_oneOf_5_sections_items_segments_items_angle

- **type**:array
- **minItems**:1
- **maxItems**:2
:::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/angle/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_angle_items

- **type**:number
:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models

- **documentation**:
- **default value**:
- **type**:array
:::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf

:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/1
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_1

- **type**:object
- **documentation**:Adiabatic temperature model. Uses global values by default.
- **additionalProperties**:false
- **required**:[model]

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/1/model
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[adiabatic]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/1/operation
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/1/min distance slab top
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_1_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/1/max distance slab top
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_1_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/1/potential mantle temperature
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_1_potential-mantle-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The potential temperature of the mantle at the surface in Kelvin. If the value is lower then zero, the global value is used.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/1/thermal expansion coefficient
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_1_thermal-expansion-coefficient

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal expansion coefficient in $K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/1/specific heat
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_1_specific-heat

- **default value**:-1.0
- **type**:number
- **documentation**:The specific heat in $J kg^{-1} K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::



:::::::::::::

:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/2
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_2

- **type**:object
- **documentation**:Linear temperature model. Can be set to use an adiabatic temperature at the boundaries.
- **additionalProperties**:false
- **required**:[model, max distance slab top]

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/2/model
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[linear]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/2/operation
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_2_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/2/min distance slab top
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_2_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/2/max distance slab top
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_2_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/2/top temperature
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_2_top-temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature at the top in degree Kelvin of this feature.If the value is below zero, the an adiabatic temperature is used.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/2/bottom temperature
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_2_bottom-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The temperature at the bottom in degree Kelvin of this feature. If the value is below zero, an adiabatic temperature is used.
::::::::::::



:::::::::::::

:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3

- **type**:object
- **documentation**:Mass conserving temperature model. The temperature model uses the heat content (proportional to to thermal mass anomaly) to define a smooth temperature profile that conserves mass along the slab length. An empirical model, using error functions for smooth transitions, is used to  define how the minimum temperature increases with depth and how the location of the minimum temperature shifts into the slab interior. The slab is divided into top and bottom parts, which meet at the location where the minimum temperature occurs in the slab. For the bottom slab, the temperature is defined by a half-space cooling model. For the top of the slab the temperature is defined by one side of a 1D infinite space cooling model: this function was chosen to have a smoother temperature function across the minimum temperature position. The age of the overriding plate is used so the slab temperature at shallow depth smoothly transitions to the temperature of the overriding plate: this is not perfect, and is affected by the value of "top truncation" parameter subducting plate. Notes:1) the parameter "thickness" for the subducting plate segments needs to be defined but is not used. 2) because we use a negative truncation for distance above the slab, it is recommended to usedepth method:begin at end segment, in the main part of the world-builder file.Other methods may lead to gpas in temperatures at the segment boundaries.3)the empirical model used to define how Tmin increases with depth and how the position of Tmin shift with depth is expected to change somewhat after better calibrating with further tests.
- **additionalProperties**:false
- **required**:[model, plate velocity]

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3/model
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[mass conserving]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3/operation
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3/min distance slab top
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from the top surface of the slab over which the temperature is determined by this feature. This parameter should be negative and should be 1.5-2 times larger than the nominal slab thickness to allow the diffusion of cold temperatures from in the slab into the mantle above the slab surface. Also note that the top truncation value for the slab segment needs to have a value of -1, otherwise the temperature above the slab will be cut off at a distance less than the value set here.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3/max distance slab top
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance in meters from the top surface of the slab over which the temperature is determined by this feature. This parameter should be positive and approximately 2.5-3.0 times larger than the nominal slab thickness to allow the diffusion of coldtemperatures from in the slab into the mantle below the slab surface.For example if the slab starts with cold temperatures over a 100 km wide region, thisparameters should be about 250 km.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3/density
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3_density

- **default value**:3300.0
- **type**:number
- **documentation**:The reference density of the subducting plate in $kg/m^3$
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3/plate velocity
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3_plate-velocity

- **default value**:0.05
- **type**:number
- **documentation**:The velocity with which the plate subducts in meters per year. Default is 5 cm/yr
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3/coupling depth
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3_coupling-depth

- **default value**:100000.0
- **type**:number
- **documentation**:The depth at which the slab surface first comes in contact with the hot mantle wedge in meters. Default is 100 km.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3/forearc cooling factor
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3_forearc-cooling-factor

- **default value**:1.0
- **type**:number
- **documentation**:Increase the value to create thin (~2 km) cold thermal boundary layer above the slab.Any value greater than 1 does NOT meet the instantaneous conservation of mass, but does allow one to account for the history of insulating the forearc from heating up to this point in time. Note younger subducting lithosphere provides less insulation, while thicker, older slabs provide more insulation. Values up to 10 to 30 have been tested and don't cause any other extraneous effects. The larger th value the more you are not meeting the mass conserving criteria, so you don't want to see this affecting the temperature beyond the coupling depth as it will increase the mass of the slab and affect how it sinks.  If you use higher values, you will start to see that this creates a very thick cool layer above the entire slab - if you see this extending beyond the coupling zone reduce the value. You should use a value of 1 first and then only increase as little as possible to cool just the forearc region. Please examine the output temperature carefully. 
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3/thermal conductivity
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3_thermal-conductivity

- **default value**:3.3
- **type**:number
- **documentation**:The thermal conductivity of the subducting plate material in $W m^{-1} K^{-1}$.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3/thermal expansion coefficient
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3_thermal-expansion-coefficient

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal expansivity of the subducting plate material in $K^{-1}$. If smaller than zero, the global value is used.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3/specific heat
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3_specific-heat

- **default value**:-1.0
- **type**:number
- **documentation**:The specific heat of the subducting plate material in $J kg^{-1} K^{-1}$. If smaller than zero, the global value is used.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3/thermal diffusivity
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3_thermal-diffusivity

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal conductivity of the subducting plate material in $W m^{-1} K^{-1}$.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3/adiabatic heating
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3_adiabatic-heating

- **default value**:true
- **type**:boolean
- **documentation**:Whether adiabatic heating should be used for the slab.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3/taper distance
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3_taper-distance

- **default value**:100000.0
- **type**:number
- **documentation**:Distance over which to taper the slab tip.tapers the initial heat content to zero and the minimum temperature to the background temperature.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3/potential mantle temperature
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3_potential-mantle-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The potential temperature of the mantle at the surface in Kelvin. If smaller than zero, the global value is used.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3/ridge coordinates
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3_ridge-coordinates

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:An list of ridges. Each ridge is a lists of at least 2 2d points which define the location of the ridge. You need to define at least one ridge.So the an example with two ridges is [[[10,20],[20,30],[10,40]],[[50,10],[60,10]]].
:::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3/ridge coordinates/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3_ridge-coordinates_items

- **type**:array
- **minItems**:2
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3/ridge coordinates/items/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3_ridge-coordinates_items_items

- **type**:array
- **minItems**:2
- **maxItems**:2
- **documentation**:
:::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3/ridge coordinates/items/items/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3_ridge-coordinates_items_items_items

- **type**:number
:::::::::

::::::::::

:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/3/reference model name
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_3_reference-model-name

- **default value**:half space model
- **type**:string
- **documentation**:The type of thermal model to use in the mass conserving model of slab temperature. Options are half space model and plate model
::::::::::::



:::::::::::::

:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/4
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_4

- **type**:object
- **documentation**:Plate model (based on McKenzie, 1970).
- **additionalProperties**:false
- **required**:[model, plate velocity]

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/4/model
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_4_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[plate model]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/4/operation
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_4_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/4/min distance slab top
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_4_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/4/max distance slab top
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_4_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/4/density
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_4_density

- **default value**:3300.0
- **type**:number
- **documentation**:The reference density of the subducting plate in $kg/m^3$
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/4/plate velocity
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_4_plate-velocity

- **default value**:NaN
- **type**:number
- **documentation**:The velocity in meters per year with which the plate subducts in meters per year.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/4/thermal conductivity
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_4_thermal-conductivity

- **default value**:2.0
- **type**:number
- **documentation**:The thermal conductivity of the subducting plate material in $W m^{-1} K^{-1}$.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/4/thermal expansion coefficient
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_4_thermal-expansion-coefficient

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal expansivity of the subducting plate material in $K^{-1}$. If smaller than zero, the global value is used.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/4/specific heat
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_4_specific-heat

- **default value**:-1.0
- **type**:number
- **documentation**:The specific heat of the subducting plate material in $J kg^{-1} K^{-1}$. If smaller than zero, the global value is used.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/4/adiabatic heating
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_4_adiabatic-heating

- **default value**:true
- **type**:boolean
- **documentation**:Whether adiabatic heating should be used for the slab. Setting the parameter to false leads to equation 26 from McKenzie (1970),which is the result obtained from McKenzie 1969.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/4/potential mantle temperature
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_4_potential-mantle-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The potential temperature of the mantle at the surface in Kelvin. If smaller than zero, the global value is used.
::::::::::::



:::::::::::::

:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/5
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_5

- **type**:object
- **documentation**:Uniform temperature model. Set the temperature to a constan value.
- **additionalProperties**:false
- **required**:[model, temperature]

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/5/model
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_5_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[uniform]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/5/operation
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_5_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/5/min distance slab top
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_5_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/5/max distance slab top
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_5_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/temperature models/items/oneOf/5/temperature
:name: closed_features_items_oneOf_5_sections_items_segments_items_temperature-models_items_oneOf_5_temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature in degree Kelvin which this feature should have
::::::::::::



:::::::::::::


:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models

- **documentation**:
- **default value**:
- **type**:array
:::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf

:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf/1
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf_1

- **type**:object
- **documentation**:Compositional model object
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf/1/model
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the composition model.
- **enum**:[smooth]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf/1/min distance slab top
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf_1_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from which the composition of this layer is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf/1/max distance slab top
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf_1_max-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from which the composition of this layer is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf/1/top fractions
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf_1_top-fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:The composition fraction at the top of the slab (layer).
:::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf/1/top fractions/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf_1_top-fractions_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf/1/bottom fractions
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf_1_bottom-fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:The composition fraction at the bottom of the slab (layer).
:::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf/1/bottom fractions/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf_1_bottom-fractions_items

- **default value**:0.0
- **type**:number
- **documentation**:
:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the composition which are present there.
:::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf/1/operation
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value. Replacing implies that all compositions not explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.
- **enum**:[replace, replace defined only, add, subtract]
::::::::::::



:::::::::::::

:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf/2
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf_2

- **type**:object
- **documentation**:Uniform compositional model. Sets constant compositional field.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf/2/model
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the composition model.
- **enum**:[uniform]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf/2/min distance slab top
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf_2_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf/2/max distance slab top
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf_2_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf/2/compositions
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf_2_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the composition which are present there.
:::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf/2/compositions/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf_2_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf/2/fractions
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf_2_fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:TA list of compositional fractions corresponding to the compositions list.
:::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf/2/fractions/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf_2_fractions_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/composition models/items/oneOf/2/operation
:name: closed_features_items_oneOf_5_sections_items_segments_items_composition-models_items_oneOf_2_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value. Replacing implies that all compositions not explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.
- **enum**:[replace, replace defined only, add, subtract]
::::::::::::



:::::::::::::


:::::::::::::::

::::::::::::::::

::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models

- **documentation**:
- **default value**:
- **type**:array
:::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf

:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/1
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_1

- **type**:object
- **documentation**:Random uniform distribution grains model. The size of the grains can be independently set to a single value or to a random distribution.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/1/model
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[random uniform distribution]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/1/min distance slab top
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_1_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:The distance from the slab top in meters from which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/1/max distance slab top
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_1_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance from the slab top in meters to which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/1/orientation operation
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_1_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/1/grain sizes
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_1_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be randomized between 0 and 1.
:::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/1/grain sizes/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_1_grain-sizes_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/1/normalize grain sizes
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_1_normalize-grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of whether the sizes of the grains should be normalized or not. If normalized, the total of the grains of a composition will be equal to 1.
:::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/1/normalize grain sizes/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_1_normalize-grain-sizes_items

- **default value**:true
- **type**:boolean
- **documentation**:
:::::::::::

::::::::::::



:::::::::::::

:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/2
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_2

- **type**:object
- **documentation**:Uniform grains model. All grains start exactly the same.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/2/model
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[uniform]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/2/min distance slab top
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_2_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:The distance from the slab top in meters from which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/2/max distance slab top
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_2_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance from the slab top in meters to which the composition of this feature is present.
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/2/compositions
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_2_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/2/compositions/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_2_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/2/rotation matrices
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_2_rotation-matrices

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the grains which are present there for each compositions.
:::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/2/rotation matrices/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_2_rotation-matrices_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/2/rotation matrices/items/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_2_rotation-matrices_items_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
:::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/2/rotation matrices/items/items/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_2_rotation-matrices_items_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
:::::::::

::::::::::

:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/2/Euler angles z-x-z
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_2_Euler-angles-z-x-z

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the z-x-z Euler angles of the grains which are present there for each compositions.
:::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/2/Euler angles z-x-z/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_2_Euler-angles-z-x-z_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/2/Euler angles z-x-z/items/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_2_Euler-angles-z-x-z_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::

:::::::::::

::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/2/orientation operation
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_2_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::

::::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/2/grain sizes
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_2_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be set so that the total is equal to 1.
:::::::::::{dropdown} /features/items/oneOf/5/sections/items/segments/items/grains models/items/oneOf/2/grain sizes/items
:name: closed_features_items_oneOf_5_sections_items_segments_items_grains-models_items_oneOf_2_grain-sizes_items

- **default value**:-1.0
- **type**:number
- **documentation**:
:::::::::::

::::::::::::



:::::::::::::


:::::::::::::::

::::::::::::::::



:::::::::::::::::

::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models
:name: closed_features_items_oneOf_5_sections_items_temperature-models

- **documentation**:A list of temperature models.
- **default value**:
- **type**:array
:::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items

::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf

:::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/1
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_1

- **type**:object
- **documentation**:Adiabatic temperature model. Uses global values by default.
- **additionalProperties**:false
- **required**:[model]

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/1/model
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[adiabatic]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/1/operation
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/1/min distance slab top
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_1_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/1/max distance slab top
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_1_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/1/potential mantle temperature
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_1_potential-mantle-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The potential temperature of the mantle at the surface in Kelvin. If the value is lower then zero, the global value is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/1/thermal expansion coefficient
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_1_thermal-expansion-coefficient

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal expansion coefficient in $K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/1/specific heat
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_1_specific-heat

- **default value**:-1.0
- **type**:number
- **documentation**:The specific heat in $J kg^{-1} K^{-1}$. If the value is lower then zero, the global value is used.
::::::::::::::



:::::::::::::::

:::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/2
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_2

- **type**:object
- **documentation**:Linear temperature model. Can be set to use an adiabatic temperature at the boundaries.
- **additionalProperties**:false
- **required**:[model, max distance slab top]

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/2/model
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[linear]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/2/operation
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_2_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/2/min distance slab top
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_2_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/2/max distance slab top
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_2_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/2/top temperature
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_2_top-temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature at the top in degree Kelvin of this feature.If the value is below zero, the an adiabatic temperature is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/2/bottom temperature
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_2_bottom-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The temperature at the bottom in degree Kelvin of this feature. If the value is below zero, an adiabatic temperature is used.
::::::::::::::



:::::::::::::::

:::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3

- **type**:object
- **documentation**:Mass conserving temperature model. The temperature model uses the heat content (proportional to to thermal mass anomaly) to define a smooth temperature profile that conserves mass along the slab length. An empirical model, using error functions for smooth transitions, is used to  define how the minimum temperature increases with depth and how the location of the minimum temperature shifts into the slab interior. The slab is divided into top and bottom parts, which meet at the location where the minimum temperature occurs in the slab. For the bottom slab, the temperature is defined by a half-space cooling model. For the top of the slab the temperature is defined by one side of a 1D infinite space cooling model: this function was chosen to have a smoother temperature function across the minimum temperature position. The age of the overriding plate is used so the slab temperature at shallow depth smoothly transitions to the temperature of the overriding plate: this is not perfect, and is affected by the value of "top truncation" parameter subducting plate. Notes:1) the parameter "thickness" for the subducting plate segments needs to be defined but is not used. 2) because we use a negative truncation for distance above the slab, it is recommended to usedepth method:begin at end segment, in the main part of the world-builder file.Other methods may lead to gpas in temperatures at the segment boundaries.3)the empirical model used to define how Tmin increases with depth and how the position of Tmin shift with depth is expected to change somewhat after better calibrating with further tests.
- **additionalProperties**:false
- **required**:[model, plate velocity]

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3/model
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[mass conserving]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3/operation
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3/min distance slab top
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from the top surface of the slab over which the temperature is determined by this feature. This parameter should be negative and should be 1.5-2 times larger than the nominal slab thickness to allow the diffusion of cold temperatures from in the slab into the mantle above the slab surface. Also note that the top truncation value for the slab segment needs to have a value of -1, otherwise the temperature above the slab will be cut off at a distance less than the value set here.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3/max distance slab top
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance in meters from the top surface of the slab over which the temperature is determined by this feature. This parameter should be positive and approximately 2.5-3.0 times larger than the nominal slab thickness to allow the diffusion of coldtemperatures from in the slab into the mantle below the slab surface.For example if the slab starts with cold temperatures over a 100 km wide region, thisparameters should be about 250 km.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3/density
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3_density

- **default value**:3300.0
- **type**:number
- **documentation**:The reference density of the subducting plate in $kg/m^3$
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3/plate velocity
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3_plate-velocity

- **default value**:0.05
- **type**:number
- **documentation**:The velocity with which the plate subducts in meters per year. Default is 5 cm/yr
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3/coupling depth
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3_coupling-depth

- **default value**:100000.0
- **type**:number
- **documentation**:The depth at which the slab surface first comes in contact with the hot mantle wedge in meters. Default is 100 km.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3/forearc cooling factor
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3_forearc-cooling-factor

- **default value**:1.0
- **type**:number
- **documentation**:Increase the value to create thin (~2 km) cold thermal boundary layer above the slab.Any value greater than 1 does NOT meet the instantaneous conservation of mass, but does allow one to account for the history of insulating the forearc from heating up to this point in time. Note younger subducting lithosphere provides less insulation, while thicker, older slabs provide more insulation. Values up to 10 to 30 have been tested and don't cause any other extraneous effects. The larger th value the more you are not meeting the mass conserving criteria, so you don't want to see this affecting the temperature beyond the coupling depth as it will increase the mass of the slab and affect how it sinks.  If you use higher values, you will start to see that this creates a very thick cool layer above the entire slab - if you see this extending beyond the coupling zone reduce the value. You should use a value of 1 first and then only increase as little as possible to cool just the forearc region. Please examine the output temperature carefully. 
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3/thermal conductivity
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3_thermal-conductivity

- **default value**:3.3
- **type**:number
- **documentation**:The thermal conductivity of the subducting plate material in $W m^{-1} K^{-1}$.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3/thermal expansion coefficient
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3_thermal-expansion-coefficient

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal expansivity of the subducting plate material in $K^{-1}$. If smaller than zero, the global value is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3/specific heat
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3_specific-heat

- **default value**:-1.0
- **type**:number
- **documentation**:The specific heat of the subducting plate material in $J kg^{-1} K^{-1}$. If smaller than zero, the global value is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3/thermal diffusivity
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3_thermal-diffusivity

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal conductivity of the subducting plate material in $W m^{-1} K^{-1}$.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3/adiabatic heating
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3_adiabatic-heating

- **default value**:true
- **type**:boolean
- **documentation**:Whether adiabatic heating should be used for the slab.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3/taper distance
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3_taper-distance

- **default value**:100000.0
- **type**:number
- **documentation**:Distance over which to taper the slab tip.tapers the initial heat content to zero and the minimum temperature to the background temperature.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3/potential mantle temperature
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3_potential-mantle-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The potential temperature of the mantle at the surface in Kelvin. If smaller than zero, the global value is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3/ridge coordinates
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3_ridge-coordinates

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:An list of ridges. Each ridge is a lists of at least 2 2d points which define the location of the ridge. You need to define at least one ridge.So the an example with two ridges is [[[10,20],[20,30],[10,40]],[[50,10],[60,10]]].
:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3/ridge coordinates/items
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3_ridge-coordinates_items

- **type**:array
- **minItems**:2
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3/ridge coordinates/items/items
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3_ridge-coordinates_items_items

- **type**:array
- **minItems**:2
- **maxItems**:2
- **documentation**:
:::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3/ridge coordinates/items/items/items
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3_ridge-coordinates_items_items_items

- **type**:number
:::::::::::

::::::::::::

:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/3/reference model name
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_3_reference-model-name

- **default value**:half space model
- **type**:string
- **documentation**:The type of thermal model to use in the mass conserving model of slab temperature. Options are half space model and plate model
::::::::::::::



:::::::::::::::

:::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/4
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_4

- **type**:object
- **documentation**:Plate model (based on McKenzie, 1970).
- **additionalProperties**:false
- **required**:[model, plate velocity]

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/4/model
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_4_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[plate model]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/4/operation
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_4_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/4/min distance slab top
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_4_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/4/max distance slab top
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_4_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/4/density
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_4_density

- **default value**:3300.0
- **type**:number
- **documentation**:The reference density of the subducting plate in $kg/m^3$
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/4/plate velocity
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_4_plate-velocity

- **default value**:NaN
- **type**:number
- **documentation**:The velocity in meters per year with which the plate subducts in meters per year.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/4/thermal conductivity
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_4_thermal-conductivity

- **default value**:2.0
- **type**:number
- **documentation**:The thermal conductivity of the subducting plate material in $W m^{-1} K^{-1}$.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/4/thermal expansion coefficient
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_4_thermal-expansion-coefficient

- **default value**:-1.0
- **type**:number
- **documentation**:The thermal expansivity of the subducting plate material in $K^{-1}$. If smaller than zero, the global value is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/4/specific heat
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_4_specific-heat

- **default value**:-1.0
- **type**:number
- **documentation**:The specific heat of the subducting plate material in $J kg^{-1} K^{-1}$. If smaller than zero, the global value is used.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/4/adiabatic heating
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_4_adiabatic-heating

- **default value**:true
- **type**:boolean
- **documentation**:Whether adiabatic heating should be used for the slab. Setting the parameter to false leads to equation 26 from McKenzie (1970),which is the result obtained from McKenzie 1969.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/4/potential mantle temperature
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_4_potential-mantle-temperature

- **default value**:-1.0
- **type**:number
- **documentation**:The potential temperature of the mantle at the surface in Kelvin. If smaller than zero, the global value is used.
::::::::::::::



:::::::::::::::

:::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/5
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_5

- **type**:object
- **documentation**:Uniform temperature model. Set the temperature to a constan value.
- **additionalProperties**:false
- **required**:[model, temperature]

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/5/model
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_5_model

- **default value**:
- **type**:string
- **documentation**:The name of the temperature model.
- **enum**:[uniform]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/5/operation
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_5_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace), add the value to the previously define value (add) or subtract the value to the previously define value (subtract).
- **enum**:[replace, add, subtract]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/5/min distance slab top
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_5_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/5/max distance slab top
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_5_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/temperature models/items/oneOf/5/temperature
:name: closed_features_items_oneOf_5_sections_items_temperature-models_items_oneOf_5_temperature

- **default value**:293.15
- **type**:number
- **documentation**:The temperature in degree Kelvin which this feature should have
::::::::::::::



:::::::::::::::


:::::::::::::::::

::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models
:name: closed_features_items_oneOf_5_sections_items_composition-models

- **documentation**:A list of composition models.
- **default value**:
- **type**:array
:::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items
:name: closed_features_items_oneOf_5_sections_items_composition-models_items

::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf

:::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf/1
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf_1

- **type**:object
- **documentation**:Compositional model object
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf/1/model
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the composition model.
- **enum**:[smooth]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf/1/min distance slab top
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf_1_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from which the composition of this layer is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf/1/max distance slab top
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf_1_max-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:The distance in meters from which the composition of this layer is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf/1/top fractions
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf_1_top-fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:The composition fraction at the top of the slab (layer).
:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf/1/top fractions/items
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf_1_top-fractions_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf/1/bottom fractions
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf_1_bottom-fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:The composition fraction at the bottom of the slab (layer).
:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf/1/bottom fractions/items
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf_1_bottom-fractions_items

- **default value**:0.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the composition which are present there.
:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf/1/operation
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf_1_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value. Replacing implies that all compositions not explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.
- **enum**:[replace, replace defined only, add, subtract]
::::::::::::::



:::::::::::::::

:::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf/2
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf_2

- **type**:object
- **documentation**:Uniform compositional model. Sets constant compositional field.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf/2/model
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the composition model.
- **enum**:[uniform]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf/2/min distance slab top
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf_2_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:todo The depth in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf/2/max distance slab top
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf_2_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:todo The depth in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf/2/compositions
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf_2_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the composition which are present there.
:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf/2/compositions/items
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf_2_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf/2/fractions
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf_2_fractions

- **type**:array
- **minItems**:1
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:TA list of compositional fractions corresponding to the compositions list.
:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf/2/fractions/items
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf_2_fractions_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/composition models/items/oneOf/2/operation
:name: closed_features_items_oneOf_5_sections_items_composition-models_items_oneOf_2_operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value. Replacing implies that all compositions not explicitly defined are set to zero. To only replace the defined compositions use the replace only defined option.
- **enum**:[replace, replace defined only, add, subtract]
::::::::::::::



:::::::::::::::


:::::::::::::::::

::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models
:name: closed_features_items_oneOf_5_sections_items_grains-models

- **documentation**:A list of grains models.
- **default value**:
- **type**:array
:::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items
:name: closed_features_items_oneOf_5_sections_items_grains-models_items

::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf

:::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/1
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_1

- **type**:object
- **documentation**:Random uniform distribution grains model. The size of the grains can be independently set to a single value or to a random distribution.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/1/model
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_1_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[random uniform distribution]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/1/min distance slab top
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_1_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:The distance from the slab top in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/1/max distance slab top
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_1_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance from the slab top in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/1/compositions
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_1_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/1/compositions/items
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_1_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/1/orientation operation
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_1_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/1/grain sizes
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_1_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be randomized between 0 and 1.
:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/1/grain sizes/items
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_1_grain-sizes_items

- **default value**:1.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/1/normalize grain sizes
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_1_normalize-grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of whether the sizes of the grains should be normalized or not. If normalized, the total of the grains of a composition will be equal to 1.
:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/1/normalize grain sizes/items
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_1_normalize-grain-sizes_items

- **default value**:true
- **type**:boolean
- **documentation**:
:::::::::::::

::::::::::::::



:::::::::::::::

:::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/2
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_2

- **type**:object
- **documentation**:Uniform grains model. All grains start exactly the same.
- **additionalProperties**:false
- **required**:[model, compositions]

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/2/model
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_2_model

- **default value**:
- **type**:string
- **documentation**:The name of the grains model.
- **enum**:[uniform]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/2/min distance slab top
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_2_min-distance-slab-top

- **default value**:0.0
- **type**:number
- **documentation**:The distance from the slab top in meters from which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/2/max distance slab top
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_2_max-distance-slab-top

- **default value**:1.7976931348623157e308
- **type**:number
- **documentation**:The distance from the slab top in meters to which the composition of this feature is present.
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/2/compositions
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_2_compositions

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the integer labels of the composition which are present there.
:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/2/compositions/items
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_2_compositions_items

- **default value**:0
- **type**:integer
- **documentation**:
:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/2/rotation matrices
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_2_rotation-matrices

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the labels of the grains which are present there for each compositions.
:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/2/rotation matrices/items
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_2_rotation-matrices_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/2/rotation matrices/items/items
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_2_rotation-matrices_items_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
:::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/2/rotation matrices/items/items/items
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_2_rotation-matrices_items_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
:::::::::::

::::::::::::

:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/2/Euler angles z-x-z
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_2_Euler-angles-z-x-z

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list with the z-x-z Euler angles of the grains which are present there for each compositions.
:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/2/Euler angles z-x-z/items
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_2_Euler-angles-z-x-z_items

- **type**:array
- **minItems**:3
- **maxItems**:3
- **uniqueItems**:false
- **documentation**:
::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/2/Euler angles z-x-z/items/items
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_2_Euler-angles-z-x-z_items_items

- **default value**:0.0
- **type**:number
- **documentation**:
::::::::::::

:::::::::::::

::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/2/orientation operation
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_2_orientation-operation

- **default value**:replace
- **type**:string
- **documentation**:Whether the value should replace any value previously defined at this location (replace) or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.
- **enum**:[replace]
::::::::::::::

::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/2/grain sizes
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_2_grain-sizes

- **type**:array
- **minItems**:0
- **maxItems**:4294967295
- **uniqueItems**:false
- **documentation**:A list of the size of all of the grains in each composition. If set to <0, the size will be set so that the total is equal to 1.
:::::::::::::{dropdown} /features/items/oneOf/5/sections/items/grains models/items/oneOf/2/grain sizes/items
:name: closed_features_items_oneOf_5_sections_items_grains-models_items_oneOf_2_grain-sizes_items

- **default value**:-1.0
- **type**:number
- **documentation**:
:::::::::::::

::::::::::::::



:::::::::::::::


:::::::::::::::::

::::::::::::::::::

::::::::::::::::::{dropdown} /features/items/oneOf/5/sections/items/coordinate
:name: closed_features_items_oneOf_5_sections_items_coordinate

- **default value**:0
- **type**:integer
- **documentation**:The coordinate which should be overwritten
::::::::::::::::::



:::::::::::::::::::

::::::::::::::::::::



:::::::::::::::::::::


:::::::::::::::::::::::

::::::::::::::::::::::::





