(part:user_manual:chap:cookbooks:sec:3d_cartesian_orogenic_setup)=
3D Cartesian Orogenic Setup
======================

A three-dimensional Cartesian model representing an idealized orogenic system with laterally variable lithospheric structure.

The files can be found [here](https://github.com/GeodynamicWorldBuilder/WorldBuilder/tree/main/cookbooks/3d_cartesian_orogenic_setup).

The relevant part of the World Builder file looks like this:

:::{literalinclude} ../3d_cartesian_orogenic_setup.wb
:language: json
:lineno-start: 1
:::

Model setup
-----------

In this setup, we define a three-dimensional model domain with dimensions of 500 km in the x-direction, 2000 km in the y-direction, and 200 km in the vertical (z) direction.

Within this domain, three major tectonic regions are prescribed:

- **Domain 1 (left):** undeformed lithosphere  
- **Domain 2 (right):** stable lithosphere  
- **Domain 3 (center):** thickened orogenic domain  

Domains 1 and 2 are initialized with a reference lithospheric structure representative of stable continental regions, consisting of:

- an upper crust of 25 km  
- a lower crust of 15 km  
- a mantle lithosphere of 80 km  

This corresponds to a lithosphere–asthenosphere boundary (LAB) at approximately 120 km depth.

In contrast, the central orogenic domain is characterized by a thickened lithosphere:

- the upper crust increases to 35 km  
- the lower crust thickens progressively to a maximum of 60 km toward the orogenic center  
- the mantle lithosphere is also thickened, with the LAB deepening to approximately 155 km  

This configuration represents a fully developed orogenic root.

An initial surface topography of 4 km is imposed over the orogenic domain to reflect elevated relief.

Model visualization
-------------------

:::{figure} Figure1_setup.png
:name: orogenic_model_domain
:alt: Model domain of the orogenic setup.
:align: center

Model domain of the orogenic setup constructed using WorldBuilder. Gray represents the upper crust, light orange the lower crust, orange the lithospheric mantle, and red the asthenosphere.
:::

:::{figure} Figure2_composition_setup.png
:name: orogenic_composition
:alt: Composition setup of the model.
:align: center

Composition setup of the model.
:::

:::{figure} Figure3_Temperatures.png
:name: orogenic_temperature_profile
:alt: Temperature structure of the model.
:align: center

Temperature structure of the model.
:::

:::{figure} Figure4_Orogenic_domains.png
:name: orogenic_topography
:alt: Orogenic domains with imposed topography.
:align: center

Orogenic domains with an initial surface topography of 4 km.
:::
