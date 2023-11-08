cmake_minimum_required(VERSION 3.0.0)
project(ECElecar VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)

include_directories(./includes/)

add_executable(ECElecar ./src/main.c
                        ./src/linkedList.c 
                        ./src/display.c 
                        ./src/unassociable.c 
                        ./src/loadAssemblyLine.c 
                        ./src/handleError.c 
                        ./src/free.c
                        ./src/loadGraph.c
                        ./src/kahnAlgorithm.c
                        ./src/quickSort.c
                        ./src/sortByCycleTime.c
                        ./src/menu.c)
