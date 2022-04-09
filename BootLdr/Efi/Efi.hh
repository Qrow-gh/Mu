//
// Created by mbracker on 3/29/22.
//

#ifndef MU_BOOTLDR_EFI_EFI_HH
#define MU_BOOTLDR_EFI_EFI_HH

#include <Inc/Int.hh>
#include <Inc/Char.hh>

namespace BootLdr::Efi {
//----[ Types: ]------------------------------------------------------------------------------------------------------//
    #define IN
    #define OUT
    #define OPTIONAL
    #define EFI_SIMPLE_FILESYSTEM_IO_REVISION1 = 0x00010000
    #define EFI_SIMPLE_FILESYSTEM_IO_REVISION2 = 0x00020000

    using Efi_PhysAddr = u64;
    using Efi_VirtAddr = u64;
    using Efi_Hndl = void *;
    using Efi_Evnt = void *;

    enum Efi_InterfaceType {
        Efi_NativeInterface,
        Efi_PcodeInterface
    };

    enum Efi_Boolean : u8 {
        False = 0,
        True = 1
    };

    constexpr u64 GErrCode(u64 val) { return val | (1ull << 63); }

    enum Efi_Status : uintn {
        EFI_NOERR = 0,
        EFI_LDERR = GErrCode(1),
        EFI_INVPARAM = GErrCode(2),
        EFI_UNSUPORT = GErrCode(3),
        EFI_BADBUFSZ = GErrCode(4),
        EFI_BUFTOSML = GErrCode(5),
        EFI_NOTFOUND = GErrCode(14),
        EFI_SECV = GErrCode(26)
    };

    struct Efi_TableHeader {
        u64 Sig;
        u32 Rev;
        u32 HSz;
        u32 Crc;
    private:
        u32 Resv;
    };

    struct S_GUID {
        u32 Data1;
        u16 Data2;
        u16 Data3;
        u8 Data4[8];
    };

    using Efi_Tpl = uintn;

    struct Efi_Dev_Path {
    public:
        u8 Type;
        u8 SubType;
        u8 Len[2];
    };

    enum Efi_LocSearchType {
        AllHNDLS,
        ByRegNotif,
        ByProtoc
    };

    enum Efi_TimerDelay {
        TimerCancel,
        TimerPeriodic,
        TimerRelative,
        TimerTypeMax
    };

    enum E_Efi_AllocType {
        AllocAnyPg,
        AllocMaxAddr,
        AllocAddr,
        AllocType
    };

    enum E_Efi_MType {
        EfiReserved,
        EfiLoaderCode,
        EfiLoaderData,
        EfiBootSvCode,
        EfiBootSvData,
        EfiRunSvCode,
        EfiRunSvData,
        EfiConvMem,
        EfiUnusableMem,
        EfiAcpiReclaimMem,
        EfiAcpiNVSMem,
        EfiMMappedIO,
        EfiMMappedIOPort,
        EfiPalCode,
        EfiPersistMem,
        EfiMaxMemType
    };

    struct S_Efi_MemDescriptor {
    public:
        u32 Type;
        u32 Padding;
        Efi_PhysAddr PhysStart;
        Efi_VirtAddr VirtStart;
        u64 PgCount;
        u64 Attr;
    };

    struct S_EfiFileIoToken {
        Efi_Evnt Evnt;
        Efi_Status St;
        uintn BufSz;
        void * Buf;
    };

    struct S_EfiOpenProtocInfoEntry {
        Efi_Hndl AgentHNDL;
        Efi_Hndl ControllerHNDL;
        u32 Attr;
        u32 OpenCount;
    };

    struct S_SimpleTxtOutputProtocMode {
        i32 MMode;
        i32 CMode;
        i32 Attr;

        i32 CCol;
        i32 CRow;
        Efi_Boolean CVis;
    };

    using Efi_GUID = S_GUID;
    using Efi_TxtOutputProtocMode = S_SimpleTxtOutputProtocMode;
    using Efi_MType = E_Efi_MType;
    using Efi_MDesc = S_Efi_MemDescriptor;
    using Efi_AllocType = E_Efi_AllocType;
    using Efi_OpenProtocInfoEntry = S_EfiOpenProtocInfoEntry;
    using Efi_FileIoToken = S_EfiFileIoToken;

    struct S_Efi_SimpleFileSystemProtoc;
    struct S_Efi_TextOutputProtoc;
    struct S_Efi_SimpleFileProtoc;
    using Efi_SimpleFileProtoc = S_Efi_SimpleFileSystemProtoc;
    using Efi_FileProtoc = S_Efi_SimpleFileProtoc;
    using Efi_TextOutputProtoc = S_Efi_TextOutputProtoc;

    // Mode:

    constexpr static u64 Efi_FileModeRead { 0x0000000000000001 };
    constexpr static u64 Efi_FileModeWrite { 0x0000000000000002 };
    constexpr static u64 Efi_FileModeMake { 0x8000000000000000 };

    // Attr:
    constexpr static u64 Efi_FileReadOnly { 0x1 };
    constexpr static u64 Efi_FileHidden { 0x2 };
    constexpr static u64 Efi_FileSystem { 0x4 };
    constexpr static u64 Efi_FileReserved { 0x8 };
    constexpr static u64 Efi_FileDirectory { 0x10 };
    constexpr static u64 Efi_FileArchive { 0x20 };

    //----[ Text  Protoc: ]-------------------------------------------------------------------------------------------//
    // Output:

    using EFI_TXT_REST = Efi_Status ( * )(
            IN Efi_TextOutputProtoc * This,
            IN Efi_Boolean ExVer
            );
    using EFI_TXT_STR = Efi_Status ( * )(
            IN Efi_TextOutputProtoc * This,
            IN const wchr * Str
            );
    using EFI_TXT_TST_STR = Efi_Status ( * )(
            IN Efi_TextOutputProtoc * This,
            IN const wchr * Str
            );
    using EFI_TXT_QRY_MODE = Efi_Status ( * )(
            IN Efi_TextOutputProtoc * This,
            IN uintn ModeNum,
            OUT uintn * Col,
            OUT uintn * Row
            );
    using EFI_TXT_SET_MODE = Efi_Status ( * )(
            IN Efi_TextOutputProtoc * This,
            IN uintn ModeNum
            );
    using EFI_TXT_SET_ATTR = Efi_Status ( * )(
            IN Efi_TextOutputProtoc * This,
            IN uintn Attr
            );
    using EFI_TXT_CLR_SCRN = Efi_Status ( * )(
            IN Efi_TextOutputProtoc * This
            );
    using EFI_TXT_SET_CURSOR_POS = Efi_Status ( * )(
            IN Efi_TextOutputProtoc,
            IN uintn Col,
            IN uintn Row
            );
    using EFI_TXT_ENABLE_CURSOR = Efi_Status ( * )(
            IN Efi_TextOutputProtoc * This,
            IN Efi_Boolean Vis
            );

    // Input:



    //----[ BootLdr Srv: ]-----------------------------------------------------------------------------------------------//
    // Mem:

    using EFI_ALLOC_PG = Efi_Status ( * )(
            IN Efi_AllocType Type,
            IN Efi_MType MType,
            IN uintn PgCount,
            OUT Efi_PhysAddr * Mem
            );

    using EFI_REMOV_PG = Efi_Status ( * )(
            IN Efi_PhysAddr Mem,
            IN uintn PgCount
            );
    using EFI_GET_MMAP = Efi_Status ( * )(
            IN OUT uintn * MMapSz,
            IN OUT Efi_MDesc * MMap,
            OUT uintn * MMapKey,
            OUT uintn * DescSz,
            OUT u32 * DescVer
            );
    using EFI_ALLOC_POOL = Efi_Status ( * )(
            IN Efi_AllocType Type,
            IN uintn Sz,
            OUT void ** Buf
            );
    using EFI_FREE_POOL = Efi_Status ( * )(
            IN void * Buf
            );
    using EFI_SET_VADDER_MAP = Efi_Status ( * )(
            IN uintn MMapSz,
            IN uintn DescSz,
            IN u32 DescVer,
            IN Efi_MDesc * VirtMap
            );
    using EFI_CONV_PTR = Efi_Status ( * )(
            IN uintn Debug,
            IN OUT void ** Addr
            );

    // Event & Timer:

    using EFI_EVNT_NOTIF = void ( * )(
            IN Efi_Evnt Evnt,
            IN void * Context
            );
    using EFI_MAKE_EVNT = Efi_Status ( * )(
            IN u32 Type,
            IN Efi_Tpl NotifyTpl,
            IN EFI_EVNT_NOTIF NotifFn,
            IN void * NotifContext,
            OUT Efi_Evnt
            );
    using EFI_SET_TIMER = Efi_Status ( * )(
            IN Efi_Evnt Evnt,
            IN Efi_TimerDelay Type,
            IN u64 TriggerTime
            );
    using EFI_SIGEVNT = Efi_Status ( * )(
            IN Efi_Evnt Evnt
            );
    using EFI_WAITFOREVNT = Efi_Status ( * )(
            IN uintn EvntCount,
            IN Efi_Evnt * Evnt,
            OUT uintn * Idx
            );
    using EFI_CLOSEEVNT = Efi_Status ( * )(
            IN Efi_Evnt Evnt
            );
    using EFI_CHKEVNT = Efi_Status ( * )(
            IN Efi_Evnt Evnt
            );

    // Task Pri Lvl:

    using EFI_RAISE_TPL = Efi_Tpl ( * )(
            IN Efi_Tpl NTpl
            );
    using EFI_REST_TPL = Efi_Tpl ( * )(
            IN Efi_Tpl OTpl
            );

    // Protoc Handlers:

    using EFI_INST_PROTOC_INTERFACE = Efi_Status ( * )(
            IN OUT Efi_Hndl * Hndl,
            IN Efi_GUID * Protoc,
            IN Efi_InterfaceType type,
            IN void * Interface
            );
    using EFI_REINST_PROTOC_INTERFACE = Efi_Status ( * )(
            IN Efi_Hndl  Hndl,
            IN Efi_GUID * Protoc,
            IN void * OInterface,
            IN void * NInterface
            );
    using EFI_UINST_PROTOC_INTERFACE = Efi_Status ( * )(
            IN Efi_Hndl Handle,
            IN Efi_GUID * Protoc,
            IN void * Interface
            );
    using EFI_HANDLE_PROTOC = Efi_Status ( * )(
            IN Efi_Hndl Handle,
            IN Efi_GUID * Protoc,
            OUT void ** Reg
            );
    using EFI_REGISTER_PROTOC_NOTIF = Efi_Status ( * )(
            IN Efi_GUID * Protoc,
            IN Efi_Evnt Evnt,
            OUT void ** Reg
            );
    using EFI_LOCATE_HNDL = Efi_Status ( * )(
            IN Efi_LocSearchType SearchType,
            IN Efi_GUID * Protoc OPTIONAL,
            IN void * SearchKey OPTIONAL,
            IN OUT uintn * BufSz,
            OUT Efi_Hndl * Buf
            );
    using EFI_LOCATE_DEVPATH = Efi_Status ( * )(
            IN Efi_GUID * Protoc,
            IN OUT Efi_Dev_Path ** DevPath,
            OUT Efi_Hndl * Device
            );
    using EFI_INST_CONFTBL = Efi_Status ( * )(
            IN Efi_GUID * Guid,
            IN void * Tbl
            );
    using EFI_RES_SRV = Efi_Status ( * )(

            );

    // Image :

    using EFI_IMG_LD = Efi_Status ( * )(
            IN Efi_Boolean BootPolicy,
            IN Efi_Hndl ParentImgHandle,
            IN Efi_Dev_Path * FilePath,
            IN void * SourceBuffer OPTIONAL,
            IN uintn SourceSz,
            OUT Efi_Hndl * ImgHndl
            );
    using EFI_IMG_ST = Efi_Status ( * )(
            IN Efi_Hndl ImgHndl,
            OUT uintn * ExitDataSz,
            OUT chr16 ** ExitData OPTIONAL
            );
    using EFI_EXIT = Efi_Status ( * )(
            IN Efi_Hndl ImgHndl,
            IN Efi_Status ExitSt,
            IN uintn ExitDataSz,
            IN chr16 * ExitData OPTIONAL
            );
    using EFI_IMG_ULD = Efi_Status ( * )(
            IN Efi_Hndl ImgHndl
            );
    using EFI_EXIT_BOOTSRV = Efi_Status ( * )(
            IN Efi_Hndl ImgHndl,
            IN uintn MapK
            );

    // Misc :

    using EFI_STALL = Efi_Status ( * )(
            IN uintn Microsec
            );
    using EFI_GET_NXT_MONOTONIC_COUNT = Efi_Status (
            OUT u64 * Count
            );
    using EFI_SET_WATCHDOG_TIMER = Efi_Status (
            IN uintn Timeout,
            IN u64 WatchDogCode,
            IN uintn Datasz,
            IN chr16 * WatchDogData OPTIONAL
            );

    // Driver support Srv:

    using EFI_CONNECT_CON = Efi_Status ( * )(
            IN Efi_Hndl ConHndl,
            IN Efi_Hndl * DrvImgHndl OPTIONAL,
            IN Efi_Dev_Path * RemainingDevPath OPTIONAL,
            IN Efi_Boolean Recursive
            );
    using EFI_DCONNECT_CON = Efi_Status ( * )(
            IN Efi_Hndl ConHndl,
            IN Efi_Hndl DrvImgHndl OPTIONAL,
            IN Efi_Hndl ChldHndl OPTIONAL
            );

    // Open/Close Protoc Srv:

    using EFI_OPEN_PROTOC = Efi_Status ( * )(
            IN Efi_Hndl Hndl,
            IN Efi_GUID * Protoc,
            OUT void ** Interface OPTIONAL,
            IN Efi_Hndl AgentHndl,
            IN Efi_Hndl ConHndl,
            IN u32 Attr
            );
    using EFI_CLOSE_PROTOC = Efi_Status ( * )(
            IN Efi_Hndl Hndl,
            IN Efi_GUID * Protoc,
            IN Efi_Hndl AgentHndl,
            IN Efi_Hndl ConHndl
            );
    using EFI_OPEN_PROTOC_INFO = Efi_Status ( * )(
            IN Efi_Hndl Hndl,
            IN Efi_GUID * Protoc,
            OUT Efi_OpenProtocInfoEntry ** EntryBuf,
            OUT uintn * EntryCnt
            );

    // Inc Srv:

    using EFI_PROTOC_PER_HNDL = Efi_Status ( * )(
            IN Efi_Hndl Hndl,
            OUT Efi_GUID *** ProtocBuf,
            OUT uintn * ProtocBufCount
            );
    using EFI_LOCATE_HNDL_BUF = Efi_Status ( * )(
            IN Efi_LocSearchType SearchType,
            IN Efi_GUID * Protoc OPTIONAL,
            IN void * SearchKey OPTIONAL,
            IN OUT uintn * HndlCount,
            OUT Efi_Hndl ** Buf
            );
    using EFI_LOCATE_PROTOC = Efi_Status ( * )(
            IN Efi_GUID * Protoc,
            IN void * Reg OPTIONAL,
            OUT void ** Interface
            );
    using EFI_INST_MULTI_PROTOC_INTERFACES = Efi_Status ( * )(
            IN OUT Efi_Hndl * Hndl,
            ...
            );
    using EFI_UINST_MULTI_PROTOC_INTERFACES = Efi_Status ( * )(
            IN OUT Efi_Hndl Hndl,
            ...
            );

    // Crc:

    using EFI_CALCULATE_CRC32 = Efi_Status ( * )(
            IN void * Data,
            IN uintn DataSz,
            OUT u32 * Crc32
            );

    // Misc:

    using EFI_MCOPY = Efi_Status ( * )(
            IN void * Dest,
            IN void * Src,
            IN uintn Len
            );

    using EFI_MSET = Efi_Status ( * )(
            IN void * Buf,
            IN uintn Sz,
            IN u8 Val
            );
    using EFI_MAKE_EVNT_EX = Efi_Status ( * )(
            IN u32 Type,
            IN Efi_Tpl NotifTpl,
            IN EFI_EVNT_NOTIF NotifyFn OPTIONAL,
            IN const void * NotifContext OPTIONAL,
            IN const Efi_GUID EvntGrp OPTIONAL,
            OUT Efi_Evnt * Evnt
            );

    // Efi File System:

    using EFI_OPEN_DISK = Efi_Status ( * )(
            IN void * This,
            OUT Efi_FileProtoc ** Drv
            );
    using EFI_FILE_OPEN = Efi_Status ( * )(
            IN void * This,
            OUT Efi_FileProtoc ** NewHndl,
            IN chr16 * FName,
            IN u64 OpenMode,
            IN u64 Attr
            );
    using EFI_FILE_CLOSE = Efi_Status ( * )(
            IN void * This
            );
    using EFI_FILE_DELETE = Efi_Status ( * )(
            IN void * This
            );
    using EFI_FILE_READ = Efi_Status ( * )(
            IN void * This,
            IN OUT uintn * BufSz,
            OUT void * Buf
            );
    using EFI_FILE_WRITE = Efi_Status ( * )(
            IN void * This,
            IN OUT uintn * BufSz,
            IN void * Buf
            );
    using EFI_FILE_OPENEX = Efi_Status ( * )(
            IN void * This,
            OUT Efi_FileProtoc ** NewHndl,
            IN chr16 * FName,
            IN u64 OpenMode,
            IN OUT Efi_FileIoToken * Tok
            );
    using EFI_FILE_READEX = Efi_Status ( * )(
            IN void * This,
            IN OUT Efi_FileIoToken
            );
    using EFI_FILE_WRITEEX = Efi_Status ( * )(
            IN void * This,
            IN OUT Efi_FileIoToken
            );
    using EFI_FILE_SETPOS = Efi_Status ( * )(
            IN void * This,
            IN u64 Pos
            );
    using EFI_FILE_GETPOS = Efi_Status ( * )(
            IN void * This,
            OUT u64 * Pos
            );

    using EFI_FILE_GETINF = Efi_Status ( * )(
            IN void * This,
            IN Efi_GUID * InfType,
            IN OUT uintn * BufSz,
            OUT void * Buf
            );
    using EFI_FILE_SETINF = Efi_Status ( * )(
            IN void * This,
            IN Efi_GUID * InfType,
            IN uintn Bufsz,
            IN void * Buf
            );
    using EFI_FILE_FLUSH = Efi_Status ( * )(
            IN void * This
            );
    using EFI_FILE_FLUSHEX = void *;


    //----[ Tables: ]-------------------------------------------------------------------------------------------------//

    struct Efi_BootSrv {
        Efi_TableHeader Hdr;

        EFI_RAISE_TPL RaiseTaskPriLvl;
        EFI_REST_TPL RestoreOldTaskPriLvl;

        EFI_ALLOC_PG AllocatePg;
        EFI_REMOV_PG FreePg;
        EFI_GET_MMAP GetMMap;
        EFI_ALLOC_POOL AllocPool;
        EFI_FREE_POOL FreePool;

        EFI_MAKE_EVNT MakeEvnt;
        EFI_SET_TIMER SetTimer;
        EFI_WAITFOREVNT WaitForEvnt;
        EFI_SIGEVNT SigEvnt;
        EFI_CLOSEEVNT CloseEvent;
        EFI_CHKEVNT ChkEvnt;

        EFI_INST_PROTOC_INTERFACE InstProtocInterface;
        EFI_REINST_PROTOC_INTERFACE ReInstProtocInterface;
        EFI_UINST_PROTOC_INTERFACE RemovProtocInterface;
        EFI_HANDLE_PROTOC HandleProtoc;
        EFI_HANDLE_PROTOC PCHandleProtoc;
        EFI_REGISTER_PROTOC_NOTIF RegProtocNotify;
        EFI_LOCATE_HNDL LocateHndl;
        EFI_LOCATE_DEVPATH LocateDevPath;
        EFI_INST_CONFTBL InstConfTbl;

        EFI_IMG_LD LoadImg;
        EFI_IMG_ST StImg;
        EFI_EXIT Exit;
        EFI_IMG_ULD UnloadImg;
        EFI_EXIT_BOOTSRV ExitBootSrv;

        EFI_GET_NXT_MONOTONIC_COUNT NextMonotonicCnt;
        EFI_STALL Stall;
        EFI_SET_WATCHDOG_TIMER SetWDTmr;

        EFI_CONNECT_CON ConConnect;
        EFI_DCONNECT_CON ConDisconnect;

        EFI_OPEN_PROTOC OpenProtoc;
        EFI_CLOSE_PROTOC CloseProtoc;
        EFI_OPEN_PROTOC_INFO OpenProtocInfo;

        EFI_PROTOC_PER_HNDL ProtocPerHndl;
        EFI_LOCATE_HNDL_BUF LocateHndlBuf;
        EFI_LOCATE_PROTOC LocateProtoc;
        EFI_INST_MULTI_PROTOC_INTERFACES InstallMulProtocInterfaces;
        EFI_UINST_MULTI_PROTOC_INTERFACES RemoveMulProtocInterfaces;

        EFI_CALCULATE_CRC32 CalcCrc32;

        EFI_MCOPY MCopy;
        EFI_MSET MSet;
        EFI_MAKE_EVNT_EX MakeEventEx;
    };

    struct S_Efi_TextOutputProtoc {
        EFI_TXT_REST Reset;

        EFI_TXT_STR Print;
        EFI_TXT_TST_STR TestStr;

        EFI_TXT_QRY_MODE QueryModeInfo;
        EFI_TXT_SET_MODE SetMode;
        EFI_TXT_SET_ATTR SetAttr;

        EFI_TXT_CLR_SCRN ClearScreen;
        EFI_TXT_SET_CURSOR_POS MoveCursor;
        EFI_TXT_ENABLE_CURSOR EnableCursor;

        Efi_TxtOutputProtocMode * Mode;
    };

    struct S_EfiSysTbl {
        Efi_TableHeader Hdr;

        chr16 * FirmwareVendor;
        u32 FirmwareRevision;

        Efi_Hndl ConInHndl;
        void * ConIn;

        Efi_Hndl ConOutHndl;
        Efi_TextOutputProtoc * ConOut;

        Efi_Hndl ErrOutHndl;
        Efi_TextOutputProtoc * ErrOut;

        void * RuntimeServices;
        Efi_BootSrv * BootServices;

        uintn TblEntryCount;
        void * ConfigTable;
    };

    using EfiSysTbl = S_EfiSysTbl;

    struct S_Efi_SimpleFileProtoc {
        u64 Rev;
        EFI_FILE_OPEN OpenFile;
        EFI_FILE_CLOSE CloseFile;
        EFI_FILE_DELETE DelFile;

        EFI_FILE_READ ReadFile;
        EFI_FILE_WRITE WriteFile;

        EFI_FILE_GETPOS GetPos;
        EFI_FILE_SETPOS SetPos;

        EFI_FILE_GETINF GetInfo;
        EFI_FILE_SETINF SetInfo;

        EFI_FILE_FLUSH FlushFile;

        EFI_FILE_OPENEX OpenFileEX;
        EFI_FILE_READEX ReadFileEX;
        EFI_FILE_WRITEEX WriteFileEX;
        EFI_FILE_FLUSHEX FlushFileEX;
    };

    struct S_Efi_SimpleFileSystemProtoc {
        u64 Rev;
        EFI_OPEN_DISK OpenDisk;
    };

    struct S_Efi_FileInfo {
        u64 Sz;
        u64 FSz;
        u64 PhySz;
        u64 CreateTime;
        u64 LastAtime;
        u64 LastModTime;
        u64 Attr;
        chr16 FileName[];
    };

    using Efi_FileInfo = S_Efi_FileInfo;

    struct S_Efi_LdImgProtoc {

        u32 Rev;
        Efi_Hndl ParentHndl;
        EfiSysTbl * SysTbl;

        Efi_Hndl DevHndl;
        Efi_Dev_Path * FilePath;
        void * Resv;

        u32 LdOptsSz;
        void * LdOpts;

        void * ImgBs;
        u64 ImgSz;
        Efi_MType ImgCodeType;
        Efi_MType ImgDataType;
        EFI_IMG_ULD UnloadImg;
    };

    using Efi_LdImgProtoc = S_Efi_LdImgProtoc;
}

#endif //MU_BOOTLDR_EFI_EFI_HH
