#!/bin/bash

####################### build engine ####################
mkdir -p $BUILD_TEMP/engine
cd $BUILD_TEMP/engine
cmake $ENGINE_DIR
make