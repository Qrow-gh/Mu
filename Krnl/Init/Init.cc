extern "C" [[noreturn]] void KrnlEntry() {
    while(true) {
        asm volatile ("mov $0xDEADBEEF, %rdi");
    }
}