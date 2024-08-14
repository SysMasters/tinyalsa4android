package com.jackstudio.tinyalas4android;

public class PcmReader {
    static {
        System.loadLibrary("tinyalsa");
    }

    private long nativeHandler;

    public interface PcmDataCallback {
        void onPcmData(byte[] data);
    }

    private PcmDataCallback callback;

    public PcmReader(PcmDataCallback callback) {
        this.callback = callback;
        nativeHandler = nativeInit();
    }

    public void release() {
        nativeDestroy(nativeHandler);
    }

    private native long nativeInit();

    private native void nativeDestroy(long handler);

    private void onPcmData(byte[] data) {
        if (callback != null) {
            callback.onPcmData(data);
        }
    }
}
