This PoC is divided into three Parts,
the folder mediaserver help to inject code into mediaserver from a normal application.
the folder surfaceflinger help to inject code to surfaceflinger after you got mediaserver permission.
the folder system_server help to inject code to system_server after you got surfaceflinger permission.
the bbshell folder help to inject busybox to mediaserver

the PoC contain many hard codes, I tested it on Nexus 5 for Android 5.0(LRX21O), you may have to adust these hard codes to suit your case.
detail introduce about the vulnerability please refer to 
https://www.blackhat.com/docs/us-15/materials/us-15-Gong-Fuzzing-Android-System-Services-By-Binder-Call-To-Escalate-Privilege-wp.pdf
