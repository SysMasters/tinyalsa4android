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

    public PcmReader(int pcmCard, int pcmDevice, PcmDataCallback callback) {
        this.callback = callback;
        nativeHandler = nativeInit(pcmCard, pcmDevice);
    }

    public void release() {
        nativeDestroy(nativeHandler);
    }

    private native long nativeInit(int pcmCard, int pcmDevice);

    private native void nativeDestroy(long handler);

    private void onPcmData(byte[] data) {
        if (callback != null) {
            callback.onPcmData(data);
        }
    }
}
