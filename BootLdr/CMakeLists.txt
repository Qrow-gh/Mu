project ( Mu-BootLdr )
cmake_minimum_required ( VERSION 3.23 )

add_executable( BootLdr.efi
        Efi/Efi.hh
        Elf/Elf.cc
        Elf/Elf.hh
        Init/Efi/EfiEntry.cc
        )

add_custom_command ( TARGET BootLdr.efi COMMAND cp ${PROJECT_BINARY_DIR}/BootLdr.efi ${CMAKE_SOURCE_DIR}/Meta/ESP_FS/ )