#!/bin/bash

set -eu

cd data
python3 converter.py
mv metadata.json ..
cd ..
cd cpp
mkdir -p out
cd out
cmake .. -GNinja
ninja
mv helper ../../
