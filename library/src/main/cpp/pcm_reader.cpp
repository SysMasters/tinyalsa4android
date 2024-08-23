#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <tinyalsa/asoundlib.h>
#include <android/log.h>
#include <string.h>
#include "pcm_reader.h"


void *pcm_read_thread(void *arg) {
    struct PcmReaderContext *context = (struct PcmReaderContext *) arg;
    size_t size = pcm_frames_to_bytes(context->pcm, pcm_get_buffer_size(context->pcm));
    void *buffer = malloc(size);

    while (context->running) {
        if (pcm_read(context->pcm, buffer, size) == 0) {
            if (context->callback) {
                context->callback(buffer, size, (long) context);
            }
        } else {
            fprintf(stderr, "PCM read error\n");
        }
    }

    free(buffer);
    return NULL;
}

struct PcmReaderContext *pcm_reader_init(
        int pcm_card, int pcm_device, pcm_config config, PcmDataCallback callback) {
    struct PcmReaderContext *context = reinterpret_cast<PcmReaderContext *>(malloc(
            sizeof(struct PcmReaderContext)));
    if (!context) return NULL;

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

    ret = pcm_close(context->pcm);
    free(context);

    return ret;
}