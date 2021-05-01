# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
with the addition of author(s), date of change and optionally the relevant issue. 

Format:
- Description. [Name (optionally email adress); date; relevant github issue tag(s) and or pull requests]

This project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]
### Changed
- Change changelog style to markdown based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/). \[Menno Fraters; 2021-05-01; [#230](github.com/GeodynamicWorldBuilder/WorldBuilder/issues/230),[#231](https://github.com/GeodynamicWorldBuilder/WorldBuilder/pull/231)\]

## [0.3.0] - 2020-06-19
### Added
- An experimental grains interface containing grain sizes and orientations has been added. [Menno Fraters;;]
### Changed
- Fortran compiler is now optional [Menno Fraters;;]
- The world builder no longer requires or uses OpenMP. It was not used in the core  library, but used to speed up the Visualization program. That uses its own threads now. [Menno Fraters;;] 
- The usage of cmake has been moderized allowing for more flexibility [Menno Fraters;;]
### Fixed
- The code has been refactored and cleaned exstinively and new compiler warnings have been  enabled and fixed. [Menno Fraters;;]
