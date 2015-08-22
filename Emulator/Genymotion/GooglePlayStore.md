# Google Playstore in Genymotion

Genymotion is a x86 emulator however it is possible to run google playstore and other arm binaries by installation ARM Translation service.

# Steps

1. Download ARM Translation Installer v1.1
2. Download the correct GApps for your Android version: (https://itvends.com/gapps/)

Next Open your Genymotion VM and go to the Homescreen

Now Drag&Drop the Genymotion-ARM-Translation.zip onto the Genymotion VM window.

It should say "File transfer in progress", once it asks you to flash it click 'OK'

Now Reboot your VM using ADB. If nescessary you can simply close the VM window, not recommended

Once you're on the Homescreen again Drag&Drop the gapps-jb-20130813-signed.zip (or whatever version you got) onto your VM, and click 'OK' when asked

Once it finishes, again Reboot your VM and open the Google Play Store.

Sign in using your Google account

Once in the Store go to the 'My Apps' menu and let everything update (fixes a lot of issues), also try updating Google Play Services directly (https://play.google.com/store/apps/details?id=com.google.android.gms).
You can also search for Google Maps, update it manually (press update or install button) and you will receive the message to update Google Play Services
(For Android 4.4.4 and 5.0 is posible you need to reboot several times before updates are shown)

Now try searching for apps in Google Play and if you can find and install for example 'Netflix' or 'Google Drive' then congrats you now have ARM support and Google Play fully setup!
