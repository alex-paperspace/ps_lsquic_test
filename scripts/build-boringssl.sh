echo "Building BoringSSL...";

TYPE=`uname`;
echo 'this is' ${TYPE};

if [ ${TYPE} = Darwin ] || [ ${TYPE} = Linux
    then
    
        echo "Building for Darwin/Linux"
        cmake -DCMAKE_BUILD_TYPE=Release -B. -S$BORINGSSL_SRC
        make -j12
        echo "Copying headers..."
        cp -R $BORINGSSL_SRC/include ./include
        echo "Copied."
        
elif [ ${TYPE} = Windows ]
    then
    
        echo "Building for Windows"
        cmake -DCMAKE_GENERATOR_PLATFORM=x64 -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=OFF -DOPENSSL_NO_ASM=1 .
        msbuild /m ALL_BUILD.vcxproj
        
else
    echo "Unsupported OS - ${TYPE}"
fi

echo "Finished building BoringSSL.";






