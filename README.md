[![Contributors](https://img.shields.io/github/contributors/GeodynamicWorldBuilder/WorldBuilder?logo=git&logoColor=eeeeee&color=success)](https://github.com/GeodynamicWorldBuilder/WorldBuilder/graphs/contributors)
[![Documentation](https://img.shields.io/badge/code-documented-success)](https://codedocs.xyz/GeodynamicWorldBuilder/WorldBuilder/index.html)
[![actions](https://img.shields.io/github/workflow/status/GeodynamicWorldBuilder/WorldBuilder/tests/master?label=tests&logo=github&logoColor=eeeeee)](https://github.com/GeodynamicWorldBuilder/WorldBuilder/actions?query=branch%3Amaster)
[![AppVeyor Build status](https://img.shields.io/appveyor/build/MFraters/WorldBuilder/master?label=tests&logo=appveyor&logoColor=eeeeee)](https://ci.appveyor.com/project/MFraters/worldbuilder)
[![Coverage Status](https://img.shields.io/coveralls/github/GeodynamicWorldBuilder/WorldBuilder?logo=coveralls&logoColor=eeeeee)](https://coveralls.io/github/GeodynamicWorldBuilder/WorldBuilder?branch=master)
[![codecov](https://img.shields.io/codecov/c/github/GeodynamicWorldBuilder/WorldBuilder/master?logo=codecov&logoColor=eeeeee)](https://codecov.io/gh/GeodynamicWorldBuilder/WorldBuilder)  
[![Matrix space](https://img.shields.io/badge/Chat-matrix.org-brightgreen?logo=matrix&logoColor=eeeeee)](https://matrix.to/#/!vhukRUGUINnZOIutoQ:matrix.org)
[![Mastodon Follow](https://img.shields.io/mastodon/follow/106136314313793382?domain=https%3A%2F%2Fsocial.mfraters.net&style=social)](https://social.mfraters.net/@world_builder)

# The Geodynamic World Builder (GWB)
![Cascadia_real7_Jps2_v5_view1_highres_v1](https://user-images.githubusercontent.com/7631629/123048354-992dc680-d3fe-11eb-8e91-6fac6125f7de.png)
## What is the Geodynamic World Builder?
The Geodynamic World Builder(GWB) is an open source code library intended to set up initial conditions for computational geodynamic models and/or visualize complex 3d teconic setting, in both Cartesian and Spherical geometries. The inputs for the JSON-style parameter file are not mathematical, but rather a structured nested list describing tectonic features, e.g. a continental, an oceanic or a subducting plate. Each of these tectonic features can be assigned a specific temperature profile (e.g. plate model) or composition label (e.g. uniform). For each point in space, the GWB can return the composition and/or temperature. It is written in C++, but can be used in almost any language through its C and Fortran wrappers. Various examples of 2D and 3D subduction settings are presented.

For more information see the the [GWB site](https://geodynamicworldbuilder.github.io/), see the automatically generated extensive [online User Manual](https://gwb.mfraters.net/manual/manual.pdf) or the automatically generated [code documentation](https://codedocs.xyz/GeodynamicWorldBuilder/WorldBuilder/index.html).

## Frequently Asked Questions (FAQ)
### What do I do when I have a question or want to request a feature?
If you have a question about the code and you can not find the answer easily in the documentation or the question is already raised as an [issue](https://github.com/GeodynamicWorldBuilder/WorldBuilder/issues), please let us know by opening an [issue on github](https://github.com/GeodynamicWorldBuilder/WorldBuilder/issues/new). This is also the preferred method for asking for new features for GWB.

### I found a mistake in the documentation or code, what should I do?
Please do not keep it to yourself and let us know. Others will also profit from mistakes being found and fixed. Even if it is just a typo in the documentation feel free to raise an issue or, even better, make a pull request to fix the issue.

### Is there a way to ask a quick question or chat with the developers or other users?
We are now also experimenting with matrix chat (#gwb:matrix.org). [Matrix](matrix.org) is an open source distributed chat system. Since the main developer is already present on matrix, a special space has been created for the world builder allowing the option for multiple rooms. The space can be found [here](https://matrix.to/#/!vhukRUGUINnZOIutoQ:matrix.org). The matrix system is not limited to one client, but using the [Element](https://element.io) client is generally recommended. Element can be used as a dektop, mobile or web application. Feel free to join!

### How to cite?
The developers of the Geodynamic World Builder request that you cite the following publication:

Fraters, M., Thieulot, C., van den Berg, A., and Spakman, W.: The Geodynamic World Builder: a solution for complex initial conditions in numerical modelling, Solid Earth, [https://doi.org/10.5194/se-10-1785-2019](https://doi.org/10.5194/se-10-1785-2019), 2019.

And cite the specific version of the software used. Version 0.4.0 can be cited as:

Menno Fraters and others. 2021, June 22. The Geodynamic World Builder v0.4.0. Zenodo. [https://doi.org/10.5281/zenodo.5014808](https://doi.org/10.5281/zenodo.5014808).

### How can I follow the progress of this project?
There are multiple ways in which you can follow this project:
 1. Watch the repository on github. This will give you an update of what happening in the repository. This happens automatically and you can set it up to notify you for different kind of events. 
 2. Follow the [world builder Mastodon account](https://social.mfraters.net/@world_builder). This is a manually updated feed which updates when there are new release or major new features merged in the main branch. More general new related to the project may also be posted.
 3. Subscribe the the Mastodon RSS feed: https://social.mfraters.net/@world_builder.rss. This will show exactly the same information as the mastodon account, but you can use any RSS reader.
 4. Visit the [world builder website](https://geodynamicworldbuilder.github.io/). Besides all kind of useful information and links, it also contains a RSS feed viewer the world builder Mastodon account.
 5. Join the [Matrix chat rooms](https://matrix.to/#/!vhukRUGUINnZOIutoQ:matrix.org)!
