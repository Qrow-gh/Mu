//
// Created by mbracker on 3/30/22.
//

#ifndef MU_KICKSTART_ELF_ELF_HH
#define MU_KICKSTART_ELF_ELF_HH

#include <Inc/Int.hh>
#include <Inc/Char.hh>

namespace BootLdr::Elf {

    struct Elf64Phdr {
        u32 p_ElfType;
        u32 p_ElfFlags;
        u64 p_ElfOffset;
        u64 p_ElfVirtAddr;
        u64 p_ElfPhysAddr;
        u64 p_ElfFileSz;
        u64 p_ElfMemSz;
        u64 p_ElfAlign;
    };

    struct Elf64Ehdr {
        uchr e_ElfIdent[16]; // File Magic ( 0x7F, 'E', 'L', 'F' )
        u16 e_ElfType; // Type Of Elf
        u16 e_ElfMachine; // Arch
        u32 e_ElfVersion; // Elf Version
        u64 e_ElfEntry; // Entry Point
        u64 e_ElfPhOff; // Prog Header Offset
        u64 e_ElfShOff; // Section Header Offset
        u32 e_ElfFlags; // Elf Flags
        u16 e_ElfEhSz; // Elf Header Size
        u16 e_ElfPhEntSz; // Size Of Entries In Prog Hdr Table
        u16 e_ElfPhNum; // Program Header Count
        u16 e_ElfShEntSz; // Size Of Entries In Sec Hdr Table
        u16 e_ElfShNum; // Section Header Count
        u16 e_ElfShStrNdx; // Section Header That Contains The Offset Of The Section Header String Tbl
    };
}

#endif //MU_KICKSTART_ELF_ELF_HH
