# VX2740
Simple reader for CAEN VX2740 digitizer readout with MIDAS

## Build

Make sure that the submodule to read MIDAS data is available

```
git submodule update --init
```

Then configure with [cmake](https://cmake.org/download/) and compile

```
cmake .
make -j
```


## Run

```
vx2740reader.exe <midasfile1> <midasfile2> ... <midasfileN>
```