file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_library(platform-basic-path
ethernet.cpp
display.cpp
${HEADERS})

target_include_directories(platform-basic-path PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(platform-basic-path systemc)

add_executable(riscv-vp-path
        main.cpp)

target_link_libraries(riscv-vp-path rv32 platform-basic-path platform-common gdb-mc ${Boost_LIBRARIES} systemc pthread)

INSTALL(TARGETS riscv-vp-path RUNTIME DESTINATION bin)
