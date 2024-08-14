#include <jni.h>
#include "pcm_reader.h"

#define UNUSED(x) (void)(x)

static JavaVM *jvm = nullptr;
static jobject javaCallback = nullptr;

void pcm_data_callback(const void *buffer, size_t size) {
    JNIEnv *env;
    jint res = jvm->AttachCurrentThread(&env, nullptr);
    if (res != JNI_OK) {
        return;
    }

    jbyteArray data = env->NewByteArray(size);
    env->SetByteArrayRegion(data, 0, size, reinterpret_cast<const jbyte *>(buffer));

    jclass cls = env->GetObjectClass(javaCallback);
    jmethodID methodID = env->GetMethodID(cls, "onPcmData", "([B)V");
    env->CallVoidMethod(javaCallback, methodID, data);

    env->DeleteLocalRef(data);
    jvm->DetachCurrentThread();
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_jackstudio_tinyalas4android_PcmReader_nativeInit(JNIEnv *env, jobject obj) {
    env->GetJavaVM(&jvm);
    javaCallback = env->NewGlobalRef(obj);
    return reinterpret_cast<jlong>(pcm_reader_init(pcm_data_callback));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_jackstudio_tinyalas4android_PcmReader_nativeDestroy(JNIEnv *env, jobject thiz,
                                                             jlong handler) {
    pcm_reader_destroy(reinterpret_cast<struct PcmReaderContext *>(handler));
    env->DeleteGlobalRef(javaCallback);
}