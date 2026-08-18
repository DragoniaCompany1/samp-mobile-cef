package com.samp.cef;

import android.app.Activity;
import android.util.Log;

/**
 * SA:MP Mobile CEF - Java Client Manager & JNI Communication (Pro Edition)
 * Copyright © 2026 drgxbytezone & Community
 */
public class CefClientManager {
    private static final String TAG = "SAMP_CEF_CLIENT";
    private final Activity mActivity;
    private CefJavaManager mJavaManager;

    static {
        try {
            System.loadLibrary("SAMPMobileCef");
            Log.i(TAG, "libSAMPMobileCef.so loaded successfully.");
        } catch (UnsatisfiedLinkError e) {
            Log.w(TAG, "libSAMPMobileCef library link pending: " + e.getMessage());
        }
    }

    public CefClientManager(Activity activity) {
        this.mActivity = activity;
    }

    public void setJavaManager(CefJavaManager javaManager) {
        this.mJavaManager = javaManager;
    }

    public CefJavaManager getJavaManager() {
        return mJavaManager;
    }

    public void onBrowserInit(boolean isSuccess, int errorCode) {
        try {
            nativeOnBrowserInit(isSuccess, errorCode);
        } catch (UnsatisfiedLinkError e) {
            Log.e(TAG, "nativeOnBrowserInit call failed: " + e.getMessage());
        }
    }

    public void sendClientEvent(String eventName, String eventDataJson) {
        try {
            nativeSendClientEvent(eventName, eventDataJson);
        } catch (UnsatisfiedLinkError e) {
            Log.e(TAG, "nativeSendClientEvent call failed: " + e.getMessage());
        }
    }

    // Native JNI Methods implemented in C++ (libSAMPMobileCef)
    private native void nativeOnBrowserInit(boolean isSuccess, int errorCode);
    private native void nativeSendClientEvent(String eventName, String eventDataJson);
}
