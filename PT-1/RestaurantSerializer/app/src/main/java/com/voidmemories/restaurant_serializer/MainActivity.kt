package com.voidmemories.restaurant_serializer

import android.os.Bundle
import androidx.activity.ComponentActivity
import java.time.DayOfWeek
import java.time.LocalTime

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        main()
    }

    private fun main() {
        val restaurant = Restaurant(
            id = "rest-123",
            name = "Sample Diner",
            address = Address(
                street = "123 Main St",
                city = "SomeCity",
                state = "CA",
                zipCode = "98765",
                country = "USA"
            ),
            rating = 4.5,
            cuisines = listOf("American", "Fast Food"),
            phoneNumber = "555-1234",
            website = "www.samplediner.com",
            openingHours = listOf(
                OpeningHour(DayOfWeek.MONDAY, LocalTime.of(8, 0), LocalTime.of(20, 0)),
                OpeningHour(DayOfWeek.TUESDAY, LocalTime.of(8, 0), LocalTime.of(20, 0))
            ),
            menu = listOf(
                MenuItem("menu-1", "Burger", "Tasty beef burger", 5.99, "Main"),
                MenuItem("menu-2", "Fries", "Crispy fries", 2.49, "Side")
            )
        )

        val externalFunctions = ExternalFunctions()
        val jsonResult = externalFunctions.serializeRestaurant(restaurant)
        println("Restaurant JSON: $jsonResult")
    }
}