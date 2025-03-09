cmake_minimum_required(VERSION 3.10)

# Name of the library that will be loaded via System.loadLibrary("restaurant-lib")
project("restaurant-lib")

add_library(restaurant-lib
        SHARED
        core/RestaurantNative.cpp

        jni/shadowClasses/AddressShadow.h
        jni/shadowClasses/MenuItemShadow.h
        jni/shadowClasses/OpeningHourShadow.h
        jni/shadowClasses/RestaurantShadow.h

        jni/jniString.h
        jni/jni.cpp
)

# Link libraries, if needed:
find_library( # variable name
        log-lib
        log)

target_link_libraries(
        restaurant-lib
        ${log-lib}
)
