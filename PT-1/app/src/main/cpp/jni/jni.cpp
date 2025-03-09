#include <jni.h>
#include <string>

#include "shadowClasses/RestaurantShadow.h"
#include "shadowClasses/AddressShadow.h"
#include "shadowClasses/MenuItemShadow.h"
#include "shadowClasses/OpeningHourShadow.h"

extern std::string buildJsonFromRestaurant(JNIEnv *env, RestaurantShadow &restShadow);

JavaVM *globalJvm = nullptr;

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void * /*reserved*/) {
    globalJvm = vm;

    JNIEnv *env = nullptr;
    if (vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK || !env) {
        return -1;
    }

    // Initialize shadows
    AddressShadow::init(env);
    MenuItemShadow::init(env);
    OpeningHourShadow::init(env);
    RestaurantShadow::init(env);

    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_voidmemories_restaurant_1serializer_ExternalFunctions_serializeRestaurant(JNIEnv *env,
                                                                                   jobject thiz,
                                                                                   jobject jRestaurant) {
    RestaurantShadow restShadow(env, jRestaurant);
    std::string json = buildJsonFromRestaurant(env, restShadow);
    return env->NewStringUTF(json.c_str());
}