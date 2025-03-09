package com.voidmemories.maintainable_jni.kotlin

import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import okhttp3.MediaType.Companion.toMediaType
import okhttp3.OkHttpClient
import okhttp3.Request
import okhttp3.RequestBody.Companion.toRequestBody
import java.io.IOException

class Networking {

    private val client = OkHttpClient()

    /**
     * Executes a GET request to the provided URL.
     *
     * @param url The URL to request.
     * @return The response body as a String.
     * @throws IOException if the request is not successful or the response body is null.
     */
    suspend fun getRequest(url: String): String = withContext(Dispatchers.IO) {
        val request = Request.Builder()
            .url(url)
            .get()
            .build()

        client.newCall(request).execute().use { response ->
            if (!response.isSuccessful) {
                throw IOException("Unexpected code $response")
            }
            response.body?.string() ?: throw IOException("Response body is null")
        }
    }

    /**
     * Executes a POST request with a JSON body to the provided URL.
     *
     * @param url The URL to post to.
     * @param jsonBody The JSON body to send.
     * @return The response body as a String.
     * @throws IOException if the request is not successful or the response body is null.
     */
    suspend fun postRequest(url: String, jsonBody: String): String = withContext(Dispatchers.IO) {
        val mediaType = "application/json; charset=utf-8".toMediaType()
        val body = jsonBody.toRequestBody(mediaType)
        val request = Request.Builder()
            .url(url)
            .post(body)
            .build()

        client.newCall(request).execute().use { response ->
            if (!response.isSuccessful) {
                throw IOException("Unexpected code $response")
            }
            response.body?.string() ?: throw IOException("Response body is null")
        }
    }
}
