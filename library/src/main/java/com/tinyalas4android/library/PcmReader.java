package com.tinyalas4android.library;

public class PcmReader {
    static {
        System.loadLibrary("tinyalsa");
    }

    private long nativeHandler;
    private int pcmCard;
    private int pcmDevice;
    private PcmDataCallback callback;

    private PcmConfig pcmConfig;


    /**
     * config.channels = 2; // 双声道
     * config.rate = 48000; // 采样率 48000Hz
     * config.period_size = 512; // 周期大小
     * config.period_count = 2; // 周期计数
     * config.format = PCM_FORMAT_S16_LE; // 16位小端序格式
     * config.start_threshold = 1;
     */
    public PcmReader(int pcmCard, int pcmDevice, PcmDataCallback callback) {
        this.pcmCard = pcmCard;
        this.pcmDevice = pcmDevice;
        this.callback = callback;
        pcmConfig = new PcmConfig();
    }

    public void start() {
        nativeHandler = nativeInit(pcmCard, pcmDevice, pcmConfig);
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

    private native long nativeInit(int pcmCard, int pcmDevice, PcmConfig pcmConfig);

    private native int nativeDestroy(long handler);


    public interface PcmDataCallback {
        void onPcmData(byte[] data);
    }


    public PcmConfig getPcmConfig() {
        return pcmConfig;
    }
}
