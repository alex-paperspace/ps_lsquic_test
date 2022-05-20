echo "Building LSQUIC...";

TYPE=`uname`;
echo 'this is' ${TYPE};

if [ ${TYPE} = Darwin ] || [ ${TYPE} = Linux ]
    then
    
        echo "Building for Darwin/Linux"
        cmake -DLSQUIC_SHARED_LIB=OFF -DLSQUIC_BIN=OFF -DCMAKE_BUILD_TYPE=Release -DBORINGSSL_DIR=${BORINGSSL_BUILD} -B. -S${LSQUIC_SRC}
        make -j12
        
elif [ ${TYPE} = Windows ]
    then
    
        echo "Building for Windows"
        cmake -DCMAKE_GENERATOR_PLATFORM=x64 -DBUILD_SHARED_LIBS=OFF -DVCPKG_TARGET_TRIPLET=x64-windows-static -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=$TODO -DBORINGSSL_DIR=%BORINGSSL_BUILD% .
        msbuild /m ALL_BUILD.vcxproj
        
else
    echo "Unsupported OS - ${TYPE}"
fi

echo "Finished building LSQUIC.";

