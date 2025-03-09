#ifndef ANDROID_SDK_ADDRESSSHADOW_H
#define ANDROID_SDK_ADDRESSSHADOW_H

#include <jni.h>
#include <stdexcept>
#include <string>
#include "../JNIString.h"

// Forward declare your global JVM from somewhere in your project
extern JavaVM* globalJvm;

class AddressShadow {
private:
    inline  static jclass addressClass;
    inline static jmethodID getStreetMethodId;
    inline static jmethodID getCityMethodId;
    inline static jmethodID getStateMethodId;
    inline static jmethodID getZipCodeMethodId;
    inline static jmethodID getCountryMethodId;

    jobject addressObject;

public:
    static bool init(JNIEnv* env) {
        if (!env) return false;

        // Find the Address class
        jclass localClass = env->FindClass("com/voidmemories/restaurant_serializer/Address");
        if (!localClass) {
            return false;
        }
        addressClass = static_cast<jclass>(env->NewGlobalRef(localClass));
        env->DeleteLocalRef(localClass);
        if (!addressClass) {
            return false;
        }

        // Get method IDs for the generated getters in Address data class
        getStreetMethodId   = env->GetMethodID(addressClass, "getStreet", "()Ljava/lang/String;");
        getCityMethodId     = env->GetMethodID(addressClass, "getCity", "()Ljava/lang/String;");
        getStateMethodId    = env->GetMethodID(addressClass, "getState", "()Ljava/lang/String;");
        getZipCodeMethodId  = env->GetMethodID(addressClass, "getZipCode", "()Ljava/lang/String;");
        getCountryMethodId  = env->GetMethodID(addressClass, "getCountry", "()Ljava/lang/String;");

        if (!getStreetMethodId || !getCityMethodId || !getStateMethodId ||
            !getZipCodeMethodId || !getCountryMethodId) {
            return false;
        }

        return true;
    }

    AddressShadow(JNIEnv* env, jobject obj) {
        if (!env || !obj) {
            throw std::runtime_error("Invalid constructor arguments for AddressShadow.");
        }
        addressObject = env->NewGlobalRef(obj);
    }

    ~AddressShadow() {
        JNIEnv* env;
        int getEnvStatus = globalJvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
        if (addressObject && getEnvStatus != JNI_EDETACHED && env != nullptr) {
            env->DeleteGlobalRef(addressObject);
        }
    }

    // Example: retrieve the street as a std::string
    std::string getStreet(JNIEnv* env) {
        if (!env || !addressObject || !getStreetMethodId) {
            throw std::runtime_error("Invalid state to call getStreet.");
        }
        jstring jStreet = (jstring)env->CallObjectMethod(addressObject, getStreetMethodId);
        if (!jStreet) {
            return std::string(); // or handle null appropriately
        }
        JNIString street(env, jStreet);
        return std::string(street.c_str());
    }

    std::string getCity(JNIEnv* env) {
        if (!env || !addressObject || !getCityMethodId) {
            throw std::runtime_error("Invalid state to call getCity.");
        }
        jstring jCity = (jstring)env->CallObjectMethod(addressObject, getCityMethodId);
        if (!jCity) {
            return std::string();
        }
        JNIString city(env, jCity);
        return std::string(city.c_str());
    }

    std::string getState(JNIEnv* env) {
        if (!env || !addressObject || !getStateMethodId) {
            throw std::runtime_error("Invalid state to call getState.");
        }
        jstring jState = (jstring)env->CallObjectMethod(addressObject, getStateMethodId);
        if (!jState) {
            return std::string();
        }
        JNIString state(env, jState);
        return std::string(state.c_str());
    }

    std::string getZipCode(JNIEnv* env) {
        if (!env || !addressObject || !getZipCodeMethodId) {
            throw std::runtime_error("Invalid state to call getZipCode.");
        }
        jstring jZip = (jstring)env->CallObjectMethod(addressObject, getZipCodeMethodId);
        if (!jZip) {
            return std::string();
        }
        JNIString zip(env, jZip);
        return std::string(zip.c_str());
    }

    std::string getCountry(JNIEnv* env) {
        if (!env || !addressObject || !getCountryMethodId) {
            throw std::runtime_error("Invalid state to call getCountry.");
        }
        jstring jCountry = (jstring)env->CallObjectMethod(addressObject, getCountryMethodId);
        if (!jCountry) {
            return std::string();
        }
        JNIString country(env, jCountry);
        return std::string(country.c_str());
    }
};

#endif // ANDROID_SDK_ADDRESSSHADOW_H
