#ifndef ANDROID_SDK_OPENINGHOURSHADOW_H
#define ANDROID_SDK_OPENINGHOURSHADOW_H

#include <jni.h>
#include <stdexcept>
#include <string>
#include "../JNIString.h"

extern JavaVM* globalJvm;

/**
 * Shadow for:
 * data class OpeningHour(
 *    val dayOfWeek: DayOfWeek,
 *    val openTime: LocalTime,
 *    val closeTime: LocalTime
 * )
 *
 * The example below retrieves the dayOfWeek as an enum object, and uses toString().
 * For times, we do the same (LocalTime -> toString()) approach.
 * Adapt as needed, e.g., to call getHour()/getMinute(), etc.
 */
class OpeningHourShadow {
private:
   inline static jclass openingHourClass;
    inline static jmethodID getDayOfWeekMethodId;
    inline static jmethodID getOpenTimeMethodId;
    inline static jmethodID getCloseTimeMethodId;

    // We'll need these to call toString() on DayOfWeek or LocalTime
    inline static jmethodID toStringMethodId;

    jobject openingHourObject;

public:
    static bool init(JNIEnv* env) {
        if (!env) return false;

        // OpeningHour class
        jclass localClass = env->FindClass("com/voidmemories/restaurant_serializer/OpeningHour");
        if (!localClass) {
            return false;
        }
        openingHourClass = static_cast<jclass>(env->NewGlobalRef(localClass));
        env->DeleteLocalRef(localClass);
        if (!openingHourClass) {
            return false;
        }

        getDayOfWeekMethodId = env->GetMethodID(openingHourClass, "getDayOfWeek", "()Ljava/time/DayOfWeek;");
        getOpenTimeMethodId  = env->GetMethodID(openingHourClass, "getOpenTime", "()Ljava/time/LocalTime;");
        getCloseTimeMethodId = env->GetMethodID(openingHourClass, "getCloseTime", "()Ljava/time/LocalTime;");

        if (!getDayOfWeekMethodId || !getOpenTimeMethodId || !getCloseTimeMethodId) {
            return false;
        }

        // For dayOfWeek or LocalTime objects, we can call .toString() to retrieve a jstring
        // We'll get that method from java.lang.Object or specifically from the class itself
        // since DayOfWeek and LocalTime override toString()
        jclass objectClass = env->FindClass("java/lang/Object");
        toStringMethodId   = env->GetMethodID(objectClass, "toString", "()Ljava/lang/String;");
        if (!toStringMethodId) {
            return false;
        }

        return true;
    }

    OpeningHourShadow(JNIEnv* env, jobject obj) {
        if (!env || !obj) {
            throw std::runtime_error("Invalid constructor arguments for OpeningHourShadow.");
        }
        openingHourObject = env->NewGlobalRef(obj);
    }

    ~OpeningHourShadow() {
        JNIEnv* env;
        int getEnvStatus = globalJvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
        if (openingHourObject && getEnvStatus != JNI_EDETACHED && env != nullptr) {
            env->DeleteGlobalRef(openingHourObject);
        }
    }

    // Retrieve the dayOfWeek as a string (e.g. "MONDAY", "TUESDAY", etc.)
    std::string getDayOfWeek(JNIEnv* env) {
        if (!env || !openingHourObject || !getDayOfWeekMethodId) {
            throw std::runtime_error("Invalid state to call getDayOfWeek.");
        }
        jobject dayOfWeekObj = env->CallObjectMethod(openingHourObject, getDayOfWeekMethodId);
        if (!dayOfWeekObj) {
            return std::string();
        }
        jstring dayOfWeekStr = (jstring)env->CallObjectMethod(dayOfWeekObj, toStringMethodId);
        if (!dayOfWeekStr) {
            return std::string();
        }
        JNIString jniStr(env, dayOfWeekStr);
        return std::string(jniStr.c_str());
    }

    // Retrieve openTime as something like "10:00" using toString()
    std::string getOpenTime(JNIEnv* env) {
        if (!env || !openingHourObject || !getOpenTimeMethodId) {
            throw std::runtime_error("Invalid state to call getOpenTime.");
        }
        jobject openTimeObj = env->CallObjectMethod(openingHourObject, getOpenTimeMethodId);
        if (!openTimeObj) {
            return std::string();
        }
        jstring timeStr = (jstring)env->CallObjectMethod(openTimeObj, toStringMethodId);
        if (!timeStr) {
            return std::string();
        }
        JNIString jniStr(env, timeStr);
        return std::string(jniStr.c_str());
    }

    // Retrieve closeTime as e.g. "22:00"
    std::string getCloseTime(JNIEnv* env) {
        if (!env || !openingHourObject || !getCloseTimeMethodId) {
            throw std::runtime_error("Invalid state to call getCloseTime.");
        }
        jobject closeTimeObj = env->CallObjectMethod(openingHourObject, getCloseTimeMethodId);
        if (!closeTimeObj) {
            return std::string();
        }
        jstring timeStr = (jstring)env->CallObjectMethod(closeTimeObj, toStringMethodId);
        if (!timeStr) {
            return std::string();
        }
        JNIString jniStr(env, timeStr);
        return std::string(jniStr.c_str());
    }
};

#endif // ANDROID_SDK_OPENINGHOURSHADOW_H
