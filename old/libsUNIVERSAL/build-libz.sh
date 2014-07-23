#!/bin/sh

###########################################################################
#  Change values here							  #
#									  #
VERSION="1.2.7"								  #
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
if [ ! -e zlib-${VERSION}.tar.gz ]; then
	echo "Downloading zlib-${VERSION}.tar.gz"
    curl -O http://zlib.net/zlib-${VERSION}.tar.gz
else
	echo "Using zlib-${VERSION}.tar.gz"
fi

if [ -d  ${CURRENTPATH}/src ]; then
	rm -rf ${CURRENTPATH}/src
fi

if [ -d ${CURRENTPATH}/bin ]; then
	rm -rf ${CURRENTPATH}/bin
fi

mkdir -p "${CURRENTPATH}/src"
tar zxf zlib-${VERSION}.tar.gz -C "${CURRENTPATH}/src"
cd "${CURRENTPATH}/src/zlib-${VERSION}"

############
# iPhone Simulator
ARCH="i386"
PLATFORM="iPhoneSimulator"
echo "Building libz for ${PLATFORM} ${SDKVERSION} ${ARCH}"
echo "Please stand by..."

export CC="${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer/usr/bin/gcc"
export CFLAGS="-arch ${ARCH} -isysroot ${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer/SDKs/${PLATFORM}${SDKVERSION}.sdk"
mkdir -p "${CURRENTPATH}/bin/${PLATFORM}${SDKVERSION}.sdk"

LOG="${CURRENTPATH}/bin/${PLATFORM}${SDKVERSION}.sdk/build-libz-${VERSION}.log"

echo "Configure libz for ${PLATFORM} ${SDKVERSION} ${ARCH}"

./configure -prefix=${CURRENTPATH}/bin/${PLATFORM}${SDKVERSION}.sdk --static > "${LOG}" 2>&1

echo "Make libz for ${PLATFORM} ${SDKVERSION} ${ARCH}"

make >> "${LOG}" 2>&1
make install >> "${LOG}" 2>&1
make clean >> "${LOG}" 2>&1

echo "Building libz for ${PLATFORM} ${SDKVERSION} ${ARCH}, finished"
#############

#############
# iPhoneOS armv7
ARCH="armv7"
PLATFORM="iPhoneOS"
echo "Building libz for ${PLATFORM} ${SDKVERSION} ${ARCH}"
echo "Please stand by..."

export CC="${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer/usr/bin/gcc"
export CFLAGS="-arch ${ARCH} -isysroot ${DEVELOPER}/Platforms/${PLATFORM}.platform/Developer/SDKs/${PLATFORM}${SDKVERSION}.sdk"
mkdir -p "${CURRENTPATH}/bin/${PLATFORM}${SDKVERSION}-${ARCH}.sdk"

LOG="${CURRENTPATH}/bin/${PLATFORM}${SDKVERSION}-${ARCH}.sdk/build-libz-${VERSION}.log"

echo "Configure libz for ${PLATFORM} ${SDKVERSION} ${ARCH}"

./configure -prefix=${CURRENTPATH}/bin/${PLATFORM}${SDKVERSION}-${ARCH}.sdk --archs="-arch ${ARCH}" --static > "${LOG}" 2>&1

echo "Make libz for ${PLATFORM} ${SDKVERSION} ${ARCH}"

make >> "${LOG}" 2>&1
make install >> "${LOG}" 2>&1
make clean >> "${LOG}" 2>&1

echo "Building libcurl for ${PLATFORM} ${SDKVERSION} ${ARCH}, finished"
#############

#############
# Universal Library
echo "Build universal library..."

lipo -create ${CURRENTPATH}/bin/iPhoneSimulator${SDKVERSION}.sdk/lib/libz.a ${CURRENTPATH}/bin/iPhoneOS${SDKVERSION}-armv7.sdk/lib/libz.a -output ${CURRENTPATH}/libz.a

cp -R ${CURRENTPATH}/bin/iPhoneSimulator${SDKVERSION}.sdk/include/. ${CURRENTPATH}/
echo "Building all steps done."
echo "Cleaning up..."
rm -rf ${CURRENTPATH}/src
rm -rf ${CURRENTPATH}/bin
echo "Done."
