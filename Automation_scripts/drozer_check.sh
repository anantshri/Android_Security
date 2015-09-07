#!/bin/bash
#TODO : check if drozer exist
#TODO : Create a proper report out of it.
if [ $# -eq 1 ]
then
    PRE_CMD="drozer console connect -c"
    name=$1
    #Check for Package Info
    $PRE_CMD "run app.package.info -a $name"
    #Check for Package Manifest
    $PRE_CMD "run app.package.manifest $name"
    #identify Attack Surface
    $PRE_CMD "run app.package.attacksurface $name"
    #identify exposed activities 
    $PRE_CMD "run app.activity.info -a $name"
    #identify all content providers
    $PRE_CMD "run app.provider.info -a $name"
    #Check for various URL's found on application
    $PRE_CMD "run scanner.provider.finduris -a $name"
    #Automated SQLiCheck
    $PRE_CMD "run scanner.provider.injection -a $name"
    #Automated LFI Finder
    $PRE_CMD "run scanner.provider.traversal -a $name"
    #Run Javascript Bridge scanner
    $PRE_CMD "run scanner.misc.checkjavascriptbridge -v -a $name"
    #Run Native library scanner
    $PRE_CMD "run scanner.misc.native -v -a $name"
    #Check for readable files
    $PRE_CMD "run scanner.misc.readablefiles /data/data/$name/ -p"
    #Suid/Sgid binary detection
    #run scanner.misc.sflagbinaries -t /data/data/$name
    # Lists all WebURL's used
    $PRE_CMD "run scanner.misc.weburls -a $name"
    # List all writable files
    $PRE_CMD "run scanner.misc.writablefiles /data/data/$name"
    # Device check
    $PRE_CMD "run scanner.root.check"
    # if package is debuggable.
    $PRE_CMD "run app.package.debuggable -f $name"
    # check backup API
    $PRE_CMD "run app.package.backup -f $name"
    echo "Test Stats"
    $PRE_CMD "run information.datetime"
    $PRE_CMD "run information.deviceinfo"
    $PRE_CMD "run information.permissions"
    
    
else
    echo "Incorrect usage specify app name"
fi
