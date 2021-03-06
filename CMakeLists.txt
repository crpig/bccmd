PROJECT(bccmd)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CMAKE_C_FLAGS "-W -g -O2")

# find all the source file under src
FILE(GLOB_RECURSE TOOLS_LIST "tools/*.c")
FILE(GLOB_RECURSE LIB_LIST "lib/*.c")
FILE(GLOB_RECURSE SRC_LIST "src/*.c")

# find the include directories
FILE(GLOB_RECURSE HEADER_LIST "*.h")
SET (INCLUDE_DIRS "")
FOREACH(_headerFile ${HEADER_LIST})
    get_filename_component(_dir ${_headerFile} PATH)
    LIST(APPEND INCLUDE_DIRS ${_dir})
ENDFOREACH()
LIST(REMOVE_DUPLICATES INCLUDE_DIRS)
INCLUDE_DIRECTORIES(${INCLUDE_DIRS})

FIND_PACKAGE(Threads)

ADD_LIBRARY(bt_test SHARED ${SRC_LIST} ${LIB_LIST} ${TOOLS_LIST})
TARGET_LINK_LIBRARIES(bt_test ${CMAKE_THREAD_LIBS_INIT})
# TARGET_LINK_LIBRARIES(bt_test rt)
TARGET_LINK_LIBRARIES(bt_test dl)

ADD_EXECUTABLE(bccmd bccmd.c ${TOOLS_LIST} ${LIB_LIST})

ADD_EXECUTABLE(test test.c ${TOOLS_LIST} ${LIB_LIST})

