//
// Created by mbracker on 4/9/22.
//

#ifndef MU_BOOTLDR_INIT_EFI_EFI_HH
#define MU_BOOTLDR_INIT_EFI_EFI_HH

#include <BootLdr/Efi/Efi.hh>
#include <BootLdr/Elf/Elf.hh>
#include <Inc/Inline.hh>
namespace BootLdr::Efi {

    BootLdr::Efi::Efi_GUID LdImgProtocGUID{0x5B1B31A1, 0x9562, 0x11d2, {0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3B}};
    BootLdr::Efi::Efi_GUID SimpleFsProtocGUID{0x0964e5b22, 0x6459, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};
    BootLdr::Efi::Efi_GUID FileInfoGUID{0x9576e92, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x0, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};

    ALWAYS_INLINE BootLdr::Efi::Efi_FileInfo * GetFileInfo( BootLdr::Efi::Efi_FileProtoc * File, BootLdr::Efi::EfiSysTbl * ST ) {

    BootLdr::Efi::Efi_FileInfo *Buf = nullptr;
    uintn BufSz = sizeof(BootLdr::Efi::Efi_FileInfo) + 50;

    auto Status = BootLdr::Efi::EFI_BUFTOSML;

    ST->BootServices->AllocPool( BootLdr::Efi::Efi_AllocType::AllocAnyPg, BufSz, reinterpret_cast<void **>(&Buf) );

        Status = File->GetInfo(File, &FileInfoGUID, &BufSz, Buf);

        while ( Status == BootLdr::Efi::EFI_BUFTOSML ) {
            ST->ConOut->Print( ST->ConOut, L"Buffer To Small! Growing...\r\n" );
            ST->BootServices->FreePool( &Buf );
            BufSz += 50;
            ST->BootServices->AllocPool( BootLdr::Efi::Efi_AllocType::AllocAnyPg, BufSz, reinterpret_cast<void **>(&Buf) );
            Status = File->GetInfo(File, &FileInfoGUID, &BufSz, Buf);
        }
        return Buf;
    }
}

#endif //MU_BOOTLDR_INIT_EFI_EFI_HH
