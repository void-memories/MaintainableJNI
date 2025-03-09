#ifndef JNI_STRING_HELPER_H
#define JNI_STRING_HELPER_H

#include <jni.h>
#include <stdexcept>

/**
 * @class JNIString
 * @brief A helper class that manages the lifetime of a jstring->C string mapping.
 *
 * Usage:
 *   JNIString jniString(env, someJString);
 *   const char* cStr = jniString.c_str();
 *   // cStr remains valid until jniString goes out of scope
 */
class JNIString {
public:
    /**
     * Constructs a JNIString from a jstring, acquiring its UTF-8 chars.
     * @param env Pointer to the JNI environment.
     * @param javaStr jstring to convert to a C-style string.
     * @throws std::runtime_error if env or javaStr is null.
     */
    JNIString(JNIEnv* env, jstring javaStr)
            : env_(env), javaStr_(javaStr), cStr_(nullptr)
    {
        if (!env_ || !javaStr_) {
            throw std::runtime_error("JNIString constructor: env or javaStr is null");
        }
        cStr_ = env_->GetStringUTFChars(javaStr_, nullptr);
    }

    /**
     * Releases the UTF-8 chars back to the JVM if they were acquired.
     */
    ~JNIString() {
        if (env_ && javaStr_ && cStr_) {
            env_->ReleaseStringUTFChars(javaStr_, cStr_);
        }
    }

    /**
     * @return A const char* representing the UTF-8 string data.
     */
    const char* c_str() const {
        return cStr_;
    }

private:
    JNIEnv* env_;
    jstring javaStr_;
    const char* cStr_;
};

#endif // JNI_STRING_HELPER_H
