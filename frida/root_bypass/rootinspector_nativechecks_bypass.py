import frida
import sys

package_name = "com.devadvance.rootinspector"


def get_messages_from_js(message, data):
            print(message)
            #print (message['payload'])
 

def instrument_debugger_checks():
    hook_code = """

	var didHookApis = false;

	Interceptor.attach(Module.findExportByName(null, 'dlopen'), {
	  onEnter: function (args) {
	    this.path = Memory.readUtf8String(args[0]);
	  },
	  onLeave: function (retval) {
	    if (!retval.isNull() && this.path === '/data/app-lib/com.devadvance.rootinspector-1/libnative2.so' && !didHookApis) {
	      didHookApis = true;
	      console.log("File loaded hooking");
	      hooknative2();
	      // ...
	    }
	  }
	});
	function hooknative2(){
	Interceptor.attach (Module.findExportByName ( "libnative2.so", "Java_com_devadvance_rootinspector_Root_checkifstream"), {
            onLeave: function (retval) {
                retval.replace(0);
    }
});
    Interceptor.attach (Module.findExportByName ( "libnative2.so", "Java_com_devadvance_rootinspector_Root_checkfopen"), {
            onLeave: function (retval) {
                retval.replace(0);
    }
});
    Interceptor.attach (Module.findExportByName ( "libnative2.so", "Java_com_devadvance_rootinspector_Root_checkfopen"), {
            onLeave: function (retval) {
                retval.replace(0);
    }
});
    Interceptor.attach (Module.findExportByName ( "libnative2.so", "Java_com_devadvance_rootinspector_Root_statfile"), {
            onLeave: function (retval) {
                retval.replace(0);
    }
});
    Interceptor.attach (Module.findExportByName ( "libnative2.so", "Java_com_devadvance_rootinspector_Root_runsu"), {
            onLeave: function (retval) {
                retval.replace(0);
    }
});
    Interceptor.attach (Module.findExportByName ( "libnative2.so", "Java_com_devadvance_rootinspector_Root_runls"), {
            onLeave: function (retval) {
                retval.replace(0);
    }
});
    Interceptor.attach (Module.findExportByName ( "libnative2.so", "Java_com_devadvance_rootinspector_Root_runpmlist"), {
            onLeave: function (retval) {
                retval.replace(0);
    }
});

	}
    
    """
    return hook_code

device=frida.get_usb_device()
#run package
p1=device.spawn(["com.devadvance.rootinspector"])
process = device.attach(p1)
#process = device.attach(package_name)
script = process.create_script(instrument_debugger_checks())
script.on('message',get_messages_from_js)
script.load()
device.resume(p1)
sys.stdin.read()