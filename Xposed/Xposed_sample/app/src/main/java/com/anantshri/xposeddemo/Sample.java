package com.anantshri.xposeddemo;

import android.content.Intent;
import android.util.Log;

import java.io.File;
import java.net.URL;
import java.net.URLStreamHandler;

import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.XC_MethodHook;
import de.robv.android.xposed.XposedBridge;
import de.robv.android.xposed.callbacks.XC_LoadPackage;

import static de.robv.android.xposed.XposedHelpers.findAndHookConstructor;
import static de.robv.android.xposed.XposedHelpers.findAndHookMethod;


public class Sample implements IXposedHookLoadPackage {

    //set targetPackage to a specific application, leave empty to target all
    String targetPackage = "";
    String packageName = null;

    @Override
    public void handleLoadPackage(XC_LoadPackage.LoadPackageParam lpparam) throws Throwable {

        packageName = lpparam.packageName;

        if (!targetPackage.isEmpty() && !targetPackage.equals(packageName))
            return;


        findAndHookConstructor("java.net.URL", lpparam.classLoader, String.class, new XC_MethodHook() {
            @Override
            protected void beforeHookedMethod(MethodHookParam param) throws Throwable {

                XposedBridge.log("Sample: " + packageName + " Spec: " + param.args[0]);
                Log.d("Found "+packageName+" : "+param.args[0],"Sample");

            }

        });

    }
}
