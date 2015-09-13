android\_device\_database\_utils
=================

To get root or unlock security, we need to know device address. Our new rooting and unlocking tools use device.db file by device\_database library (android\_device\_database repository) to store address in sqlite3 database format. But it is too hard to manage address by SQL statement, so tools in this repository will help you.

# List of tools:
- show_address.sh

    Show all address for the device.

- delete_address.sh

    Delete all address for the device.

- export_address.sh

    Export all address for the device to text file format. Some devices needs extra property name to detect unique device build such as `ro.lge.swversion` for LG devices, `ro.aa.romver` for HTC devices and so on.  File name will be `model name`\_`build id`.txt or `model name`\_`build id`\_`extra property name`\_`extra property value`.txt if extra property name is needed.

- import_address.sh [extra property name]

    Import all address for the device from text file format.

- set_address.sh [name] [value]

    Manually set address for the the device.

- setup_variables.sh

    Do not use this directly, it is a setup script used by tools above.

# How to use:
1. Install tools into directory you want to run them on.
2. Place device.db file to manage into tools directory.
3. Move into tools directory and run tool in adb shell.

# For examples:

Get new address for Xperia UL (SOL22) build 10.3.1.D.0.220 with another tool get\_essential\_address (android\_get\_essential\_address repository). This tool finds address automatically and store result into device.db.

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

Show address for this device from device.db.

      > ./show_address.sh
      10000|commit_creds|-1073075448
      10000|prepare_kernel_cred|-1073074204
      10000|ptmx_fops|-1057716736
      10000|remap_pfn_range|-1072584680
      10000|vmalloc_exec|-1072532244

Export address for this device to text file.

      > ./export_address.sh
      Export address to 'SOL22_10.3.1.D.0.220.txt'

See result.

      > cat SOL22_10.3.1.D.0.220.txt
      commit_creds|-1073075448
      prepare_kernel_cred|-1073074204
      ptmx_fops|-1057716736
      remap_pfn_range|-1072584680
      vmalloc_exec|-1072532244

Delete address for this device.

      > ./delete_address.sh

Show address for this device again. There is no address in device.db.

      > ./show_address.sh

Import address for this device.

      > ./import_address.sh
      Import address from 'SOL22_10.3.1.D.0.220.txt'
      commit_creds: -1073075448
      prepare_kernel_cred: -1073074204
      ptmx_fops: -1057716736
      remap_pfn_range: -1072584680
      vmalloc_exec: -1072532244

See result.

      > ./show_address.sh
      10000|commit_creds|-1073075448
      10000|prepare_kernel_cred|-1073074204
      10000|ptmx_fops|-1057716736
      10000|remap_pfn_range|-1072584680
      10000|vmalloc_exec|-1072532244
