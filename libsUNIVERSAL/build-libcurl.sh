#!/bin/sh

###########################################################################
#  Change values here							  #
#									  #
VERSION="7.30.0"								  #
SDKVERSION="6.1"								  #
#									  #
###########################################################################
#									  #
# Don't change anything under this line!				  #
#									  #
###########################################################################

CURRENTPATH=`pwd`
DEVELOPER=`xcode-select --print-path`

set -e
if [ ! -e curl-${VERSION}.tar.gz ]; then
	echo "Downloading curl-${VERSION}.tar.gz"
    curl -O http://curl.haxx.se/download/curl-${VERSION}.tar.gz
else
	echo "Using curl-${VERSION}.tar.gz"
fi

if [ -d  ${CURRENTPATH}/src ]; then
	rm -rf ${CURRENTPATH}/src
fi

if [ -d ${CURRENTPATH}/bin ]; then
	rm -rf ${CURRENTPATH}/bin
fi

mkdir -p "${CURRENTPATH}/src"
tar zxf curl-${VERSION}.tar.gz -C "${CURRENTPATH}/src"
cd "${CURRENTPATH}/src/curl-${VERSION}"

############
# iPhone Simulator
ARCH="i386"
PLATFORM="iPhoneSimulator"
echo "Building libcurl for ${PLATFORM} ${SDKVERSION} ${ARCH}"
echo "Please stand by..."

export CC="${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer/usr/bin/gcc"
export CFLAGS="-arch ${ARCH} -isysroot ${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer/SDKs/${PLATFORM}${SDKVERSION}.sdk"
mkdir -p "${CURRENTPATH}/bin/${PLATFORM}${SDKVERSION}.sdk"

LOG="${CURRENTPATH}/bin/${PLATFORM}${SDKVERSION}.sdk/build-libcurl-${VERSION}.log"

echo "Configure libcurl for ${PLATFORM} ${SDKVERSION} ${ARCH}"

./configure -prefix=${CURRENTPATH}/bin/${PLATFORM}${SDKVERSION}.sdk -disable-shared > "${LOG}" 2>&1

echo "Make libcurl for ${PLATFORM} ${SDKVERSION} ${ARCH}"

make >> "${LOG}" 2>&1
make install >> "${LOG}" 2>&1
make clean >> "${LOG}" 2>&1

echo "Building libcurl for ${PLATFORM} ${SDKVERSION} ${ARCH}, finished"
#############

#############
# iPhoneOS armv7
ARCH="armv7"
PLATFORM="iPhoneOS"
echo "Building libcurl for ${PLATFORM} ${SDKVERSION} ${ARCH}"
echo "Please stand by..."

export CC="${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer/usr/bin/gcc"
export CFLAGS="-arch ${ARCH} -isysroot ${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer/SDKs/${PLATFORM}${SDKVERSION}.sdk"
mkdir -p "${CURRENTPATH}/bin/${PLATFORM}${SDKVERSION}-${ARCH}.sdk"

LOG="${CURRENTPATH}/bin/${PLATFORM}${SDKVERSION}-${ARCH}.sdk/build-libcurl-${VERSION}.log"

echo "Configure libcurl for ${PLATFORM} ${SDKVERSION} ${ARCH}"

./configure -prefix=${CURRENTPATH}/bin/${PLATFORM}${SDKVERSION}-${ARCH}.sdk --host=${ARCH}-apple-darwin --disable-shared > "${LOG}" 2>&1

echo "Make libcurl for ${PLATFORM} ${SDKVERSION} ${ARCH}"

make >> "${LOG}" 2>&1
make install >> "${LOG}" 2>&1
make clean >> "${LOG}" 2>&1

echo "Building libcurl for ${PLATFORM} ${SDKVERSION} ${ARCH}, finished"
#############

#############
# Universal Library
echo "Build universal library..."

lipo -create ${CURRENTPATH}/bin/iPhoneSimulator${SDKVERSION}.sdk/lib/libcurl.a ${CURRENTPATH}/bin/iPhoneOS${SDKVERSION}-armv7.sdk/lib/libcurl.a -output ${CURRENTPATH}/libcurl.a

ls ${CURRENTPATH}/bin/iPhoneSimulator${SDKVERSION}.sdk/include/curl
cp -R ${CURRENTPATH}/bin/iPhoneSimulator${SDKVERSION}.sdk/include/curl ${CURRENTPATH}/
echo "Building all steps done."
echo "Cleaning up..."
rm -rf ${CURRENTPATH}/src
rm -rf ${CURRENTPATH}/bin
echo "Done."
