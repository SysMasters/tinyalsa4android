#ifndef PCM_READER_H
#define PCM_READER_H

#include <stddef.h> // for size_t
#include <pthread.h>

// 数据回调函数类型定义
typedef void (*PcmDataCallback)(const void *buffer, size_t size);

// PCM读取上下文结构体定义
struct PcmReaderContext {
    struct pcm *pcm;                 // PCM设备句柄
    pthread_t thread;                // 读取线程
    int running;                     // 运行标志
    PcmDataCallback callback;        // 数据回调函数
};

// 函数声明
struct PcmReaderContext *pcm_reader_init(PcmDataCallback callback);
void pcm_reader_destroy(struct PcmReaderContext *context);

#endif // PCM_READER_H
