#include "json.hpp"
#include "CurlClient.hpp"

#include <string>
#include <vector>

using json = nlohmann::json;

class Lights {

public:
    Lights(CurlClient & curl_client);

    void setLights(const json & response);
    void turnLightsOff(const json & response);

private:
    std::vector<std::string> lights_on_;
    CurlClient curl_client_;
};