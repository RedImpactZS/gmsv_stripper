#!/usr/bin/env bash

SCRIPTPATH="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

mkdir -p "$SCRIPTPATH/thirdparty"
git clone https://github.com/danielga/garrysmod_common "$SCRIPTPATH/thirdparty/garrysmod_common" --recursive
export GARRYSMOD_COMMON="$SCRIPTPATH/thirdparty/garrysmod_common"
export PATH="$SCRIPTPATH/thirdparty/premake-5.0.0-beta2-linux:$PATH"
