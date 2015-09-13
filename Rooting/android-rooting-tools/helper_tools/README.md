helper_tools
============

This repo will contain scripts, binaries which will speed up the kernel address extraction process

Extract address:

        user@buildhost:/SO-04D/9.1.C.0.475$ /helper_tools/extract_address_and_disassemble kernel.bin
        Searching for zImage compression
        LZO compression detected
        Unpacking zImage
        lzop: <stdin>: warning: ignoring trailing garbage in lzop file
        DONE unpacking zImage
        Grabbing addresses
        [+]mmap
          mem=f69f7000 length=00bfb544 offset=c9611000
        [+]kallsyms_addresses=c0789cb0
          count=0000e7c7
        [+]kallsyms_num_syms=0000e7c7
        [+]kallsyms_names=c07c3be0
        [+]kallsyms_markers=c086b630
        [+]kallsyms_token_table=c086b9d0
        [+]kallsyms_token_index=c086bd80
        [+]kallsyms_lookup_name
        Disassembling kernel for specific functions
        59188 symbols are loaded.
        59188 symbols are loaded.
        end at 0xc00e45c4 by rnv requested
        searching for ptmx_fops
        searching for perf_swevent_enabled

See result:

        user@buildhost:/SO-04D/9.1.C.0.475$ cat addresses.txt
        c0095cb0 prepare_kernel_cred
        c00957d4 commit_creds
        c010aaf8 remap_pfn_range
        c0d02498 + 8 = (hexdec addition)    ptmx_fops
        c0ced2b4 perf_swevent_enabled
