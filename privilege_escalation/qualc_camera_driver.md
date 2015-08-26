# Qualcom Camera Driver 

__Ref:__ https://www.codeaurora.org/projects/security-advisories/multiple-issues-camera-drivers-cve-2014-9410-cve-2015-0568

__CVE:__ CVE-2014-9410, CVE-2015-0568


__Affected Products:__
1. Android for MSM
1. Firefox OS for MSM
1. QRD Android


CVE-2014-9410:
The MSM-VFE31 driver provides an ioctl system call interface to user space clients for communication. When processing this communication, the vfe31_proc_general function uses the user-supplied value cmd->id as an index to a buffer for read and write operations without any boundary checks.



CVE-2015-0568:
The MSM-Camera driver provides an ioctl system call interface to user space clients for communication. When processing this communication, the msm_set_crop function frees a previously allocated buffer when an error is encountered while attempting to copy data from userspace. The error handling neglects to mark the buffer as being freed, leading to a use-after free vulnerability when the buffer is used again in subsequent ioctl calls.



Patchs: (as per vendor website)

We advise customers to apply the following patches:

    CVE-2014-9410:
    https://www.codeaurora.org/patches/quic/la/PATCH_8064_113026_check_vfe_cmd_usage_20150728.tar.gz
    CVE-2015-0568:
    https://www.codeaurora.org/patches/quic/la/PATCH_8064_113028_cropinfo_pointer_fix_20150728.tar.gz

