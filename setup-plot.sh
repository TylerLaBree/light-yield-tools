#!/bin/bash
# Tyler LaBree
# Northern Illinois University

# Setup the latest versions of root and dune-plot-style
source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
setup root v6_26_06b -q e26:p3913:prof
setup dune_plot_style v01_00

# Teach ROOT about the dune-plot-style include directory
export ROOT_INCLUDE_PATH=$DUNE_PLOT_STYLE_INC:$ROOT_INCLUDE_PATH
