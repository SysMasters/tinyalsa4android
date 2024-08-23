#include <jni.h>
#include<map>
#include "pcm_reader.h"
#include "tinyalsa/pcm.h"

JavaVM *jvm = nullptr;
std::map<jlong, jobject> javaCallbacks;

void pcm_data_callback(const void *buffer, size_t size, long handler) {
    JNIEnv *env;
    jint res = jvm->AttachCurrentThread(&env, nullptr);
    if (res != JNI_OK) {
        return;
    }

    jbyteArray data = env->NewByteArray(size);
    env->SetByteArrayRegion(data, 0, size, (jbyte *) buffer);

    jobject javaCallback = javaCallbacks[handler];
    if (javaCallback != nullptr) {
        jclass cls = env->GetObjectClass(javaCallback);
        jmethodID methodID = env->GetMethodID(cls, "onPcmData", "([B)V");
        env->CallVoidMethod(javaCallback, methodID, data);
    }

    env->DeleteLocalRef(data);
    jvm->DetachCurrentThread();
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_tinyalas4android_library_PcmReader_nativeInit(JNIEnv *env, jobject obj, jint pcm_card,
                                                       jint pcm_device, jobject pcmConfig) {
    env->GetJavaVM(&jvm);
    jclass cls = env->GetObjectClass(pcmConfig);
    jmethodID getChannels = env->GetMethodID(cls, "getChannels", "()I");
    jmethodID getRate = env->GetMethodID(cls, "getRate", "()I");
    jmethodID getPeriodSize = env->GetMethodID(cls, "getPeriodSize", "()I");
    jmethodID getPeriodCount = env->GetMethodID(cls, "getPeriodCount", "()I");
    jmethodID getFormat = env->GetMethodID(cls, "getFormat", "()I");
    jmethodID getStartThreshold = env->GetMethodID(cls, "getStartThreshold", "()I");
    jmethodID getStopThreshold = env->GetMethodID(cls, "getStopThreshold", "()I");
    jmethodID getSilenceThreshold = env->GetMethodID(cls, "getSilenceThreshold", "()I");
    jint channels = env->CallIntMethod(pcmConfig, getChannels);
    jint rate = env->CallIntMethod(pcmConfig, getRate);
    jint periodSize = env->CallIntMethod(pcmConfig, getPeriodSize);
    jint periodCount = env->CallIntMethod(pcmConfig, getPeriodCount);
    jint format = env->CallIntMethod(pcmConfig, getFormat);
    jint startThreshold = env->CallIntMethod(pcmConfig, getStartThreshold);
    jint stopThreshold = env->CallIntMethod(pcmConfig, getStopThreshold);
    jint silenceThreshold = env->CallIntMethod(pcmConfig, getSilenceThreshold);

    long handler = (long) pcm_reader_init(pcm_card, pcm_device, channels, rate, periodSize,
                                          periodCount, format, startThreshold, stopThreshold,
                                          silenceThreshold, pcm_data_callback);
    javaCallbacks[handler] = env->NewGlobalRef(obj);
    return handler;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_tinyalas4android_library_PcmReader_nativeDestroy(JNIEnv *env, jobject thiz,
                                                          jlong handler) {
    if (handler == -1) {
        return -1;
    }
    int ret = 0;
    ret = pcm_reader_destroy((struct PcmReaderContext *) handler);
    jobject javaCallback = javaCallbacks[handler];
    if (javaCallback != nullptr) {
        env->DeleteGlobalRef(javaCallback);
    }
    javaCallbacks.erase(handler);

    return ret;
}