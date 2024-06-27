# cpp rest sdk.

This project provides an example http server & client for building rest end points in c/c++. 

# Dependencies

1) OpenSSL 3.1
2) lib Curl
3) microsoft rest sdk
4) toml (included as a submodule)
5) solidity compiler(solc)


# Install dependencies
1) To install solc
```bash
brew update
brew upgrade
brew tap ethereum/ethereum
brew install solidit
```

2) To install the microsoft rest sdk.
```bash
 brew install cpprestsdk
 ```

 3) CMake tries to build a solidity contract that uses openzepplin libraries. To install:
 ```bash
cd contracts
npm install @openzeppelin/contracts
```

4) To clone the toml11 submodule
```bash
 git submodule update --init --recursive
```

## Building
```bash
mkdir build
cd build
 cmake ../ -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
```
