#!/bin/sh

###########################################################################

CURRENTPATH=`pwd`
BUILD_PATH="${CURRENTPATH}/"

echo "Start building libraries."

#if [ -d lib ]; then
#	rm -rf  ${BUILD_PATH}
#fi
mkdir -p "${BUILD_PATH}/"

cd "${CURRENTPATH}/"
/bin/sh build-libz.sh
cd "${CURRENTPATH}/"
/bin/sh build-libcurl.sh

echo "Building libraries done."
