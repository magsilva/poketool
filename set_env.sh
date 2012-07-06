#!/bin/bash

echo "Note: This script only works when run with 'source set_env.sh' or '. set_env.sh'."

DIR=$(dirname $(readlink -f $1))
export NEWPOKETABS="$DIR/bin/poketabs"
export NEWLITABS="$DIR/bin/litabs"
export PATH="$PATH:$DIR/bin"
