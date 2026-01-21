(part:dev_manual:chap:parameter_documentation:sec:features:subsec:oceanic_plate)=
Oceanic plate
==========

An oceanic plate in the Geodynamic World Builder (GWB) represents a piece of rigid lithosphere that is created at mid-ocean ridges through seafloor spreading where the upwelling mantle material partially melts and builds new oceanic crust. This plate moves laterally away from the ridge over time as new crust is produced. This motion is driven by plate tectonics and mantle convection. The plate cools and thickens with age as it moves away from the ridge axis, forming a thermal boundary layer. Older oceanic plates are colder, denser, and mechanically stronger than younger ones. With aging, it ultimately recycles at subduction zones, where the colder hence denser oceanic plate sinks back into the mantle. This full life cycle — birth, movement, cooling, and eventual destruction — defines the dynamic nature of oceanic plates in Earth’s tectonic system {cite:p}`Crameri_etal_2019`. 

The oceanic plate feature in GWB allows for the definition of a tectonic plate where the thermal structure is determined by the age of the lithosphere. This feature is an {ref}`part:user_manual:chap:concepts:sec:area_features`, meaning it is defined by a 2D surface polygon that is projected downwards between a `min depth` and `max depth`.

## Geometry
The surface extent of the oceanic plate is defined by the `coordinates` parameter, which takes a list of 2D points (Cartesian x-y or spherical latitude-longitude) to form a closed polygon. The feature is active for any point in the model that falls within this polygon and lies between the specified depth limits.

## Temperature 
The primary purpose of the oceanic plate feature is to assign temperatures based on lithospheric cooling models using `temperature models` parameter. The temperature is calculated based on the age of the plate at any given location. If the age varies across the plate, it can be calculated dynamically by providing `ridge coordinates` and a `spreading velocity`. In this case, the age at any point within the polygon is determined by its distance from the specified ridge axis and the rate of plate motion.

With the age determined, the temperature profile $T(z, t)$ is now calculated using one of the following models:

* **Adiabatic**: Defines a temperature profile that follows an adiabatic gradient with depth, representing mantle temperature increase due to compressional heating {cite:p}`Turcotte_Schubert_book_2014`.

* **Half-space cooling model**: Based on an analytical solution for a cooling semi-infinite half-space, where the oceanic lithosphere cools conductively with age as a semi-infinite medium away from a mid-ocean ridge {cite:p}`Turcotte_Schubert_book_2014`.

* **Linear model**: Applies a simple linear temperature gradient between a specified surface temperature and a basal or adiabatic temperature at depth.

* **Plate model**: This model assumes a constant temperature at a fixed basal depth, preventing the lithosphere from thickening indefinitely as it ages {cite:p}`Turcotte_Schubert_book_2014`.

* **Plate Model with Constant Age**: Same as the plate model, but with a fixed plate age, resulting in a time-independent thermal structure {cite:p}`Fowler_2005`

* **Uniform**: Assigns a constant temperature throughout the entire feature, independent of depth or age, mainly for simplified setups.


## Composition
Compositional layers (such as oceanic crust or depleted lithospheric mantle) can be added to the plate using the `composition models` parameter. These layers are defined relative to the top of the feature. The following model variations are supported:

* **Uniform**: A common model, where for example, an uniform layer of oceanic crust can be defined from the `min depth` and `max depth` down to a specific thickness, followed by a layer of lithospheric mantle. The fraction of each composition is defined by `fractions`.

* **TianWaterContent Model**: Sets bound water content as a compositional field. The returned water content is computed from the local temperature and pressure at a given point in the model domain using `max depth`, `min depth`. Bound water content is parameterized for four lithologies: sediment, mid-ocean ridge basalt (MORB), gabbro, and peridotite, using phase diagram parameterizations {cite:p}`Tian2019` set using `initial water content` and `lithology` type. Pressure is assumed to be lithostatic and is calculated using a constant, user-defined density. To prevent invalid results where the parameterization breaks down at high pressures, the pressure is limited by a user-defined cutoff value `cutoff pressure` (in GPa) for each lithology. Recommended cutoff pressures are 10 GPa for peridotite, 26 GPa for gabbro, 16 GPa for MORB, and 1 GPa for sediment.
 
```{figure} ./oceanic_plate.png
:name: oceanic plate
:alt: oceanic plate temperature.
:align: center

Geometry of the oceanic plate defined with half space cooling temperature model.
```

## Grains
Grains model in GWB is used to simulate crystal preferred orientation or lattice preferred orientation (CPO/LPO) within each composition. These are invoked within a `"grains models"` list in an oceanic plate definition and applied over the specified depth or spatial domain. The depth in meters to which the composition of this feature is present can be given as `min depth` and `max depth`. A list with the integer labels of the composition defined using `compositions` and the labels in `rotation matrices`. `Euler angles z-x-z` defines the angles of the grains for each compositions and `grain sizes` define the size of all of the grains in each composition. If set to <0, the size will be set so that the total is equal to 1. The `orientation operation` is an option to set whether the value should replace any value previously defined at this location or add the value to the previously define value (add, not implemented). Replacing implies that all values not explicitly defined are set to zero.

* **Uniform model**: This model applies a constant grain across the specified domain.

* **Random Uniform Distribution**: This model applies random uniform distribution [0,1] to each grain using the Graphic Gems III method, which are then set to the rotation matrices. The deflection for the rotation about the pole is constant.

* **Random Uniform Distribution Deflected**: Similar to the previous model but with an additional option of setting the `deflections` parameter, which lies between [0,1].


## Velocity
The composition of the model can be initiated with a velocity defined in this model. The depth in meters at which this feature must be present can be given as `min depth` and `max depth`. The following `velocity models` are supported: 

* **Uniform raw model**: A constant `velocity` in meter per year is applied. The coordinates must be in cartesian coordinate system.

* **Euler pole model**: To obtain a angular velocity this model can be used. The `euler pole` and `angular velocity` parameters define the (longitude, latitude) in degree and rotational velocity of the Euler pole in degree/Myr respectively. Spherical coordinates is generally preferred but internally support to convert cartesian coordinates to spherical is provided. In that case, a normalization is applied to the coordinates before computing the angular velocity.


## Topography
Topography models enable adding topographical features to the original composition. These serve the need when you want a predefined topography for your geodynamic or landscape evolution model runs. Likewise other models, depth in meters to which the composition of this feature is present can be given as `min depth` and `max depth`. The following `topography models` are supported:

* **Uniform**: A user-defined constant `topography` in meters is added to the composition.

* **Depth surface**: The `topography` in meters is added on the coordinates where the depth of surface is defined.



