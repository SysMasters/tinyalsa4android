#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <tinyalsa/asoundlib.h>

#define PCM_DEVICE 0
#define PCM_CARD 0
#define PCM_FORMAT PCM_FORMAT_S16_LE
#define PCM_CHANNELS 2
#define PCM_RATE 44100

typedef void (*PcmDataCallback)(const void *buffer, size_t size);

struct PcmReaderContext {
    struct pcm *pcm;
    pthread_t thread;
    int running;
    PcmDataCallback callback;
};

void *pcm_read_thread(void *arg) {
    struct PcmReaderContext *context = (struct PcmReaderContext *)arg;
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

struct PcmReaderContext *pcm_reader_init(PcmDataCallback callback) {
    struct PcmReaderContext *context = static_cast<PcmReaderContext *>(malloc(
            sizeof(struct PcmReaderContext)));
    if (!context) return NULL;

    struct pcm_config config;
    config.channels = PCM_CHANNELS;
    config.rate = PCM_RATE;
    config.format = PCM_FORMAT;
    config.period_size = 1024;
    config.period_count = 4;

    context->pcm = pcm_open(PCM_CARD, PCM_DEVICE, PCM_IN, &config);
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