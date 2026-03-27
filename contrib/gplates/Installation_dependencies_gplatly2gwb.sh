#!/usr/bin/env bash

set -e  # stop on error

ENV_NAME="gplates_gwb26_py311"
PYTHON_VERSION="3.11"

echo "=========================================="
echo "Creating conda environment: $ENV_NAME"
echo "=========================================="

conda create -n $ENV_NAME python=$PYTHON_VERSION -y

echo "=========================================="
echo "Activating environment"
echo "=========================================="

source "$(conda info --base)/etc/profile.d/conda.sh"
conda activate $ENV_NAME

echo "=========================================="
echo "Installing core scientific stack (conda)"
echo "=========================================="

conda install -c conda-forge -y \
    numpy=1.26.4 \
    scipy \
    pandas \
    matplotlib \
    cartopy \
    geopandas \
    rasterio \
    shapely \
    fiona \
    pyproj \
    geopy

echo "=========================================="
echo "Upgrading pip"
echo "=========================================="

python -m pip install --upgrade pip

echo "=========================================="
echo "Installing GPlates / GPlately stack (pip)"
echo "=========================================="

python -m pip install \
    gplately==2.0.0 \
    pygplates==1.0.0 \
    plate-model-manager==1.3.1 \
    pygmt==0.17.0

echo "=========================================="
echo "Installing Jupyter support"
echo "=========================================="

python -m pip install \
    ipykernel \
    jupyterlab \
    jupyter \
    ipywidgets

echo "=========================================="
echo "Installing slabdip (GitHub)"
echo "=========================================="

python -m pip install git+https://github.com/brmather/Slab-Dip.git@c2f2fe1a4378d25cd76c918b371047916602122d

echo "=========================================="
echo "Fixing setuptools version"
echo "=========================================="

python -m pip install "setuptools<82"

echo "=========================================="
echo "Registering Jupyter kernel"
echo "=========================================="

python -m ipykernel install --user --name $ENV_NAME --display-name "Python ($ENV_NAME)"

echo "=========================================="
echo "Running verification"
echo "=========================================="

python - <<EOF
import sys, numpy, pygplates, gplately, cartopy
print("Python:", sys.version)
print("Executable:", sys.executable)
print("NumPy:", numpy.__version__)
print("pygplates:", getattr(pygplates, "__version__", "unknown"))
print("pygplates path:", pygplates.__file__)
print("VelocityDeltaTimeType exists:", hasattr(pygplates, "VelocityDeltaTimeType"))
print("gplately OK")
print("cartopy:", cartopy.__version__)
EOF

echo "=========================================="
echo "INSTALLATION COMPLETE"
echo "=========================================="
echo ""
echo "⚠️  IMPORTANT:"
echo "- Do NOT use old local pygplates paths"
echo "- Do NOT upgrade NumPy to version 2"
echo "- Use kernel: Python ($ENV_NAME)"