package com.tinyalas4android.library;

public class PcmReader {
    static {
        System.loadLibrary("tinyalsa");
    }

    private long nativeHandler;
    private int pcmCard;
    private int pcmDevice;
    private PcmDataCallback callback;

    public PcmReader(int pcmCard, int pcmDevice, PcmDataCallback callback) {
        this.pcmCard = pcmCard;
        this.pcmDevice = pcmDevice;
        this.callback = callback;
    }

    public void start() {
        nativeHandler = nativeInit(pcmCard, pcmDevice);
    }

    public void stop() {
        int ret = nativeDestroy(nativeHandler);
        if (ret == 0) {
            nativeHandler = -1;
        }
    }

    private void onPcmData(byte[] data) {
        if (callback != null && nativeHandler != -1) {
            callback.onPcmData(data);
        }
    }

    private native long nativeInit(int pcmCard, int pcmDevice);

    private native int nativeDestroy(long handler);


    public interface PcmDataCallback {
        void onPcmData(byte[] data);
    }

}
