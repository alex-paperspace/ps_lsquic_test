# LSQUIC in C++

LSQUIC is an open source quic library written in C. This is a testing client/server tool trying to get basic functionality out of the library's low level interface in a C++ lib with a qmake build system.

# Building

## Windows

For Windows, it's recommended to follow the BUILD-WINDOWS.md file in the lsquic repo. However, this is also somewhat outdated. Here are some modified instructions that worked for me:

Requirements:

- Visual Studio
- cmake
- golang
- nasm
- perl 

vcpkg https://github.com/Microsoft/vcpkg

With vcpkg you want to clone it in a general purpose directory and run that bootstrap file specified in the readme. Then, run these commands:

```
vcpkg install zlib:x64-windows-static\n
vcpkg install libevent:x64-windows-static
vcpkg install pcre:x64-windows-static
vcpkg install getopt:x64-windows-static 
vcpkg integrate install
```

*Note: vcpkg might automatically install some packages under two triplets (i.e. x64-windows-static and x64-windows). This could cause a problem when building lsquic. I recommend deleting the x64-windows folder under vcpkg/installed if it does cause problems*

the last command will give you a compiler flag specifying a cmake toolchain file that you can append to the compiler flags when building lsquic

Now, you want to build boringssl. Clone it, and run cmake. Here are the inputs I use. Modify to your liking. I think there is a configuration for Debug/Release somewhere but --config doesn't work for me. Not sure if -DCMAKE_BUILD_TYPE works either.

```
cmake -DCMAKE_GENERATOR_PLATFORM=x64 -DBUILD_SHARED_LIBS=OFF .
```
Now, in a VS Developer Prompt: (can also specify /m to do a parallel build)
```
msbuild ALL_BUILD.vcxproj
```
now, set the directory as a var:
```
set BORINGSSL=%cd%
```
*Note: LSQUIC needs to find both the headers and the necessary boringssl libs to be built so if you want to build boringssl in a different directory, don't forget to let cmake know where the headers are too.*

Now build LSQUIC, modify to your liking:

```
cmake -DCMAKE_GENERATOR_PLATFORM=x64 -DBUILD_SHARED_LIBS=OFF -DVCPKG_TARGET_TRIPLET=x64-windows-static -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=path/to/vcpkg/scripts/buildsystems/vcpkg.cmake -DBORINGSSL_DIR=%BORINGSSL% .
```
and the same msbuild command as before:
```
msbuild /m ALL_BUILD.vcxproj
```

You're done. You can now build ps_lsquic_lib, provided that you specify lib locations in the pri files for boringssl and lsquic. This process will be refined in the future.

## Unix

Should be able to just build the project with qmake && make. The auxiliary targets will build boringssl and lsquic for you.

# Usage

Very unpolished at the moment. For the server, you have to generate your own openssl cert and key. Not sure if .der formats will work but .pem/.key/.crt is known to work.

Once that is done, place the cert and key where the server binary is located and rename them to cert.pem and key.pem. This is due to laziness for now. will provide them as file dialogs soon.

Once, the engine is instantiated on both sides with no errors, you should be able to communicate once you listen and connect.

# Status 

Since this project is not finished, here's what I'm working on:

- Get the handshake to function. Since these are clients/servers run on my local network, sni parsing doesn't exactly work as expected yet.
- provide sni input on client
- start working on client/server callbacks

