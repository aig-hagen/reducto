
# reducto - A Vacuous Reduct-based Argumentation Solver 

**reducto** is a SAT-based solver for reasoning problems in abstract argumentation. The solver is mainly built on standard SAT-encodings with some improvements. In particular, for skeptical reasoning wrt. preferred semantics it contributes a non-standard approach. reducto refrains from iterative maximisation and instead utilises a reduct-based characterisation of preferred semantics to efficiently find counterexamples for the skeptical acceptance of an argument.

## Build process

### For compilation:
- get CryptoMiniSat 5.11.21 from https://github.com/msoos/cryptominisat/releases
- add crypto to sat/crypto/
- to compile crypto:
```
cd ./sat/crypto/
mkdir -p build && cd build
cmake .. 
make 
sudo make install 
sudo ldconfig
```
- call ``` make ``` in ./ to compile reducto

