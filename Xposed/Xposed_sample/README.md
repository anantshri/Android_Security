# Sample
Sample is a [xposed module](https://github.com/rovo89/XposedInstaller) for capturing URLs sniffed from java.net.URL and logging the callig packagename and URL to logcat and the Xposed log.

Build and tested on Nexus 5 running Android 4.4.4 with SELinux set permissive, using DalvikVM. At this time, Sample is untested with ART.

Usage
------
Its one class, less than 100 lines of code. Read it, or don't worry about it.


Revision History
----------------
v1.0 (Initial release)
 - Hook URL, log calls to Xposedlog and Logcat

License
-------
    The MIT License (MIT)
    
    Copyright (c) 2015 Red Naga
    
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:
    
    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
