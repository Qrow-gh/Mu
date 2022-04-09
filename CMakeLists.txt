project ( Mu )
cmake_minimum_required ( VERSION 3.23 )

set ( CMAKE_CXX_STANDARD 20 )
set ( CMAKE_CXX_EXTENSIONS OFF )
set ( BOOTLDR_FILE_EFI BootLdr.efi )
set ( KRNL_FILE Krnl.elf )

include_directories ( ${CMAKE_CURRENT_LIST_DIR} )

set ( BUILD_BOOTLDR ON )
set ( BUILD_KRNL ON )
set ( RUN_QEMU_VM ON )

if ( BUILD_BOOTLDR )

    message ( STATUS "Including BootLoader" )
    add_subdirectory( BootLdr )

    target_compile_options ( BootLdr.efi
            PRIVATE
            -target x86_64-unknown-windows
            -ffreestanding
            -fshort-wchar
            -mno-red-zone
            )
    target_link_options( BootLdr.efi
            PRIVATE
            -target x86_64-unknown-windows
            -nostdlib
            -Wl,-entry:efi_main
            -Wl,-subsystem:efi_application
            -fuse-ld=lld-link
            )

    list( APPEND BLDLIST BootLdr )

endif()

if ( BUILD_KRNL )

    message ( STATUS "Including Kernel" )
    add_subdirectory ( Krnl )

    target_compile_options( Krnl.elf
            PRIVATE
            -ffreestanding
            -mcmodel=large
            -mno-red-zone
            -mno-mmx
            -mno-sse
            -mno-sse2
            -fuse-ld=lld
            -nostdlib
            )
    target_link_options( Krnl.elf
            PRIVATE
            -T${CMAKE_CURRENT_LIST_DIR}/Krnl/Lnk.ld
            -fuse-ld=lld
            -nostdlib
            -static
            )

    list( APPEND BLDLIST Krnl.elf )

endif()

if ( RUN_QEMU_VM )

    message ( STATUS "Including Meta Script" )
    add_subdirectory ( Meta )

    disk_file_prepare()
    file_system_prepare()

    list( APPEND BLDLIST Meta )

endif()

add_subdirectory( Inc )