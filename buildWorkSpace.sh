#!/bin/bash

echo ""
echo "<<< BUILD WORKSPACE >>>"
echo ""

ROOT_PATH=$(cd $(dirname $0) && pwd)
echo "Working Directory : $ROOT_PATH"

IOS_SDK_ROOT="/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS6.1.sdk"

# build Xcode project for testbed
TESTBED_PATH=$ROOT_PATH/testbed/tmp
if [ -d "$TESTBED_PATH" ]
then
	echo "building testbed Xcode project into $TESTBED_PATH"
	cd "$TESTBED_PATH"
	rm -rfv ./*;
	cmake -GXcode "$ROOT_PATH"
else
	echo "<< error >> building testbed Xcode project: missing directory $TESTBED_PATH"
fi;

# build Xcode project for iOS application
IOSAPP_PATH=$ROOT_PATH/iOS_FrontEnd/tmp
if [ -d "$IOSAPP_PATH" ]
then
	echo "building iOS application Xcode project into $IOSAPP_PATH"
	cd "$IOSAPP_PATH"
	rm -rfv ./*
	cmake -DCMAKE_TOOLCHAIN_FILE=$ROOT_PATH/iOS.cmake -DCMAKE_IOS_SDK_ROOT=$IOS_SDK_ROOT -GXcode $ROOT_PATH/model
else
	echo "<< error >> building iOS application Xcode project: missing directory $IOSAPP_PATH"
fi;

#cd tmp
#rm -rfv ./*
#cmake -DCMAKE_TOOLCHAIN_FILE=../iOS.cmake -DCMAKE_IOS_SDK_ROOT=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS6.1.sdk -GXcode ..
#xcodebuild -target install -configuration Debug

echo ""