# Getting Started

## DE2
1. create a new project called 'zap' (name of the high level vhd file)
1. DE2 Cyclone II EP2C35F672C6
1. generate the qsys vhd files from `nios_system.qsys`
1. add files `nios_system/zap.vhd` and `nios_system/synthesis/nios_system.qip` to the project
1. copy files in `replacement_files` into `nios_system/synthesis/submodules/`
1. add pin assignments
1. compile the project

## C Core
1. create a new project in eclipse using the `nios_system.sopcinfo` file
1. commit only changes to source code (`.c` and `.h` files) in the `software/<eclipse_project_name>`
1. add any other important docs in /docs (like HAL header files for api reference)
