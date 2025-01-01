#!/bin/bash

mkdir -p "./.bitray_engine/Externals/"

_deps=(
  "lz4-1.9.3-rev1-linux"
  "mimalloc-2.0.6-rev1-linux"
)

for dep in ${_deps[*]}
do
  wget "http://79.143.191.61/re/${dep}.tar.xz" -P "./.bitray_engine/Externals/"
  mkdir -p "./.bitray_engine/Externals/${dep}"
  tar -xf "./.bitray_engine/Externals/${dep}.tar.xz" -C "./.bitray_engine/Externals/${dep}"
done
