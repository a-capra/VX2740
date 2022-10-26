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

## Output for scope-mode firmware at NTF with PDU+

Assuming `Verbose = true` as is by default

```
UnpackVX2740Module::Analyze VX2740 bank D000 found... Board: 0
VX2740 decoder for bank D000
     Num bytes: 20024
   Data Format: 0x10
  Channel Mask: 0x80008181
 Event counter: 765
  Trigger time: 478329622
   Num samples: 2000
  Num channels: 5
Unpack VX2740 ETTT: 3.82664 s	Event Counter: 765
TRawChannel board:   0 ch:   0 idx:   0 # of samples:   2000
32723,<...>
TRawChannel board:   0 ch:   7 idx:   7 # of samples:   2000
32792,<...>
TRawChannel board:   0 ch:   8 idx:   8 # of samples:   2000
32744,<...>
TRawChannel board:   0 ch:  15 idx:  15 # of samples:   2000
32610,<...>
TRawChannel board:   0 ch:  31 idx:  31 # of samples:   2000
32779,<...>
```

where `<...>` is a shorthand for the 2000 samples separated by a comma. 
Channels 0, 7, 8 and 15 correspond to the 4 quadrants of the PDU+.
Channel 31 is the external trigger.

