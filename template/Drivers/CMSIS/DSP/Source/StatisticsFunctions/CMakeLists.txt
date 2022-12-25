<<<<<<< HEAD
cmake_minimum_required (VERSION 3.6)

project(CMSISDSPStatistics)


file(GLOB SRC "./*_*.c")

add_library(CMSISDSPStatistics STATIC ${SRC})

configdsp(CMSISDSPStatistics ..)

### Includes
target_include_directories(CMSISDSPStatistics PUBLIC "${DSP}/../../Include")



=======
cmake_minimum_required (VERSION 3.6)

project(CMSISDSPStatistics)


file(GLOB SRC "./*_*.c")

add_library(CMSISDSPStatistics STATIC ${SRC})

configdsp(CMSISDSPStatistics ..)

### Includes
target_include_directories(CMSISDSPStatistics PUBLIC "${DSP}/../../Include")



>>>>>>> 803d28391d70180eff564897324395ef596c0094
