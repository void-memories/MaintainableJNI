#ifndef ANDROID_SDK_MENUITEMSHADOW_H
#define ANDROID_SDK_MENUITEMSHADOW_H

#include <jni.h>
#include <string>
#include "../JNIString.h"

extern JavaVM* globalJvm;

class MenuItemShadow {
private:
    inline  static jclass menuItemClass;
    inline static jmethodID getIdMethodId;
    inline static jmethodID getNameMethodId;
    inline static jmethodID getDescriptionMethodId;
    inline  static jmethodID getPriceMethodId;
    inline static jmethodID getCategoryMethodId;

    jobject menuItemObject;

public:
    static bool init(JNIEnv* env) {
        if (!env) return false;

        jclass localClass = env->FindClass("com/voidmemories/restaurant_serializer/MenuItem");
        if (!localClass) {
            return false;
        }
        menuItemClass = static_cast<jclass>(env->NewGlobalRef(localClass));
        env->DeleteLocalRef(localClass);
        if (!menuItemClass) {
            return false;
        }

        getIdMethodId          = env->GetMethodID(menuItemClass, "getId", "()Ljava/lang/String;");
        getNameMethodId        = env->GetMethodID(menuItemClass, "getName", "()Ljava/lang/String;");
        getDescriptionMethodId = env->GetMethodID(menuItemClass, "getDescription", "()Ljava/lang/String;");
        getPriceMethodId       = env->GetMethodID(menuItemClass, "getPrice", "()D");
        getCategoryMethodId    = env->GetMethodID(menuItemClass, "getCategory", "()Ljava/lang/String;");

        if (!getIdMethodId || !getNameMethodId || !getDescriptionMethodId ||
            !getPriceMethodId || !getCategoryMethodId) {
            return false;
        }

        return true;
    }

    MenuItemShadow(JNIEnv* env, jobject obj) {
        if (!env || !obj) {
            throw std::runtime_error("Invalid constructor arguments for MenuItemShadow.");
        }
        menuItemObject = env->NewGlobalRef(obj);
    }

    ~MenuItemShadow() {
        JNIEnv* env;
        int getEnvStatus = globalJvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
        if (menuItemObject && getEnvStatus != JNI_EDETACHED && env != nullptr) {
            env->DeleteGlobalRef(menuItemObject);
        }
    }

    std::string getId(JNIEnv* env) {
        if (!env || !menuItemObject || !getIdMethodId) {
            throw std::runtime_error("Invalid state to call getId.");
        }
        jstring jId = (jstring)env->CallObjectMethod(menuItemObject, getIdMethodId);
        if (!jId) {
            return std::string();
        }
        JNIString idStr(env, jId);
        return std::string(idStr.c_str());
    }

    std::string getName(JNIEnv* env) {
        if (!env || !menuItemObject || !getNameMethodId) {
            throw std::runtime_error("Invalid state to call getName.");
        }
        jstring jName = (jstring)env->CallObjectMethod(menuItemObject, getNameMethodId);
        if (!jName) {
            return std::string();
        }
        JNIString nameStr(env, jName);
        return std::string(nameStr.c_str());
    }

    std::string getDescription(JNIEnv* env) {
        if (!env || !menuItemObject || !getDescriptionMethodId) {
            throw std::runtime_error("Invalid state to call getDescription.");
        }
        jstring jDesc = (jstring)env->CallObjectMethod(menuItemObject, getDescriptionMethodId);
        if (!jDesc) {
            return std::string();
        }
        JNIString descStr(env, jDesc);
        return std::string(descStr.c_str());
    }

    double getPrice(JNIEnv* env) {
        if (!env || !menuItemObject || !getPriceMethodId) {
            throw std::runtime_error("Invalid state to call getPrice.");
        }
        return env->CallDoubleMethod(menuItemObject, getPriceMethodId);
    }

    std::string getCategory(JNIEnv* env) {
        if (!env || !menuItemObject || !getCategoryMethodId) {
            throw std::runtime_error("Invalid state to call getCategory.");
        }
        jstring jCat = (jstring)env->CallObjectMethod(menuItemObject, getCategoryMethodId);
        if (!jCat) {
            return std::string();
        }
        JNIString catStr(env, jCat);
        return std::string(catStr.c_str());
    }
};

#endif // ANDROID_SDK_MENUITEMSHADOW_H
