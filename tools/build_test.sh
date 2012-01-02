#!/bin/bash

# Get Script directory
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
CB="$DIR/cbcompiler/CB.exe"
OUTDIR="$DIR/../tests/exe"

mkdir -p "$OUTDIR"
cd "$DIR/../tests"


# Process all .cb files
for i in `ls | grep \.cb`
do
	echo "Building $i"
	name=`echo $i|cut -d'.' -f1`
	wine $CB $i
	
	mv "$DIR/cbcompiler/$name.exe" "$OUTDIR"
done


