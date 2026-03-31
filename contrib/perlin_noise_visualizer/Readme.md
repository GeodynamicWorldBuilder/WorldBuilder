# Perlin Noise Playground for World Builder

## Purpose

This tool helps choose physically meaningful parameters for the  
`perlin_noise` topography and can also be used to calibrate it with the thermal and compositional model by linking Perlin noise to real  
geodynamic scales and resolution.

---

## Main idea

Perlin parameters are abstract, but geodynamic models use real units (km, m).  
This tool converts everything into physical space and checks whether  
the generated topography is resolvable by your model grid.

---

## Parameters (quick explanation)

- **Frequency**: sets the size of features (higher = smaller wavelengths, lower = broader structures)  
- **Octaves**: defines how many layers of detail are added (large + smaller features)  
- **Persistence**: controls how strong the smaller-scale features are  
- **Lacunarity**: controls how quickly feature size decreases between layers  

---

## Model setup

- Model width / height (km)  
- Model depth (km)  
- **Horizontal resolution (km)**  

---

## Diagnostics (key feature)

The tool computes:


Base wavelength:
λ ≈ 1 / frequency

Smallest wavelength:
λ_min ≈ 1 / (frequency * lacunarity^(octaves - 1))

Resolution check:
cells_per_feature = λ_min / model_resolution


---

## Interpretation

| Cells per feature | Meaning |
|------------------|--------|
| ≥ 6              | ✅ Well resolved |
| 3–6              | ⚠️ Borderline |
| < 3              | ❌ Under-resolved |

---

## Example (typical case)

**Model**
- 100 × 50 km  
- resolution = 2 km  

**Example of good parameters**

frequency ≈ 1e-5
octaves = 4
lacunarity = 2


→ smallest wavelength ≈ 12 km  
→ ~6 cells per feature → good  

**Example of bad parameters**

frequency ≈ 1e-4


→ features ~1–2 km → not resolved  

---

## Rule of thumb

- Do **not** generate features smaller than ~2–4 × grid spacing  
- Prefer **≥ 6 × grid spacing** for stable results  

---

## Output

The tool generates a JSON block ready for World Builder:

```json
{
  "model": "perlin_noise",
  "min depth": 0,
  "max depth": 100000,
  "min topography": -1000,
  "max topography": 1000,
  "frequency": ...,
  "octaves": ...,
  "persistence": ...,
  "lacunarity": ...,
  "operation": "add"
}

## Usage
Open the HTML file in a browser
Set your model resolution
Tune parameters
Check resolution indicator
Copy JSON into your model