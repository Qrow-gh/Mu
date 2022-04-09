//
// Created by mbracker on 3/30/22.
//
#include <BootLdr/Init/Efi/EfiAbs.hh>
#include <BootLdr/Efi/Efi.hh>
#include <BootLdr/Elf/Elf.hh>
#include <Inc/Char.hh>


extern "C" BootLdr::Efi::Efi_Status efi_main( void * ImgHndl, BootLdr::Efi::EfiSysTbl * ST ) {
    // Vars:

    static chr16 KrnlPath[] = u"Mu\\Krnl\\Krnl.elf";
    BootLdr::Efi::Efi_Status EfiStatus;
    BootLdr::Efi::Efi_LdImgProtoc *LdImg;
    BootLdr::Efi::Efi_SimpleFileProtoc * Disk;
    BootLdr::Efi::Efi_FileProtoc * RootDir;
    BootLdr::Efi::Efi_FileProtoc * Krnl;

    // Get Disk:

    ST->BootServices->HandleProtoc(ImgHndl, &BootLdr::Efi::LdImgProtocGUID, reinterpret_cast<void **>(&LdImg));
    ST->BootServices->HandleProtoc(LdImg->DevHndl, &BootLdr::Efi::SimpleFsProtocGUID, reinterpret_cast<void **>(&Disk));
    Disk->OpenDisk(Disk, &RootDir);

    RootDir->OpenFile(RootDir, &Krnl, KrnlPath, BootLdr::Efi::Efi_FileModeRead, BootLdr::Efi::Efi_FileReadOnly);

    BootLdr::Efi::Efi_FileInfo * KrnlInfo = GetFileInfo(Krnl, ST);

    // Elf Reading

    auto KrnlSz = KrnlInfo->Sz;

    BootLdr::Elf::Elf64Ehdr KrnlEhdr;
    BootLdr::Elf::Elf64Phdr KrnlPhdr;
    void ** KrnlEHdrBuf;
    void ** KrnlPhdrBuf;
    uintn EhdrSz = sizeof( BootLdr::Elf::Elf64Ehdr );
    uintn PhdrSz = sizeof( BootLdr::Elf::Elf64Phdr );

    EfiStatus = Krnl->ReadFile( Krnl, &EhdrSz, *KrnlEHdrBuf );
    if ( EfiStatus != BootLdr::Efi::EFI_NOERR ) {
        ST->ConOut->Print( ST->ConOut, L"Error Reading Kernel Elf Header" );
    }
    KrnlEhdr = *reinterpret_cast<BootLdr::Elf::Elf64Ehdr*>(KrnlEHdrBuf);

    auto ProgHdrOffset = KrnlEhdr.e_ElfPhOff;

    EfiStatus = Krnl->SetPos( Krnl, ProgHdrOffset );
    if ( EfiStatus != BootLdr::Efi::EFI_NOERR ) {
        ST->ConOut->Print( ST->ConOut, L"Error Setting Krnl File Offset" );
    }

    EfiStatus = Krnl->ReadFile( Krnl, &PhdrSz, *KrnlPhdrBuf );
    if ( EfiStatus != BootLdr::Efi::EFI_NOERR ) {
        ST->ConOut->Print( ST->ConOut, L"Error Reading Kernel Program Header" );
    }
    KrnlPhdr = *reinterpret_cast<BootLdr::Elf::Elf64Phdr*>(KrnlPhdrBuf);



    return BootLdr::Efi::EFI_NOERR;
}
