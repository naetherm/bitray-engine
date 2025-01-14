#!/bin/bash

mkdir -p "./.bitray_engine/Externals/"
EXTERNALS_DIR="./.bitray_engine/Externals"

_deps=(
  "glm-0.9.9.8-rev1-linux"
  "lz4-1.9.3-rev1-linux"
  "mimalloc-2.0.6-rev1-linux"
  "pcre-8.37-linux"
  "smolv-20200525-linux"
)

for dep in ${_deps[*]}
do
  if [ ! -f "${EXTERNALS_DIR}/${dep}.tar.xz" ]
  then
    wget "http://79.143.191.61/re/${dep}.tar.xz" -P ${EXTERNALS_DIR}
    mkdir -p "${EXTERNALS_DIR}/${dep}"

    if [ ! -f "${EXTERNALS_DIR}/${dep}" ]
    then
      tar -xf "${EXTERNALS_DIR}/${dep}.tar.xz" -C "./.bitray_engine/Externals/${dep}"
    fi
  else
    echo "${dep}.tar.xz already present, skipping ... "
  fi
done

# Download the LunarG Vulkan SDK to the Externals directory as well
VULKAN_SDK_VERSION="1.3.239.0"
#VULKAN_SDK_VERSION="1.2.131.2"
VULKAN_SDK_PATH="https://sdk.lunarg.com/sdk/download/${VULKAN_SDK_VERSION}/linux/vulkansdk-linux-x86_64-${VULKAN_SDK_VERSION}.tar.gz"

if [ ! -f "${EXTERNALS_DIR}/vulkansdk-linux-x86_64-${VULKAN_SDK_VERSION}.tar.gz" ]
then
  DEP="vulkansdk-linux-x86_64-${VULKAN_SDK_VERSION}"
  wget ${VULKAN_SDK_PATH} -P ${EXTERNALS_DIR}
fi

if [ ! -f "${EXTERNALS_DIR}/vulkansdk-linux" ]
then
  DEP="vulkansdk-linux-x86_64-${VULKAN_SDK_VERSION}"
  mkdir -p "${EXTERNALS_DIR}/vulkansdk-linux"
  tar -xf "${EXTERNALS_DIR}/${DEP}.tar.gz" -C "./.bitray_engine/Externals/vulkansdk-linux"
fi