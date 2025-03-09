#include "../jni/shadowClasses/RestaurantShadow.h"
#include "../jni/shadowClasses/AddressShadow.h"
#include "../jni/jniString.h"
#include "../jni/shadowClasses/OpeningHourShadow.h"
#include "../jni/shadowClasses/MenuItemShadow.h"

#include <jni.h>
#include <string>
#include <sstream>

/**
 * Utility to get size of a java.util.List
 */
static int getListSize(JNIEnv* env, jobject listObj) {
    if (!listObj) return 0;
    jclass listClass = env->FindClass("java/util/List");
    jmethodID sizeMethod = env->GetMethodID(listClass, "size", "()I");
    return env->CallIntMethod(listObj, sizeMethod);
}

/**
 * Utility to retrieve an element from a java.util.List by index
 */
static jobject getListElement(JNIEnv* env, jobject listObj, int index) {
    jclass listClass = env->FindClass("java/util/List");
    jmethodID getMethod = env->GetMethodID(listClass, "get", "(I)Ljava/lang/Object;");
    return env->CallObjectMethod(listObj, getMethod, index);
}

/**
 * Build a simple JSON from the RestaurantShadow's fields.
 * In real projects, you'd likely use a JSON library (cJSON, nlohmann/json, RapidJSON, etc.).
 */
std::string buildJsonFromRestaurant(JNIEnv* env, RestaurantShadow& restShadow) {
    // Basic fields
    std::string id = restShadow.getId(env);
    std::string name = restShadow.getName(env);
    double rating = restShadow.getRating(env);
    std::string phone = restShadow.getPhoneNumber(env);
    std::string website = restShadow.getWebsite(env);

    // Address
    jobject addressObj = restShadow.getAddress(env);
    AddressShadow addrShadow(env, addressObj);

    // Cuisines
    jobject cuisinesList = restShadow.getCuisines(env);
    int cuisinesCount = getListSize(env, cuisinesList);

    // OpeningHours
    jobject openHoursList = restShadow.getOpeningHours(env);
    int openHoursCount = getListSize(env, openHoursList);

    // Menu
    jobject menuList = restShadow.getMenu(env);
    int menuCount = getListSize(env, menuList);

    // Manual JSON building
    std::ostringstream oss;
    oss << "{";
    oss << R"("id":")" << id << "\",";
    oss << R"("name":")" << name << "\",";
    oss << "\"rating\":" << rating << ",";
    oss << R"("phoneNumber":")" << phone << "\",";
    oss << R"("website":")" << website << "\",";

    // Address
    oss << "\"address\":{";
    oss << R"("street":")" << addrShadow.getStreet(env) << "\",";
    oss << R"("city":")" << addrShadow.getCity(env) << "\",";
    oss << R"("state":")" << addrShadow.getState(env) << "\",";
    oss << R"("zipCode":")" << addrShadow.getZipCode(env) << "\",";
    oss << R"("country":")" << addrShadow.getCountry(env) << "\"";
    oss << "},";

    // Cuisines array
    oss << "\"cuisines\":[";
    for (int i = 0; i < cuisinesCount; i++) {
        jobject elem = getListElement(env, cuisinesList, i);
        auto jStr = (jstring)elem; // Because it's List<String>
        if (!jStr) continue;
        JNIString cStr(env, jStr);
        oss << "\"" << cStr.c_str() << "\"";
        if (i < cuisinesCount - 1) {
            oss << ",";
        }
    }
    oss << "],";

    // OpeningHours array
    oss << "\"openingHours\":[";
    for (int i = 0; i < openHoursCount; i++) {
        jobject elem = getListElement(env, openHoursList, i);
        OpeningHourShadow ohShadow(env, elem);

        oss << "{";
        oss << R"("dayOfWeek":")" << ohShadow.getDayOfWeek(env) << "\",";
        oss << R"("openTime":")" << ohShadow.getOpenTime(env) << "\",";
        oss << R"("closeTime":")" << ohShadow.getCloseTime(env) << "\"";
        oss << "}";
        if (i < openHoursCount - 1) {
            oss << ",";
        }
    }
    oss << "],";

    // Menu array
    oss << "\"menu\":[";
    for (int i = 0; i < menuCount; i++) {
        jobject elem = getListElement(env, menuList, i);
        MenuItemShadow miShadow(env, elem);

        oss << "{";
        oss << R"("id":")" << miShadow.getId(env) << "\",";
        oss << R"("name":")" << miShadow.getName(env) << "\",";
        oss << R"("description":")" << miShadow.getDescription(env) << "\",";
        oss << "\"price\":" << miShadow.getPrice(env) << ",";
        oss << R"("category":")" << miShadow.getCategory(env) << "\"";
        oss << "}";
        if (i < menuCount - 1) {
            oss << ",";
        }
    }
    oss << "]";

    oss << "}"; // end JSON object

    return oss.str();
}
