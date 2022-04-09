project ( Mu-Runner )
cmake_minimum_required ( VERSION 3.23 )
add_custom_target ( Run )

function ( disk_file_prepare )

if ( EXISTS "${CMAKE_CURRENT_LIST_DIR}/Meta/Mu.img" )
    return()
else()
    execute_process ( COMMAND qemu-img create ${CMAKE_CURRENT_LIST_DIR}/Meta/Mu.img 32G -f raw )
    return()
endif()

endfunction()


function ( file_system_prepare )

    add_custom_target( Mu.img )

    add_custom_command ( TARGET Mu.img COMMAND sudo bash -c ' "modprobe" "nbd" "&&"
            "qemu-nbd" "-c" "/dev/nbd0" "${CMAKE_CURRENT_LIST_DIR}/Meta/Mu.img" "-f" "raw" "&&"
            "cat" "${CMAKE_CURRENT_LIST_DIR}/Meta/MuDiskScript.sfdisk" "|" "sfdisk" "/dev/nbd0" "&&"
            "mkfs.vfat" "-F" "32" "/dev/nbd0p1" "&&"
            "mount" "/dev/nbd0p1" "/mnt" "&&"
            "cp" "-r" "${CMAKE_CURRENT_LIST_DIR}/Meta/ESP_FS/*" "/mnt" "&&"
            "umount" "/mnt" "&&"
            "qemu-nbd" "-d" "/dev/nbd0" "&&"
            "sleep" "0.3" "&&"
            "modprobe" "-r" "nbd" ' USES_TERMINAL )
endfunction()

add_custom_command ( TARGET Run
        DEPENDS Mu.img
        COMMAND bash -c ' "qemu-system-x86_64"
        "-enable-kvm"
        "-drive" "file=${CMAKE_CURRENT_LIST_DIR}/Mu.img,if=none,id=nvm,format=raw"
        "-device" "nvme,serial=RootDrive,drive=nvm"
        "-bios" "/usr/share/edk2-ovmf/x64/OVMF_CODE.fd"
        "-d" "cpu_reset,guest_errors"
        "-machine" "q35" ' USES_TERMINAL )

