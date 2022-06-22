#!/bin/bash

####################### build extra ####################

#webp
mkdir -p $BUILD_TEMP/libwebp-1.2.2
cd $BUILD_TEMP/libwebp-1.2.2
if [ ! -f "build_ok" ]; then
    echo ">>>>>>>>>>>>>>>>>>>>>>>>>>> build libwebp-1.2.2 >>>>>>>>>>>>>>>>>>>>>>>";
    CC=$CC $EXTRA_DIR/libwebp-1.2.2/configure --prefix=$INSTALL_DIR \
        --disable-jpeg --disable-tiff --disable-gif --disable-wic \
        --with-pkgconfigdir=$INSTALL_DIR/lib/pkgconfig
    make -j 8 && make install
    if [ $? -eq 0 ]; then
        touch build_ok;
        echo "libwebp-1.2.2 compiled successfully, please delete \"build_ok\" file in $BUILD_TEMP/libwebp-1.2.2 if you wanted rebuild.";
    else
        echo "libwebp-1.2.2 compiled fail, please fixed error and retry.";
        exit 0;
    fi;
else
    echo "libwebp-1.2.2 has been compiled.";
fi;

#ffmpeg
mkdir -p $BUILD_TEMP/ffmpeg-5.0.1
cd $BUILD_TEMP/ffmpeg-5.0.1
if [ ! -f "build_ok" ]; then
    echo ">>>>>>>>>>>>>>>>>>>>>>>>>>> build ffmpeg-5.0.1 >>>>>>>>>>>>>>>>>>>>>>>";
    CC=$CC $EXTRA_DIR/ffmpeg-5.0.1/configure --prefix=$INSTALL_DIR \
        --enable-shared
    make -j 8 && make install
    if [ $? -eq 0 ]; then
        touch build_ok;
        echo "ffmpeg-5.0.1 compiled successfully, please delete \"build_ok\" file in $BUILD_TEMP/ffmpeg-5.0.1 if you wanted rebuild.";
    else
        echo "ffmpeg-5.0.1 compiled fail, please fixed error and retry.";
        exit 0;
    fi;
else
    echo "ffmpeg-5.0.1 has been compiled.";
fi;