#!/bin/bash
# Create two linux versions of cbEnchanted
# to the "build/release" directory and pack
# them as .tar.gz files, with the contents
# of VERSION file added to the file name.
# README.txt and customfunctions.cb will
# be added to the package, too.
#
# If a file with the same name already
# exists, it will be deleted.
#
# This file should reside at the root of
# cbEnchanteds git repository.
#
# Whatever options you pass to this script
# will be passed along to `make`. No tests
# of what the arguments are is made.
#
# The final file will look like this:
# cbEnchanted_x64-0.1.0.tar.gz
#  - cbEnchanted_x64-0.1.0/ 
#    -- CBEnchanted
#    -- CBEnchanted_debug
#    -- README
#    -- customfunctions.cb

# Stop on error
set -e

# Find out the name of the file and directory
NAME="cbenchanted_x"
NAME+="`getconf LONG_BIT`"
NAME+="-"
NAME+=`cat VERSION | head -n1`
echo "Release to be created:" $NAME

# A kick-ass way to get the directory of this file
# http://stackoverflow.com/a/246128/1152564
SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ] ; do SOURCE="$(readlink "$SOURCE")"; done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )"

# If "build" dir doesn't exist, create it.
if [ ! -d "$DIR/build" ]; then
	mkdir "$DIR/build"
fi

# If "build/release" dir doesn't exists, create it.
if [ ! -d "$DIR/build/release" ]; then
	mkdir "$DIR/build/release"
fi

# If inside "build/release" there's a matching release
# already, delete it.
if [ -d "$DIR/build/release/$NAME" ]; then
	rm -r -f "$DIR/build/release/$NAME"
	echo "INFO: Removed directory build/release/$NAME"
fi

# Make a dir for this release
mkdir "$DIR/build/release/$NAME"
echo "INFO: Created directory build/release/$NAME"

# cd to build
cd "$DIR/build"

# Run cmake, create release first
echo "---------------------------------"
echo "##  Running cmake for Release  ##"
echo "---------------------------------"
cmake -DCMAKE_BUILD_TYPE:string=Release -DTEST:bool=true ..

echo "---------------------------------"
echo "##        Running make         ##"
echo "---------------------------------"
# Run make, pass along arguments given to this script
make $@

# Move the built CBEnchanted file to right place
mv CBEnchanted "$DIR/build/release/$NAME/CBEnchanted"

echo "---------------------------------"
echo "##   Running cmake for Debug   ##"
echo "---------------------------------"
cmake -DCMAKE_BUILD_TYPE:string=Debug -DTEST:bool=true ..

echo "---------------------------------"
echo "##        Running make         ##"
echo "---------------------------------"
# Run make, pass along arguments given to this script
make $@

# Move the built CBEnchanted file to right place
mv CBEnchanted "$DIR/build/release/$NAME/CBEnchanted_debug"

# Copy README-release.txt as README and
# tests/include/customfunctions.cb to the directory
cp "$DIR/README-release.txt" "$DIR/build/release/$NAME/README"
cp "$DIR/tests/include/customfunctions.cb" "$DIR/build/release/$NAME/customfunctions.cb"

echo "---------------------------------"
echo "##        Packing files        ##"
echo "---------------------------------"
cd "$DIR/build/release/"
tar -cavf "$NAME.tar.gz" $NAME/

# Remove the unnecessary directory
rm -r -f "$DIR/build/release/$NAME/"

echo "All done."
