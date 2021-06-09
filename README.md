# larlite

A lightweight interface to larsoft products. Originally developed from MicroBooNE analyses.

This version is a stripped-down, reorganization of the original `larlight/larlite` fork.

It uses CMake and removes the `UserDev` folder. Header files should now be referenced with a `larlite` prefix.

## Dependencies

* ROOT 6
* Python 2 or 3

## Building

clone the repository and go into the folder

```
clone https://github.com/nutufts/larlite larlite
cd larlite
```

setup the environment variables

```
source config/setup.sh
```

make a build folder, in principle you can put this anywhere

```
mkdir build
cd build
```

run cmake to setup the build

```
cmake -DUSE_PYTHON2:bool=ON ../
```

or

```
cmake -DUSE_PYTHON3:bool=ON ../
```

finally, compile and install

```
make install
```



