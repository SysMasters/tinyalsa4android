#include <jni.h>
#include "pcm_reader.h"

static JavaVM *jvm = nullptr;
static jobject javaCallback = nullptr;

void pcm_data_callback(const void *buffer, size_t size) {
    JNIEnv *env;
    jint res = jvm->AttachCurrentThread(&env, nullptr);
    if (res != JNI_OK) {
        return;
    }

    jbyteArray data = env->NewByteArray(size);
    env->SetByteArrayRegion(data, 0, size, (jbyte *) buffer);

    jclass cls = env->GetObjectClass(javaCallback);
    jmethodID methodID = env->GetMethodID(cls, "onPcmData", "([B)V");
    env->CallVoidMethod(javaCallback, methodID, data);

    env->DeleteLocalRef(data);
    jvm->DetachCurrentThread();
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_tinyalas4android_library_PcmReader_nativeInit(JNIEnv *env, jobject obj, jint pcm_card,
                                                       jint pcm_device) {
    env->GetJavaVM(&jvm);
    javaCallback = env->NewGlobalRef(obj);
    return (long) pcm_reader_init(pcm_card, pcm_device, pcm_data_callback);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_tinyalas4android_library_PcmReader_nativeDestroy(JNIEnv *env, jobject thiz,
                                                          jlong handler) {
    pcm_reader_destroy((struct PcmReaderContext *) handler);
    env->DeleteGlobalRef(javaCallback);
}