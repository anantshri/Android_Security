android\_get\_essential\_address
=================

To get root or unlock security, we need to know device address. Our new rooting and unlocking tools use device.db file by device\_database library (android\_device\_database repository) to store address in sqlite3 database format. This tool tries to search device address and store into device.db file automatically if possible.

# How to use:
1. Install tool get\_essential\_address into directory you want to run them on.
2. Place device.db file to manage into tool direcytory.
3. Move into tools directory and run tool in adb shell.

# For example:

Get new address for Xperia UL (SOL22) build 10.3.1.D.0.220 with get\_essential\_address.

      > adb push device.db /data/local/tmp
      > adb push get_essential_address /data/local/tmp
      > adb shell
      $ cd /data/local/tmp
      $ chmod 755 get_essential_address
      $ ./get_essential_address


      Device detected: SOL22 (10.3.1.D.0.220)

      Try to find address in memory...
      Attempt msm_cameraconfig exploit...
      Detected kernel physical address at 0x80208000 form iomem

      Attempt fb_mem exploit...
      Detected kernel physical address at 0x80208000 form iomem
      Failed to open /dev/graphics/fb0 due to Permission denied
      You need to manage to get remap_pfn_range address.

      Try copying kernel memory... It will take a long time.
      Attempt get_user exploit...
      Search address in memroy...
      Using kallsyms_in_memroy...
      Essential address are:
        prepare_kernel_cred = 0xc00a2fe4
        commit_creds = 0xc00a2b08
        remap_pfn_range = 0xc011a818
        vmalloc_exec = 0xc01274ec
        ptmx_fops = 0xc0f48600
