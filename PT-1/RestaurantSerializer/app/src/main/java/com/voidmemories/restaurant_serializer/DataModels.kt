package com.voidmemories.restaurant_serializer

import java.time.DayOfWeek
import java.time.LocalTime

data class Restaurant(
    val id: String,
    val name: String,
    val address: Address,
    val rating: Double,
    val cuisines: List<String>,
    val phoneNumber: String?,
    val website: String?,
    val openingHours: List<OpeningHour>,
    val menu: List<MenuItem>
)

data class Address(
    val street: String,
    val city: String,
    val state: String,
    val zipCode: String,
    val country: String
)

data class OpeningHour(
    val dayOfWeek: DayOfWeek,
    val openTime: LocalTime,
    val closeTime: LocalTime
)

data class MenuItem(
    val id: String,
    val name: String,
    val description: String?,
    val price: Double,
    val category: String?
)
