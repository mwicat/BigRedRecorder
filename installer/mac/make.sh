#!/bin/bash

set -o nounset
set -o errexit

if ! which -s appdmg; then
   echo "Install appdmg command first"
   exit 1
fi

if [[ $# -ne 1 ]]; then
   echo "Invocation: $0 VERSION_STRING"
   exit 1
fi

version="$1"
target="build/BigRedRecorder-${version}.dmg"

[ -f "$target" ] && rm "$target"
appdmg spec.json "$target"
