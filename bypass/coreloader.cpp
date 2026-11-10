
#include <jni.h>
#include <android/log.h>
#include <pthread.h>
#include <unistd.h>
#include <atomic>

#define TAG "CoreLoader"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)

static JavaVM* g_vm = nullptr;
static std::atomic<bool> initialized(false);

static void initialize_coreloader() {
    if (initialized.exchange(true))
        return;

    LOGI("CoreLoader native initialization started");

    // Initialize authorized mod-loader components here.

    LOGI("CoreLoader initialization complete");
}

static void* initialization_thread(void*) {
    LOGI("Starting CoreLoader background thread");

    usleep(250000);

    initialize_coreloader();

    return nullptr;
}

extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM* vm, void*) {
    g_vm = vm;

    LOGI("CoreLoader JNI_OnLoad");

    pthread_t thread;

    if (pthread_create(
        &thread,
        nullptr,
        initialization_thread,
        nullptr
    ) == 0) {
        pthread_detach(thread);
    } else {
        LOGE("Failed to create initialization thread");
    }

    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_coreloader_MainActivity_initialize(
    JNIEnv* env,
    jobject instance
) {
    (void)env;
    (void)instance;

    initialize_coreloader();
}
