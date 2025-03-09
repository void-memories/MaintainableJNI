#include <jni.h>
#include <string>

#include "shadowClasses/RestaurantShadow.h"
#include "shadowClasses/AddressShadow.h"
#include "shadowClasses/MenuItemShadow.h"
#include "shadowClasses/OpeningHourShadow.h"

extern std::string buildJsonFromRestaurant(JNIEnv *env, RestaurantShadow &restShadow);

JavaVM *globalJvm = nullptr;

// Kotlin Function declaration (without Java_ prefix)
jstring serializeRestaurant(JNIEnv *env, jobject thiz, jobject jRestaurant) {
    RestaurantShadow restShadow(env, jRestaurant);
    std::string json = buildJsonFromRestaurant(env, restShadow);
    return env->NewStringUTF(json.c_str());
}

// Init functions, called only once!!!
static const JNINativeMethod nativeMethods[] = {
        {"serializeRestaurant", "(Lcom/voidmemories/restaurant_serializer/Restaurant;)Ljava/lang/String;",
         (void *)serializeRestaurant}
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void * /*reserved*/) {
    globalJvm = vm;

    JNIEnv *env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK || !env) {
        return -1;
    }

    AddressShadow::init(env);
    MenuItemShadow::init(env);
    OpeningHourShadow::init(env);
    RestaurantShadow::init(env);

    jclass clazz = env->FindClass("com/voidmemories/restaurant_serializer/ExternalFunctions");
    if (!clazz) {
        return -1;
    }

    if (env->RegisterNatives(clazz, nativeMethods, sizeof(nativeMethods) / sizeof(nativeMethods[0])) != JNI_OK) {
        return -1;
    }

    return JNI_VERSION_1_6;
}
