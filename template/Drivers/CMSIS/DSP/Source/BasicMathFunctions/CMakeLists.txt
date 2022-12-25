<<<<<<< HEAD
cmake_minimum_required (VERSION 3.6)

project(CMSISDSPBasicMath)


file(GLOB SRC "./*_*.c")

add_library(CMSISDSPBasicMath STATIC ${SRC})

configdsp(CMSISDSPBasicMath ..)

### Includes
target_include_directories(CMSISDSPBasicMath PUBLIC "${DSP}/../../Include")



=======
cmake_minimum_required (VERSION 3.6)

project(CMSISDSPBasicMath)


file(GLOB SRC "./*_*.c")

add_library(CMSISDSPBasicMath STATIC ${SRC})

configdsp(CMSISDSPBasicMath ..)

### Includes
target_include_directories(CMSISDSPBasicMath PUBLIC "${DSP}/../../Include")



>>>>>>> 803d28391d70180eff564897324395ef596c0094
