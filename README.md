# Collectible Card Game Simulation

## License
Each submodule is distributed under its specific license. One can see it in the relevant repository. To see the license content of the `rapidjson` library which is used in this project, see `lib/rapidjson/license.txt`.

## Running and developing

### Clone
```sh
git clone git@github.com:damianzim/ccgs.git --recurse-submodules
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
