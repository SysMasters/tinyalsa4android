package com.tinyalas4android.library;

public class PcmConfig {
    /**
     * 声道数
     */
    private int channels;
    /**
     * 采样率 48000Hz
     */
    private int rate;
    /**
     * 缓冲区大小
     */
    private int periodSize;
    /**
     * 缓冲区数量
     */
    private int periodCount;
    /**
     * 采样格式
     * 16:PCM_FORMAT_S16_LE
     * 32:PCM_FORMAT_S32_LE
     */
    private int format;
    /**
     * 开始阈值
     */
    private int startThreshold;
    /**
     * 停止阈值
     */
    private int stopThreshold;
    /**
     * 静音阈值
     */
    private int silenceThreshold;


    public PcmConfig() {
        this.channels = 2;
        this.rate = 48000;
        this.periodSize = 1024;
        this.periodCount = 2;
        this.format = 32;
        this.startThreshold = 1024;
        this.stopThreshold = 1024 * 2;
        this.silenceThreshold = 1024 * 2;
    }

    public int getChannels() {
        return channels;
    }

    public void setChannels(int channels) {
        this.channels = channels;
    }

    public int getRate() {
        return rate;
    }

    public void setRate(int rate) {
        this.rate = rate;
    }

    public int getPeriodSize() {
        return periodSize;
    }

    public void setPeriodSize(int periodSize) {
        this.periodSize = periodSize;
    }

    public int getPeriodCount() {
        return periodCount;
    }

    public void setPeriodCount(int periodCount) {
        this.periodCount = periodCount;
    }

    public int getFormat() {
        return format;
    }

    public void setFormat(int format) {
        this.format = format;
    }

    public int getStartThreshold() {
        return startThreshold;
    }

    public void setStartThreshold(int startThreshold) {
        this.startThreshold = startThreshold;
    }

    public int getStopThreshold() {
        return stopThreshold;
    }

    public void setStopThreshold(int stopThreshold) {
        this.stopThreshold = stopThreshold;
    }

    public int getSilenceThreshold() {
        return silenceThreshold;
    }

    public void setSilenceThreshold(int silenceThreshold) {
        this.silenceThreshold = silenceThreshold;
    }
}
