cmake_minimum_required(VERSION 3.1.3 FATAL_ERROR)

project(cubelib)

#relative link directories are to be turned into full paths
cmake_policy(SET CMP0015 NEW)



set(GTEST_INCLUDE_DIR "./libs/googletest/googletest/googletest/include" CACHE STRING "Paths to googletest includes")
set(GTEST_LIB_DIR "./libs/googletest/googletest/googletest/build" CACHE STRING "Paths to googletest libs")
set(GTEST_LIB "gtest" CACHE STRING "Libs for googletest")



if (MSVC)
    add_definitions(-DNOMINMAX -D_CRT_SECURE_NO_WARNINGS)
endif()


if (CMAKE_COMPILER_IS_GNUCXX)
    set(CXX_WARNINGS "-Wall -Wextra -Wpointer-arith -Wcast-align -fstrict-aliasing -Wno-unused-local-typedefs")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_WARNINGS} -fvisibility-inlines-hidden")
endif()


include_directories(
    ./include
    ./src
    ${GTEST_INCLUDE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR} #for generated headers
    
    )
link_directories(

    ${GTEST_LIB_DIR}
)


###############################################################################
add_executable(cubelib-clgen-consts
    src/cubelib/cubelib-gen-consts.cpp
    )


###############################################################################
ADD_CUSTOM_COMMAND(
    OUTPUT cubelib.clgen.h
    COMMAND cubelib-clgen-consts > cubelib.clgen.h
    DEPENDS cubelib-clgen-consts
    )

    
###############################################################################
add_library(HEADER_ONLY_TARGET STATIC include/cubelib/cubelib.h cubelib.clgen.h)
   
set_target_properties(HEADER_ONLY_TARGET PROPERTIES LINKER_LANGUAGE CXX)
###############################################################################
add_library(cubexx-formatters src/cubexx/formatters.cpp)
   
set_property(TARGET cubexx-formatters PROPERTY CXX_STANDARD 11)
set_property(TARGET cubexx-formatters PROPERTY CXX_STANDARD_REQUIRED ON)

###############################################################################
add_executable(cubelib-unittests
    src/unittests/cubelib.cpp
    
    )


target_link_libraries(cubelib-unittests
    
    ${GTEST_LIB}
    )

set_property(TARGET cubelib-unittests PROPERTY CXX_STANDARD 11)
set_property(TARGET cubelib-unittests PROPERTY CXX_STANDARD_REQUIRED ON)
###############################################################################
add_executable(cubexx-unittests
    src/unittests/cubexx.cpp
    
    )


target_link_libraries(cubexx-unittests
    cubexx-formatters
    ${GTEST_LIB}
    
    )

set_property(TARGET cubexx-unittests PROPERTY CXX_STANDARD 11)
set_property(TARGET cubexx-unittests PROPERTY CXX_STANDARD_REQUIRED ON)
###############################################################################

