#include "../include/Lights.hpp"

#include <fmt/core.h>
#include <iostream>

Lights::Lights(CurlClient & curl_client)
    : curl_client_(curl_client)
{};

void Lights::setLights(const json & response)
{
    for (json::const_iterator it = response.begin(); it != response.end(); ++it) {
        if ((*it)["state"]["on"] == true) {
            std::cout << "Light " << it.key() << " is on" << std::endl;
            lights_on_.push_back(it.key());
        } else {
            std::cout << "Lights are off" << std::endl;
        }
    }
}

void Lights::turnLightsOff(const json & response)
{
    setLights(response);
    if (lights_on_.empty()) {
        std::cout << "No lights are on." << std::endl;
        return;
    }

    std::string body = "{\"on\": false}";
    std::string base_url = fmt::format("https://{}/api/{}/lights/", curl_client_.getHost(), curl_client_.getApiKey());
    for (const auto & light : lights_on_) {
        curl_client_.putRequest(base_url + light + "/state", body);
    }
}
