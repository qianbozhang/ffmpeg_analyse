#!/bin/bash

####################### build multimedia ####################
source ./build/set_env.sh

./build/build_extra.sh

./build/build_engine.sh

./build/unset_env.sh