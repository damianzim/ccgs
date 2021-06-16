# Collectible Card Game Simulation
## Running and developing

### Clone
```sh
git clone --recurse-submodules https://github.com/damianzim/ccgs.git
```

### Build
Tests are built by default.
```sh
mkdir build && cd build/
cmake ..
make
```
### Run tests
```sh
cd build/
ctest -V
```

## Third-party license

* GoogleTest ([New BSD License](https://github.com/google/googletest/blob/master/LICENSE))
* libcsv ([MIT License](https://github.com/damianzim/libcsv/blob/master/LICENSE))
* RapidJSON ([BSD License](https://github.com/Tencent/rapidjson/blob/master/license.txt))
* spdlog ([MIT License](https://github.com/gabime/spdlog/blob/v1.x/LICENSE))
* UseLATEX ([New BSD License](https://gitlab.kitware.com/kmorel/UseLATEX/-/blob/master/LICENSE))
