---
title: 'The Geodynamic World Builder: A Planetary structure creator for the geosciences'
tags:
  - C++
  - CPP
  - C
  - Fortran
  - Python
  - Geodynamics
  - Earth science
  - Tectonics
  - Seismology
authors:
  - name: Menno R. T. Fraters
    orcid: 0000-0003-0035-7723
    affiliation: "1" # (Multiple affiliations must be quoted)
  - name: Magali I. Billen
    orcid: 0000-0002-7316-1791
    affiliation: "2"
  - name: Rene Gassmöller
    orcid: 0000-0001-7098-8198
    affiliation: "1"
  - name: Arushi Saxena
    orcid: 0000-0003-2930-3497
    affiliation: "3"
  - name: Timo Heister
    orcid: 0000-0002-8137-3903
    affiliation: "3"
  - name:  Juliane Dannberg
    orcid: 0000-0003-0357-7115
    affiliation: "1"
  - name: Haoyuan Li
    orcid: 0000-0003-0676-9884
    affiliation: "2"
  - name: Daniel Douglas
    orcid: 0000-0002-7871-018X
    affiliation: "4"
  - name: Yijun Wang
    orcid: 0000-0002-7637-3239
    affiliation: "5"
affiliations:
 - name: Universiy of Florida, USA
   index: 1
 - name: UC Davis, USA
   index: 2
 - name: Clemson University, USA
   index: 3
 - name: New Mexico Tech, USA
   index: 4
 - name: University of Oslo, Norway
   index: 4
date: 28 February 2023
bibliography: paper.bib
---

# Summary

Many Earth science applications require the discretization, parametrization and/or visualization of complex geological features in a 3D geometry in global or regional settings. A prime example are geodynamic models, which have to make assumptions about the Earth's thermal and chemical structure and the geometry of different features such as plates, subducted slabs, or mantle plumes. This structure is needed in instantaneous models, as model initial conditions, or to test different hypotheses and compare model predictions to observations. Other example applications are the creation of an Earth velocity structure for seismic forward modeling and hypothesis-testing, or the visualization of tectonic features in structural geology.

The Geodynamic World Builder (GWB) has been designed to make the creation of complex parameterized models significantly easier. It can also be used to design simple synthetic models and it supports the use of several types of datasets to set up models. Besides setting up initial conditions for geodynamic models, the GWB can also be used to make detailed 3D visualizations of geologic, tectonic and geodynamic settings.

# Statement of need

The increased availability of computational resources, infrastructure and new numerical methods over the last decades has allowed for the creation of complex numerical models that closely resemble specific location on the Earth using a 3D geometry and a high resolution. However, the related increase in complexity has also made setting up these more detailed regional or global models exceedingly difficult, especially in three dimensions. Furthermore, investigating the model dynamics often requires testing different scenarios involving variations in model geometry, thermal or chemical structure, or other model assumptions. This highlights the need for changing the model setup in a way that is easy, efficient, and robust. Although successful studies have been published, such model setups often have one or many of the following disadvantages:

1. The configuration is not human-readable.
2. The software is not easily modifiable and extendable.
3. The model setup is not portable to other computing systems or reproducible in other software frameworks.
4. The model setup is not shareable with other users.

These issues lead to a number of problems with the reproducibility and reliability of modeling studies, which threaten to undermine the predictive power and usefulness of modeling results. The GWB has been designed to address these challenges, by creating human readable, parameterized, portable, reproducible and shareable geodynamic model setups. As a side benefit, the GWB comes with its own programs to visualize the constructed model through applications like Paraview. Creating the models requires no programming knowledge. Therefore, the GWB can be easily used to visualize tectonic and geodynamic settings for publications, teaching and public outreach. 

The GWB has been already been used in several published studies: TODO. Other tools to solve this problem have emerged at around the same time as the first GWB release (@Fraters_Thieulot_etal_2019). Examples include GeomIO (@Bauville_Baumann_2019 and @Spang_Baumann_2022), which uses an approach based on vector graphics, Easy (https://easyinit.readthedocs.io/), which uses a more generic function-based approach, UWGeodynamics (@Beucher_Moresi_etal_2019), which is specifically designed for Underworld (@Moresi_Dufour_2002) and GemPy (@Varga_Schaaf_2019 and @Schaaf_Varga_2021), which is designed for structural modelling. The GWB was designed to be a more general planetary structure creator, using the methods shown below.

# Methods

To address the challenges outlined in the previous section, the Geodynamic World Builder implements a specific code and world parameterization principles.

## GWB Coding Priniples
The GWB's software architecture is built around the following principles:

1. A single text-based input file
2. Code, language and platform independent
    1. Support for **Linux**, **macOS** and **Windows**
    2. Official interfaces for **C++**, **C**, **FORTRAN** and **Python** 
3. Safe parallel execution
4. Readable and extensible (modular) codes
5. Strict version numbering to ensure reproducible results

These principles are implemented in an object-oriented C++ code with the mentioned interfaces to other porgramingn languages. All part a user might want to modify are implemented as plugin systems using interface classes that decouple individual modules and allow to easily extend the code with new features. Following these principles is the first step to create a clean, portable, extendable code with reproducible results. In addition, the GWB includes an automated extensive test suite with benchmarks, integration and unit tests with high code coverage, memory checking, automatic code indentation and a spell checker to keep the GWB in a healthy state.

## GWB World Parameterization Principles

The GWB's world parameterization principles are built around the idea that a complex model region can be split into individual tectonic features. These tectonic features can be parameterized by defining their location and geometry in terms of points, lines or areas in a map view. For example, a continental plate can be represented as an area on a map, and the GWB user defines this area. A fault is a linear feature on a map, so the user can define the fault trace as a line at the surface. Users can also provide additional information for a feature, such as a spatially variable thickness or dip angle. The GWB then uses these parameters to create the 3D geometry of the feature, defining its volume. Furthermore, users can attach one or many models to those volumes to define additional properties such as thermal or chemical structure. These can be very simple models, such as a uniform temperature distribution, or follow a more complex distribution, such as a half space cooling model, or a McKenzie model [@McKenzie_1970] or mass conserving [@billen_Fraters_AGU_2023] slab temperature model. 

All these parameterizations are bundled in a single input file, which is human readable, writeable and editable. The main idea behind this design of the GWB is that users can easily create, modify and visualize complex parameterized geodynamic or tectonic setting. 

## Example 
Below we show an example input file for a Cartesian model with a subducting plate.

```json
{
  "version": "1.0",
  "coordinate system":{"model":"cartesian"},
  "features":
  [
    { 
      "model":"subducting plate", "name":"Slab", "dip point":[0,0],
      "coordinates":[[1500e3,1000e3],[1600e3,350e3],[1500e3,0]],
      "segments":[{"length":300e3, "thickness":[100e3], "angle":[0,60]}],
      "temperature models":[{"model":"plate model", "plate velocity":0.02}],
    }
  ]
}
```

Combining several of these features and changing the `coordinate system` to `spherical` can create the model setup shown in Figure 1: A plate subducting beneath an overriding ocean and a rising plume impinging on the base of the lithosphere between a mid-ocean ridge and a passive margin. The resulting input file is just 85 lines of GWB input formatted in the same way as in the example input file above.

![A schematic example of what can be built with 85 lines of a GWB input file formatted in the same way as in the example input file shown above. \label{fig:example}](../../sphinx/_static/images/user_manual/basic_starter_tutorial/BST_19.png)


# Acknowledgements

We would like to acknowledge all other contributors to the project, especially Lorraine Hwang, Becky Fildes and John Naliboff for their advice and support for this project throughout the years. We would also like to acknowledge NSF for their funding and support through grants EAR-1620618 (CPO), OCE-1948902 (Cascadia), EAR-0949446 (CIG), EAR-1550901 (CIG) and EAR-1925677 (FRES).

# References


