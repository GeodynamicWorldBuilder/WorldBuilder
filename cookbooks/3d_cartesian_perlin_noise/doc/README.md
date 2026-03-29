(part:user_manual:chap:cookbooks:sec:3d_cartesian_perlin_noise)=
3D Cartesian Perlin Noise
========================

This cookbook demonstrates how to set up a 3D Cartesian model in World Builder using Perlin noise to generate spatially varying temperature and composition fields.



:::{figure} normal_field.png
:name: perlin_noise_normal_field
:alt: Normal temperature field (without noise)
:align: center
:::

:::{figure} noise_field.png
:name: perlin_noise_noise_field
:alt: Perlin noise field
:align: center
:::

:::{figure} combined_field.png
:name: perlin_noise_combined_field
:alt: Combined temperature field (with noise)
:align: center
:::

## Model description

This example contains two main features: a mantle layer and a continental plate, each with temperature and composition models. Perlin noise is used to introduce realistic, multi-scale variations in both temperature and composition fields.

Perlin noise is a gradient-based noise function that produces smooth, natural-looking variations. In World Builder, it is implemented by layering multiple octaves of noise at different frequencies and amplitudes. For each octave, the frequency is multiplied by `lacunarity` and the amplitude by `persistence`. The final noise value is scaled to the range between `min temperature` and `max temperature`, and combined with the base field using the specified `operation` (e.g., `add`, `replace`).

**Parameter effects:**

- `min temperature` / `max temperature`: Set the output value range for the noise field.
- `octaves`: Number of noise layers combined; more octaves add detail and complexity.
- `lacunarity`: Frequency multiplier for each octave (e.g., `frequency *= lacunarity`, default: 2.0).
- `persistence`: Amplitude multiplier for each octave (e.g., `amplitude *= persistence`, default: 0.5).
- `frequency`: Sets the initial scale of the noise pattern.
- `operation`: How the noise is combined with the base field (e.g., `add` means `field += noise`, `replace` means the noise overwrites the field).

Default values are used if parameters are omitted.

## Input files

The main World Builder input file for this example is:

:::{literalinclude} ../perlin_noise.wb
:language: json
:::

The grid file used to visualize the model is:

:::{literalinclude} ../perlin_noise.grid
:::

Some more figures to demonstrate different perlin noise:

:::{figure} freq5.png
:align: center
:::

:::{figure} lac2.png
:align: center
:::

:::{figure} per0.8.png
:align: center
:::

:::{figure} rand.png
:align: center
:::

