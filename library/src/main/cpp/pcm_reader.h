#ifndef PCM_READER_H
#define PCM_READER_H

#include <cstddef>
#include <pthread.h>
#include "include/tinyalsa/pcm.h"

// 数据回调函数类型定义
typedef void (*PcmDataCallback)(const void *buffer, size_t size, long handler);

// PCM读取上下文结构体定义
struct PcmReaderContext {
    struct pcm *pcm;                 // PCM设备句柄
    pthread_t thread;                // 读取线程
    int running;                     // 运行标志
    PcmDataCallback callback;        // 数据回调函数
};

// 函数声明
struct PcmReaderContext *
pcm_reader_init(int pcm_card,
                int pcm_device,
                int channels,
                int sample_rate,
                int period_size,
                int period_count,
                int format,
                int start_threshold,
                int stop_threshold,
                int silence_threshold, PcmDataCallback callback);

int pcm_reader_destroy(struct PcmReaderContext *context);

#endif // PCM_READER_H
