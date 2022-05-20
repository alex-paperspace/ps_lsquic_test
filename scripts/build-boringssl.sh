PATH=$PATH:/usr/local/bin;

echo "Building BoringSSL...";

TYPE=`uname`;
echo 'this is ' ${TYPE};

if [ ${TYPE} = Darwin ] || [ ${TYPE} = Linux ]
    echo "Building for Darwin/Linux"
    then cmake -DCMAKE_BUILD_TYPE=Release -B. -S$SOURCEDIR/bssl/boringssl && make
elif [ ${TYPE} = Windows ]
    then cmake -DCMAKE_GENERATOR_PLATFORM=x64 -DBUILD_SHARED_LIBS=OFF -DOPENSSL_NO_ASM=1 . && msbuild ALL_BUILD.vcxproj
else echo "Unsupported OS - ${TYPE}"
fi

echo "Finished building BoringSSL.";
