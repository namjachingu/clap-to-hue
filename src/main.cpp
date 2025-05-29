#include "../include/CurlClient.hpp"
#include "../include/Lights.hpp"

#include <fmt/core.h>

int main() {
    const std::optional<std::string> & certificate_path = "<path to self-signed root certificate>";
    const std::string & host = "<host>";
    const std::string & port = "<port>";
    const std::string & ip = "<ip>";
    const std::string & api_key = "<api_key>";
    CurlClient curl_client(api_key, certificate_path, host, port, ip);

    curl_client.getRequest(fmt::format("https://{}/api/{}/lights", host, api_key));
    json response = curl_client.getResponse();

    Lights lights(curl_client);
    lights.turnLightsOff(response);

    return 0;
}