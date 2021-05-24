# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
with the addition of author(s), date of change and optionally the relevant issue. 

Add new entries a the bottom of the current list in the subheading. Item format: 
- Description. [Name; date; relevant github issue tag(s) and or pull requests]

This project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### Added
- Added basic unity build. \[Menno Fraters; 2020-08-15; [#206](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/206)\]
- Allow to chose interpolation type per feature. \[Menno Fraters; 2021-04-23; [#224](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/224)\]
- Added a benchmarks and let them automatically run by the github actions. A summary of the results are automatically posted on the github pages.  \[Menno Fraters; 2021-05-22; [#238](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/238)\]
- Added an option to not build the unit tests.  \[Menno Fraters; 2021-05-22; [#238](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/238)\]
- Added an option for Continuous Monotone Spline interpolation. This is useful for faults and slabs which need to be contious and smooth. \[Menno Fraters; 2021-05-24; [#130](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/130),[#237](github.com/GeodynamicWorldBuilder/WorldBuilder/pull/237)\]

### Changed
- Change minimum chame version fom 2.8.12 to 2.8.13. \[Menno Fraters; 2020-11-16; [#215](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/215)\]
- Change minimum xcode version to 11. \[Menno Fraters; 2020-12-10; [#217](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/217)\]
- Change changelog style to markdown based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/). \[Menno Fraters; 2021-05-01; [#230](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/230),[#231](https://github.com/GeodynamicWorldBuilder/WorldBuilder/pull/231)\]
- Changed the underlying function which determines when you are inside a slab or a fault. This may slightly change the result, but the new result is more realistic I think. \[Menno Fraters; 2021-05-01; [#229](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/229)\]
- The Vizualizer now uses compressed output by default. This decreases the file size and increases perforamnce. \[Menno Fraters; 2021-05-22; [#239](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/239)\]
- The Vizualizer buffers output before it writes it to a file. This increases performance. \[Menno Fraters; 2021-05-22; [#239](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/239)\]

### Fixed
- Fixed namespaces, adding `WorldBuilder::` where needed. \[Timo Heister; 2020-08-10; [#205](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/205)\] 

## [0.3.0] - 2020-06-19
### Added
- An experimental grains interface containing grain sizes and orientations has been added. [Menno Fraters;;]
### Changed
- Fortran compiler is now optional [Menno Fraters;;]
- The world builder no longer requires or uses OpenMP. It was not used in the core  library, but used to speed up the Visualization program. That uses its own threads now. [Menno Fraters;;] 
- The usage of cmake has been moderized allowing for more flexibility [Menno Fraters;;]
### Fixed
- The code has been refactored and cleaned exstinively and new compiler warnings have been  enabled and fixed. [Menno Fraters;;]
