#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <tinyalsa/asoundlib.h>
#include <android/log.h>
#include <string.h>
#include "pcm_reader.h"


void *pcm_read_thread(void *arg) {
    struct PcmReaderContext *context = (struct PcmReaderContext *) arg;
//    size_t size = pcm_frames_to_bytes(context->pcm, pcm_get_buffer_size(context->pcm));
//    void *buffer = malloc(size);


    struct pcm *pcm = context->pcm;
    unsigned int frame_size = pcm_frames_to_bytes(pcm, 1);
    unsigned int frames_per_sec = pcm_get_rate(pcm);

    void *frames = malloc(frame_size * frames_per_sec);
    if (frames == nullptr) {
        fprintf(stderr, "failed to allocate frames\n");
        pcm_close(pcm);
        return nullptr;
    }


    while (context->running) {
        int read_count = pcm_readi(pcm, frames, frames_per_sec);
        if (read_count > 0) {
            size_t byte_count = pcm_frames_to_bytes(pcm, read_count);
            if (context->callback) {
                context->callback(frames, byte_count, (long) context);
            }
        } else {
            fprintf(stderr, "failed to read frames\n");
            break;
        }
    }

    free(frames);
    pcm_close(pcm);
//
//    while (context->running) {
//        if (pcm_read(context->pcm, buffer, size) == 0) {
//            if (context->callback) {
//                context->callback(buffer, size, (long) context);
//            }
//        } else {
//            fprintf(stderr, "PCM read error\n");
//        }
//    }
//
//    free(buffer);
    return NULL;
}

struct PcmReaderContext *pcm_reader_init(
        int pcm_card,
        int pcm_device,
        int channels,
        int sample_rate,
        int period_size,
        int period_count,
        int format,
        int start_threshold,
        int stop_threshold,
        int silence_threshold,
        PcmDataCallback callback) {
    struct PcmReaderContext *context = reinterpret_cast<PcmReaderContext *>(malloc(
            sizeof(struct PcmReaderContext)));
    if (!context) return NULL;

    struct pcm_config config;
    memset(&config, 0, sizeof(config));
    config.channels = channels; // 双声道
    config.rate = sample_rate; // 采样率 48000Hz
    config.period_size = period_size; // 周期大小
    config.period_count = period_count; // 周期计数
    if (format == 16) {
        config.format = PCM_FORMAT_S16_LE; // 16位小端序格式
    } else if (format == 32) {
        config.format = PCM_FORMAT_S32_LE; // 32位小端序格式
    }
    config.start_threshold = start_threshold;
//    config.stop_threshold = stop_threshold;
//    config.silence_threshold = silence_threshold;


//    config.channels = 2; // 双声道
//    config.rate = 48000; // 采样率 48000Hz
//    config.period_size = 512; // 周期大小
//    config.period_count = 2; // 周期计数
//    config.format = PCM_FORMAT_S16_LE; // 16位小端序格式
//    config.start_threshold = 1;

    context->pcm = pcm_open(pcm_card, pcm_device, PCM_IN, &config);
    if (!context->pcm || !pcm_is_ready(context->pcm)) {
        __android_log_print(ANDROID_LOG_DEBUG, "pcm_reader",
                            "card:%d,device:%d,Unable to open PCM device (%s)",
                            pcm_card, pcm_device, pcm_get_error(context->pcm));
        fprintf(stderr, "Unable to open PCM device (%s)\n", pcm_get_error(context->pcm));
        free(context);
        return NULL;
    }

    context->callback = callback;
    context->running = 1;

    if (pthread_create(&context->thread, NULL, pcm_read_thread, context) != 0) {
        pcm_close(context->pcm);
        free(context);
        return NULL;
    }

    return context;
}


int pcm_reader_destroy(struct PcmReaderContext *context) {
    if (!context) return -1;
    int ret = 0;
    context->running = 0;
    pthread_join(context->thread, NULL);

//    ret = pcm_close(context->pcm);
    free(context);

    return ret;
}