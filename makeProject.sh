#!/bin/bash

echo "HELLO"
cd tmp
rm -rfv ./*
cmake -DCMAKE_TOOLCHAIN_FILE=../iOS.cmake -DCMAKE_IOS_SDK_ROOT=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS6.1.sdk -GXcode ..
#xcodebuild -target install -configuration Debug