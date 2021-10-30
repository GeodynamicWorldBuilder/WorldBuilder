# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
with the addition of author(s), date of change and optionally the relevant issue. 

Add new entries a the bottom of the current list in the subheading. Item format: 
- Description. [Name; date; relevant github issue tag(s) and or pull requests]

This project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### Added
- The World Builder Visualizer can now compress vtu files with zlib and output in binary throught the vtu11 library. ASCII output is still avaiable.  \[Menno Fraters; 2021-06-26; [#282](github.com/GeodynamicWorldBuilder/WorldBuilder/pull/282)\]
- The option to use a half space cooling model for the temperature of an oceanic plate has been added to the oceanic plate feature. \[Magali Billen; 2021-07-15; [#317](github.com/GeodynamicWorldBuilder/WorldBuilder/pull/317)\] 
- Information about the depth of the reference surface in the PointDistanceFromCurvedPlanes structure. \[Menno Fraters; 2021-07-20; [#320](github.com/GeodynamicWorldBuilder/WorldBuilder/pull/320)\]
- The option for a smooth slab temperature structure that conserves mass with distance along the slab surface. \[Magali Billen; 2021-07-28; [#323](github.com/GeodynamicWorldBuilder/WorldBuilder/pull/323)\]
- The option for a temperature model for an oceanic plate with a constant age using the plate model. \[Haoyuan Li; 2021-10-29; [#344](https://github.com/GeodynamicWorldBuilder/WorldBuilder/pull/344)\]
 
### Changed
- The World Builder Visualizer will now use zlib compression for vtu files by default. If zlib is not available binary output will be used. \[Menno Fraters; 2021-06-26; [#282](github.com/GeodynamicWorldBuilder/WorldBuilder/pull/282)\]
- The return argument type of the distance_point_from_curved_planes function has been converted from a map to a struct, requiring a change in the plugin interfaces for 'fault_models' and 'subducting_plate_models', but significantly increasing the speed of the function and all functions that access its returned values. \[Rene Gassmoeller; 2021-06-27; [#289](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/289)\]
- The plugin systems (temperature, composition and grains) and the distance_point_from_curved_planes function now all pass a precomputed NaturalCoordinate, besides just the cartesian position. It turns out that this can make a significant performance differce. \[Issue found and solution suggested by Wolfgang Bangerth, implemented and tested by Menno Fraters; 2021-07-03; [#300](github.com/GeodynamicWorldBuilder/WorldBuilder/pull/300) and [#219](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/219)\]
- Introduces a bounding box for searching the fault and the subducting plate model properties (temperature, composition, grains) in the lateral direction. This reduces the computation time by constraining the number of points for which the expensive distance_point_from_curved_planes function is called. \[Arushi Saxena; 2021-07-15; [#219](https://github.com/GeodynamicWorldBuilder/WorldBuilder/issues/219)\]
- Chaning the internal interface of the get function to add a new structure (AdditionalParameters) to hold additional parameters (total_local_segment_length and local_thickness). The local_thickness has been moved away from the PointDistanceFromCurvedPlanes structure.  \[Menno Fraters; 2021-09-08; [#330](github.com/GeodynamicWorldBuilder/WorldBuilder/pull/282)\]
- Changed the name of the main branch from master to main \[Menno Fraters; 2021-10-28; [#350](https://github.com/GeodynamicWorldBuilder/WorldBuilder/pull/350)\]

### Fixed
- Using slabs and faults beyond the -180 to 180 range gave issues. These are now fixed and it now works and is tested for the -380 to 380 range. \[Menno Fraters; 2021-10-22; [#338](https://github.com/GeodynamicWorldBuilder/WorldBuilder/issues/338), [#340](https://github.com/GeodynamicWorldBuilder/WorldBuilder/pull/340) and [#342](https://github.com/GeodynamicWorldBuilder/WorldBuilder/pull/342)\]
- The ridge coordinates for the mass conservative slab temperature model where not convered to radians interally. \[Menno Fraters; 2021-10-27; [#352](https://github.com/GeodynamicWorldBuilder/WorldBuilder/issues/352)\]

## [0.4.0] - 2021-06-22
### Added
- Added basic unity build. \[Menno Fraters; 2020-08-15; [#206](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/206)\]
- Allow to chose interpolation type per feature. \[Menno Fraters; 2021-04-23; [#224](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/224)\]
- Added a benchmarks and let them automatically run by the github actions. A summary of the results are automatically posted on the github pages.  \[Menno Fraters; 2021-05-22; [#238](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/238)\]
- Added an option to not build the unit tests.  \[Menno Fraters; 2021-05-22; [#238](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/238)\]
- Added an option for Continuous Monotone Spline interpolation. This is useful for faults and slabs which need to be continuous and smooth. \[Menno Fraters; 2021-05-24; [#130](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/130),[#237](github.com/GeodynamicWorldBuilder/WorldBuilder/pull/237)\]

### Changed
- Change minimum chame version fom 2.8.12 to 2.8.13. \[Menno Fraters; 2020-11-16; [#215](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/215)\]
- Change minimum xcode version to 11. \[Menno Fraters; 2020-12-10; [#217](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/217)\]
- Change changelog style to markdown based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/). \[Menno Fraters; 2021-05-01; [#230](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/230),[#231](https://github.com/GeodynamicWorldBuilder/WorldBuilder/pull/231)\]
- Changed the underlying function which determines when you are inside a slab or a fault. This may slightly change the result, but the new result is more realistic I think. \[Menno Fraters; 2021-05-01; [#229](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/229)\]
- The Vizualizer now uses compressed output by default. This decreases the file size and increases perforamnce. \[Menno Fraters; 2021-05-22; [#239](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/239)\]
- The Vizualizer buffers output before it writes it to a file. This increases performance. \[Menno Fraters; 2021-05-22; [#239](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/239)\]
- In the fault temperature model linear, the entiry top temperature is changed to center temperature and the entry bottom temperature is changed to side temperature, since this represents the actual sides more accurately. \[Menno Fraters; 2021-07-09; [#260](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/260)\]
- A large overhoal of the distance_point_from_curved_planes function improving the accuracy in spherical coordinates. This may slighty change the results, but it should be an improvement both in accuracy and performance. Also makes available a coordinate system independent distrance function and some fast trigonometric functions. \[Menno Fraters; 2021-06-11; [#255](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/255)\]

### Fixed
- Fixed namespaces, adding `WorldBuilder::` where needed. \[Timo Heister; 2020-08-10; [#205](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/205)\] 
- Fixed bug in linear temperature models in several features when using a top/center temperature which computes the background (-1). \[Menno Fraters; 2021-06-22; [#273](github.com/GeodynamicWorldBuilder/WorldBuilder/pull/273)\] 

## [0.3.0] - 2020-06-19
### Added
- An experimental grains interface containing grain sizes and orientations has been added. [Menno Fraters;;]
### Changed
- Fortran compiler is now optional [Menno Fraters;;]
- The world builder no longer requires or uses OpenMP. It was not used in the core  library, but used to speed up the Visualization program. That uses its own threads now. [Menno Fraters;;] 
- The usage of cmake has been moderized allowing for more flexibility [Menno Fraters;;]
### Fixed
- The code has been refactored and cleaned exstinively and new compiler warnings have been  enabled and fixed. [Menno Fraters;;]
