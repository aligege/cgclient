cd .
mkdir build
mkdir instdir
rem cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G "Ninja" -DCMAKE_INSTALL_PREFIX:PATH=instdir
rem cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX:PATH=instdir
rem cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DNO_CPP_INIT=1 -G "Visual Studio 17 2022" -DCMAKE_INSTALL_PREFIX:PATH=instdir
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 17 2022" -DCMAKE_INSTALL_PREFIX:PATH=instdir
