#bin/bash

if [ $# -lt 1 ]
then
        echo "Usage : $0 Debug/Release"
        exit
fi

if [ "$1" != "Debug"  ] && [ "$1" != "Release"  ] ; then
        echo "Usage : $0 Debug/Release"
fi


mkdir $1
cd $1
rm ../CMakeCache.txt
cmake -DCMAKE_BUILD_TYPE=$1 ..
make
