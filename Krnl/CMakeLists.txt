project ( Mu-Krnl )
cmake_minimum_required ( VERSION 3.23 )

add_executable( Krnl.elf
        Init/Init.cc)

add_custom_command( TARGET Krnl.elf COMMAND cp ${PROJECT_BINARY_DIR}/Krnl.elf ${CMAKE_SOURCE_DIR}/Meta/ESP_FS/Mu/Krnl/Krnl.elf )