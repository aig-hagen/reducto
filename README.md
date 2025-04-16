
# reducto - A Vacuous Reduct-based Argumentation Solver 

**reducto** is a SAT-based solver for reasoning problems in abstract argumentation. The solver is mainly built on standard SAT-encodings with some improvements. In particular, for skeptical reasoning wrt. preferred semantics it contributes a non-standard approach. reducto refrains from iterative maximisation and instead utilises a reduct-based characterisation of preferred semantics to efficiently find counterexamples for the skeptical acceptance of an argument.

## Build process

### For compilation:

- add glucose sat solver under ./sat/glucose/
- call ``` make ``` in  ./sat/glucose/simp to compile glucose
- call ``` make ``` in  ./ to compile reducto

