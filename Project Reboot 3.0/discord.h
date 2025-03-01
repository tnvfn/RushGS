#pragma once

#define CURL_STATICLIB

#include <iostream>
#include <string>
#include <curl/curl.h>
#include <format>

class DiscordWebhook {
public:
    DiscordWebhook(const char* webhook_url)
    {
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, webhook_url);

            curl_slist* headers = curl_slist_append(NULL, "Content-Type: application/json");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        }
        else {
            std::cerr << "Error: curl_easy_init() returned NULL pointer" << '\n';
        }
    }

    ~DiscordWebhook()
    {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }

    bool handleCode(CURLcode res)
    {
        if (res != CURLE_OK) {
            std::cerr << "Curl Error: " << curl_easy_strerror(res) << std::endl;
        }
        return res == CURLE_OK;
    }

    inline bool send_embed(const std::string& title, const std::string& description, int color = 65280)
    {
        std::string json = std::format(R"({{
            "embeds": [{{
                "title": "{}",
                "description": "{}",
                "color": {}
            }}]
        }})", title, description, color);

        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());

        return handleCode(curl_easy_perform(curl));
    }

private:
    CURL* curl;
};

namespace Information
{
    static std::string UptimeWebHook = "Ur webhook here!";
}

static DiscordWebhook UptimeWebHook(Information::UptimeWebHook.c_str());
