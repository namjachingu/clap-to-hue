#ifndef CURL_CLIENT_HPP
#define CURL_CLIENT_HPP

#include <string>
#include <curl/curl.h>
#include "json.hpp"

#include <optional>

using json = nlohmann::json;

class CurlClient {
public:
    CurlClient(
        const std::string & api_key,
        const std::optional<std::string> & certificate_path,
        const std::string & host,
        const std::string & port,
        const std::string & ip);
    ~CurlClient();

    const std::string getHost();
    const std::string getApiKey();

    void getRequest(const std::string & url);
    json getResponse();
    void putRequest(const std::string & url, const std::string & body);

private:
    void parse_response(const std::string & response);
    static size_t writeResponseData(void* contents, size_t size, size_t nmemb, std::string* userp);

    const std::string api_key_;
    const std::string host_;
    const std::string port_;
    const std::string ip_;
    CURL* curl_;
    json response_;
};

#endif // CURL_CLIENT_HPP
