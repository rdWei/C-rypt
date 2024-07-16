cmake_minimum_required(VERSION 3.10)
project(C-rypt)

# Imposta i flag del compilatore
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

# Directory include
include_directories(include)

# Aggiungi le directory dei sorgenti
file(GLOB SOURCES "src/*.c")

# Aggiungi l'eseguibile
add_executable(C-rypt ${SOURCES})

# Trova la libreria PNG e aggiungila alle librerie linkate
find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})
target_link_libraries(C-rypt ${PNG_LIBRARIES} leif glfw m GL clipboard xcb)

# Imposta la directory di output per l'eseguibile
set_target_properties(C-rypt PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../out"
)

