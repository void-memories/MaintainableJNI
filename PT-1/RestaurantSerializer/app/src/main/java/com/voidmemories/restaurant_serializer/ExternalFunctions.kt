package com.voidmemories.restaurant_serializer

class ExternalFunctions {
    init {
        System.loadLibrary("restaurant-lib")
    }

    external fun serializeRestaurant(restaurant: Restaurant):String
}