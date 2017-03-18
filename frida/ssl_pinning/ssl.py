import frida  
import sys

def on_message(message, data):  
    try:
        if message:
            print("[*] {0}".format(message["payload"]))
    except Exception as e:
        print(message)
        print(e)

def do_ssl():

    ssl = """

        Dalvik.perform(function () {

                var SSLPinning = Dalvik.use("com.onlycoin.android.secure.SSLPinning");

                    SSLPinning.a.overload("android.content.Context", "[Ljava.lang.String;", "java.lang.String").implementation = function (c, s, ss) {

                        send("SSLPinning");
                        send(c.toString());
                        send(s.toString());
                        send(ss.toString());
                        this.a.overload("android.content.Context", "[Ljava.lang.String;", "java.lang.String").call(this, c, s, ss);


                    };

                });

    """
    return ssl

if __name__ == "__main__":  
    try:
         process = frida.get_device_manager().enumerate_devices()[-1].attach("com.onlycoin.android")
         script = process.create_script(do_ssl())
         script.on('message', on_message)
         script.load()
         sys.stdin.read()
    except KeyboardInterrupt as e:
        sys.exit(0)