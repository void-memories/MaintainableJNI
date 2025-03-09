#ifndef ANDROID_SDK_RESTAURANTSHADOW_H
#define ANDROID_SDK_RESTAURANTSHADOW_H

#include <jni.h>
#include <string>
#include "../jniString.h"

extern JavaVM* globalJvm;

/**
 * data class Restaurant(
 *    val id: String,
 *    val name: String,
 *    val address: Address,
 *    val rating: Double,
 *    val cuisines: List<String>,
 *    val phoneNumber: String?,
 *    val website: String?,
 *    val openingHours: List<OpeningHour>,
 *    val menu: List<MenuItem>
 * )
 */
class RestaurantShadow {
private:
    inline static jclass restaurantClass;

    inline  static jmethodID getIdMethodId;
    inline  static jmethodID getNameMethodId;
    inline static jmethodID getAddressMethodId;
    inline static jmethodID getRatingMethodId;
    inline static jmethodID getCuisinesMethodId;
    inline  static jmethodID getPhoneNumberMethodId;
    inline static jmethodID getWebsiteMethodId;
    inline static jmethodID getOpeningHoursMethodId;
    inline static jmethodID getMenuMethodId;

    jobject restaurantObject;

public:
    static bool init(JNIEnv* env) {
        if (!env) return false;

        jclass localClass = env->FindClass("com/voidmemories/restaurant_serializer/Restaurant");
        if (!localClass) {
            return false;
        }
        restaurantClass = static_cast<jclass>(env->NewGlobalRef(localClass));
        env->DeleteLocalRef(localClass);
        if (!restaurantClass) {
            return false;
        }

        // Get method IDs for all getters
        getIdMethodId           = env->GetMethodID(restaurantClass, "getId", "()Ljava/lang/String;");
        getNameMethodId         = env->GetMethodID(restaurantClass, "getName", "()Ljava/lang/String;");
        getAddressMethodId      = env->GetMethodID(restaurantClass, "getAddress", "()Lcom/voidmemories/restaurant_serializer/Address;");
        getRatingMethodId       = env->GetMethodID(restaurantClass, "getRating", "()D");
        getCuisinesMethodId     = env->GetMethodID(restaurantClass, "getCuisines", "()Ljava/util/List;");
        getPhoneNumberMethodId  = env->GetMethodID(restaurantClass, "getPhoneNumber", "()Ljava/lang/String;");
        getWebsiteMethodId      = env->GetMethodID(restaurantClass, "getWebsite", "()Ljava/lang/String;");
        getOpeningHoursMethodId = env->GetMethodID(restaurantClass, "getOpeningHours", "()Ljava/util/List;");
        getMenuMethodId         = env->GetMethodID(restaurantClass, "getMenu", "()Ljava/util/List;");

        if (!getIdMethodId || !getNameMethodId || !getAddressMethodId ||
            !getRatingMethodId || !getCuisinesMethodId || !getPhoneNumberMethodId ||
            !getWebsiteMethodId || !getOpeningHoursMethodId || !getMenuMethodId) {
            return false;
        }

        return true;
    }

    RestaurantShadow(JNIEnv* env, jobject obj) {
        if (!env || !obj) {
            throw std::runtime_error("Invalid constructor arguments for RestaurantShadow.");
        }
        restaurantObject = env->NewGlobalRef(obj);
    }

    ~RestaurantShadow() {
        JNIEnv* env;
        int getEnvStatus = globalJvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);
        if (restaurantObject && getEnvStatus != JNI_EDETACHED && env != nullptr) {
            env->DeleteGlobalRef(restaurantObject);
        }
    }

    std::string getId(JNIEnv* env) {
        if (!env || !restaurantObject || !getIdMethodId) {
            throw std::runtime_error("Invalid state to call getId.");
        }
        jstring jId = (jstring)env->CallObjectMethod(restaurantObject, getIdMethodId);
        if (!jId) {
            return std::string();
        }
        JNIString idStr(env, jId);
        return std::string(idStr.c_str());
    }

    std::string getName(JNIEnv* env) {
        if (!env || !restaurantObject || !getNameMethodId) {
            throw std::runtime_error("Invalid state to call getName.");
        }
        jstring jName = (jstring)env->CallObjectMethod(restaurantObject, getNameMethodId);
        if (!jName) {
            return std::string();
        }
        JNIString nameStr(env, jName);
        return std::string(nameStr.c_str());
    }

    // Returns a jobject for the Address. The caller can then wrap it in AddressShadow if desired.
    jobject getAddress(JNIEnv* env) {
        if (!env || !restaurantObject || !getAddressMethodId) {
            throw std::runtime_error("Invalid state to call getAddress.");
        }
        return env->CallObjectMethod(restaurantObject, getAddressMethodId);
    }

    double getRating(JNIEnv* env) {
        if (!env || !restaurantObject || !getRatingMethodId) {
            throw std::runtime_error("Invalid state to call getRating.");
        }
        return env->CallDoubleMethod(restaurantObject, getRatingMethodId);
    }

    // Returns a jobject reference to a Java List<String> of cuisines
    jobject getCuisines(JNIEnv* env) {
        if (!env || !restaurantObject || !getCuisinesMethodId) {
            throw std::runtime_error("Invalid state to call getCuisines.");
        }
        return env->CallObjectMethod(restaurantObject, getCuisinesMethodId);
    }

    std::string getPhoneNumber(JNIEnv* env) {
        if (!env || !restaurantObject || !getPhoneNumberMethodId) {
            throw std::runtime_error("Invalid state to call getPhoneNumber.");
        }
        jstring jPhone = (jstring)env->CallObjectMethod(restaurantObject, getPhoneNumberMethodId);
        if (!jPhone) {
            return std::string();
        }
        JNIString phoneStr(env, jPhone);
        return std::string(phoneStr.c_str());
    }

    std::string getWebsite(JNIEnv* env) {
        if (!env || !restaurantObject || !getWebsiteMethodId) {
            throw std::runtime_error("Invalid state to call getWebsite.");
        }
        jstring jWebsite = (jstring)env->CallObjectMethod(restaurantObject, getWebsiteMethodId);
        if (!jWebsite) {
            return std::string();
        }
        JNIString websiteStr(env, jWebsite);
        return std::string(websiteStr.c_str());
    }

    // Returns a jobject reference to Java List<OpeningHour>
    jobject getOpeningHours(JNIEnv* env) {
        if (!env || !restaurantObject || !getOpeningHoursMethodId) {
            throw std::runtime_error("Invalid state to call getOpeningHours.");
        }
        return env->CallObjectMethod(restaurantObject, getOpeningHoursMethodId);
    }

    // Returns a jobject reference to Java List<MenuItem>
    jobject getMenu(JNIEnv* env) {
        if (!env || !restaurantObject || !getMenuMethodId) {
            throw std::runtime_error("Invalid state to call getMenu.");
        }
        return env->CallObjectMethod(restaurantObject, getMenuMethodId);
    }
};

#endif // ANDROID_SDK_RESTAURANTSHADOW_H
