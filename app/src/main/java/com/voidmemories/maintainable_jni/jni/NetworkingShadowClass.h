#ifndef ANDROID_SDK_NETWORKINGSHADOW_H
#define ANDROID_SDK_NETWORKINGSHADOW_H

#include <jni.h>
#include <stdexcept>
#include <string>
#include "../utils/jniLogger.h"
#include "JNIString.h"  // Include our JNIString helper here

extern JavaVM* globalJvm;

/**
 * Helper function to detect and clear any pending Java exceptions.
 * Logs the exception (if present) and returns true if an exception was found.
 */
inline bool checkAndClearJavaException(JNIEnv* env, const char* contextDescription) {
    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        LOGE("Java exception occurred in: %s", contextDescription);
        return true;
    }
    return false;
}

class NetworkingShadow {
private:
    // Cached global reference to the Networking class.
    static jclass networkingClass;

    // Cached method IDs.
    static jmethodID constructorId;
    static jmethodID getRequestMethodId;
    static jmethodID postRequestMethodId;

    // Global reference to the actual Networking object instance.
    jobject networkingObject;

public:
    /**
     * Initialize references to the Kotlin Networking class and its methods.
     * Call this once (e.g., on library load or JNI_OnLoad).
     */
    static bool init(JNIEnv* env) {
        if (env == nullptr) return false;

        // 1. Locate the Kotlin class: com.voidmemories.maintainable_jni.kotlin.Networking
        jclass localClass = env->FindClass("com/voidmemories/maintainable_jni/kotlin/Networking");
        if (!localClass) {
            LOGE("Class com.voidmemories.maintainable_jni.kotlin.Networking not found.");
            checkAndClearJavaException(env, "NetworkingShadow::init - FindClass");
            return false;
        }

        // 2. Create a global reference for later use.
        networkingClass = reinterpret_cast<jclass>(env->NewGlobalRef(localClass));
        env->DeleteLocalRef(localClass);
        if (!networkingClass) {
            LOGE("Failed to create global reference for Networking class.");
            return false;
        }

        // 3. Get the constructor ID (assumes a no-argument constructor).
        constructorId = env->GetMethodID(networkingClass, "<init>", "()V");
        if (!constructorId) {
            LOGE("No default constructor found for Networking.");
            checkAndClearJavaException(env, "NetworkingShadow::init - GetMethodID <init>");
            return false;
        }

        // 4. Get method IDs
        getRequestMethodId = env->GetMethodID(networkingClass,
                                              "getRequest",
                                              "(Ljava/lang/String;)Ljava/lang/String;");
        if (!getRequestMethodId) {
            LOGE("getRequest(String) not found or not accessible.");
            checkAndClearJavaException(env, "NetworkingShadow::init - GetMethodID getRequest");
            return false;
        }

        postRequestMethodId = env->GetMethodID(networkingClass,
                                               "postRequest",
                                               "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
        if (!postRequestMethodId) {
            LOGE("postRequest(String, String) not found or not accessible.");
            checkAndClearJavaException(env, "NetworkingShadow::init - GetMethodID postRequest");
            return false;
        }

        return true;
    }

    /**
     * Constructs a new Kotlin Networking object and stores a global reference.
     */
    NetworkingShadow(JNIEnv* env) : networkingObject(nullptr) {
        if (!env) {
            throw std::runtime_error("NetworkingShadow constructor: env is null");
        }
        if (!networkingClass || !constructorId) {
            throw std::runtime_error("NetworkingShadow constructor: Networking class not initialized");
        }

        // Instantiate a new Networking object in Kotlin.
        jobject localObj = env->NewObject(networkingClass, constructorId);
        if (checkAndClearJavaException(env, "NetworkingShadow::constructor - NewObject")) {
            throw std::runtime_error("Java exception thrown while creating Networking object.");
        }

        if (!localObj) {
            throw std::runtime_error("Failed to create Networking object.");
        }

        // Store it as a global reference so we can call methods on it later.
        networkingObject = env->NewGlobalRef(localObj);
        env->DeleteLocalRef(localObj);
    }

    /**
     * Destroys the global reference to the Networking object.
     */
    ~NetworkingShadow() {
        if (!networkingObject) return;

        // Obtain JNIEnv for the current thread.
        JNIEnv* env = nullptr;
        int getEnvStatus = globalJvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);

        // If we're attached, delete the global ref.
        if (env && getEnvStatus != JNI_EDETACHED) {
            env->DeleteGlobalRef(networkingObject);
        }
    }

    /**
     * Calls the getRequest(String) method on the Networking object (Kotlin).
     * Returns a jstring with the response or NULL on error.
     */
    jstring getRequest(JNIEnv* env, const char* url) {
        if (!env || !networkingObject || !getRequestMethodId) {
            throw std::runtime_error("Invalid state to call getRequest.");
        }
        if (!url) {
            throw std::runtime_error("URL parameter is null in getRequest.");
        }

        // Convert the C-string to a jstring before passing it to Kotlin.
        jstring jUrl = env->NewStringUTF(url);
        if (!jUrl) {
            throw std::runtime_error("Failed to create jstring for URL in getRequest.");
        }

        // Invoke the method on the Kotlin Networking object.
        jstring result = (jstring)env->CallObjectMethod(networkingObject, getRequestMethodId, jUrl);
        env->DeleteLocalRef(jUrl);

        // Check for Java exceptions.
        if (checkAndClearJavaException(env, "NetworkingShadow::getRequest - CallObjectMethod")) {
            return nullptr;
        }

        return result;
    }

    /**
     * Calls the postRequest(String, String) method on the Networking object (Kotlin).
     * Returns a jstring with the response or NULL on error.
     */
    jstring postRequest(JNIEnv* env, const char* url, const char* jsonBody) {
        if (!env || !networkingObject || !postRequestMethodId) {
            throw std::runtime_error("Invalid state to call postRequest.");
        }
        if (!url || !jsonBody) {
            throw std::runtime_error("URL or JSON body is null in postRequest.");
        }

        // Convert the C-strings to jstrings.
        jstring jUrl = env->NewStringUTF(url);
        jstring jJson = env->NewStringUTF(jsonBody);

        if (!jUrl || !jJson) {
            if (jUrl) env->DeleteLocalRef(jUrl);
            if (jJson) env->DeleteLocalRef(jJson);
            throw std::runtime_error("Failed to create jstring for URL or JSON in postRequest.");
        }

        // Invoke the method on the Kotlin Networking object.
        jstring result = (jstring)env->CallObjectMethod(networkingObject,
                                                        postRequestMethodId,
                                                        jUrl,
                                                        jJson);
        env->DeleteLocalRef(jUrl);
        env->DeleteLocalRef(jJson);

        // Check for Java exceptions.
        if (checkAndClearJavaException(env, "NetworkingShadow::postRequest - CallObjectMethod")) {
            return nullptr;
        }

        return result;
    }

    /**
     * Example of how to convert the returned jstring into a C++ std::string
     * using the JNIString helper class.
     */
    std::string getRequestAsStdString(JNIEnv* env, const char* url) {
        jstring jResult = getRequest(env, url);
        if (!jResult) {
            // Possibly an error or Java exception was thrown
            return std::string();
        }

        // Use JNIString to manage the conversion from jstring to const char*
        JNIString resultStr(env, jResult);
        // Optionally delete local ref if you’re done with jResult
        env->DeleteLocalRef(jResult);

        // Construct a std::string from the raw C string
        return std::string(resultStr.c_str());
    }
};

// Initialize static members
inline jclass NetworkingShadow::networkingClass = nullptr;
inline jmethodID NetworkingShadow::constructorId = nullptr;
inline jmethodID NetworkingShadow::getRequestMethodId = nullptr;
inline jmethodID NetworkingShadow::postRequestMethodId = nullptr;

#endif // ANDROID_SDK_NETWORKINGSHADOW_H
