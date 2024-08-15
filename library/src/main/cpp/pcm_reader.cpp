#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <tinyalsa/asoundlib.h>
#include "pcm_reader.h"


void *pcm_read_thread(void *arg) {
    struct PcmReaderContext *context = (struct PcmReaderContext *) arg;
    size_t size = pcm_frames_to_bytes(context->pcm, pcm_get_buffer_size(context->pcm));
    void *buffer = malloc(size);

    while (context->running) {
        if (pcm_read(context->pcm, buffer, size) == 0) {
            if (context->callback) {
                context->callback(buffer, size);
            }
        } else {
            fprintf(stderr, "PCM read error\n");
        }
    }

    free(buffer);
    return NULL;
}

struct PcmReaderContext *pcm_reader_init(int pcm_card, int pcm_device, PcmDataCallback callback) {
    struct PcmReaderContext *context = reinterpret_cast<PcmReaderContext *>(malloc(
            sizeof(struct PcmReaderContext)));
    if (!context) return NULL;

    struct pcm_config config;
    config.channels = 2;// 立体声
    config.rate = 48000;
    config.format = PCM_FORMAT_S32_LE;
    config.period_size = 1024;
    config.period_count = 4;
//    config.start_threshold = 1024;
//    config.silence_threshold = 1024 * 2;
//    config.stop_threshold = 1024 * 2;

    context->pcm = pcm_open(pcm_card, pcm_device, PCM_IN, &config);
    if (!context->pcm || !pcm_is_ready(context->pcm)) {
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


void pcm_reader_destroy(struct PcmReaderContext *context) {
    if (!context) return;

    context->running = 0;
    pthread_join(context->thread, NULL);

    pcm_close(context->pcm);
    free(context);
}