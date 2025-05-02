
# reducto - A Vacuous Reduct-based Argumentation Solver 
## SAT solver comparison
This is a branch of the reducto solver created to compare different SAT solvers.
Currently this version offers to compare the solver CryptoMiniSat 5.11.21 and Glucose 4.2.1.
To run the **reducto** with the SAT solver Cadical, see the main branch.

## Compilation

- `make all`:  to compile all SAT solvers and the reducto solver, which will be available under `./build/`.
- `make glucose`: to compile only the Glucose SAT solver
- `make cryptominisat`: to compile only the CryptoMiniSat SAT solver
- `make solo SOLVER=SATSOLVER`: to compile only reducto with the specified SAT solver 'SATSOLVER'

## Features
Supported problems: [DC-ST,DC-CO,DS-ST,DS-PR,SE-ST,SE-PR]

Supported file-formats : [.i23, .tgf]	

use `--help` for further information

<p>
created by
<br>
<a href="https://www.fernuni-hagen.de/aig/team/lars.bengel.shtml">Lars Bengel</a>,
<a href="https://www.fernuni-hagen.de/aig/team/julian.sander.shtml">Julian Sander</a> and
<a href="https://www.fernuni-hagen.de/aig/team/matthias.thimm.shtml">Matthias Thimm</a>


