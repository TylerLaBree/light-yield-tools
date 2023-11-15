#!/bin/bash
source /cvmfs/dune.opensciencegrid.org/products/dune/setup_dune.sh
setup dunesw v09_81_00d01 -q e26:prof
kx509
setup jobsub_client
