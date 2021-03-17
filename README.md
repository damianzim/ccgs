# Collectible Card Game Simulation

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
