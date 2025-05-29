#include "../include/CurlClient.hpp"
#include <fmt/core.h>

#include <iostream>

CurlClient::CurlClient(
    const std::string & api_key,
    const std::optional<std::string> & certificate_path,
    const std::string & host,
    const std::string & port,
    const std::string & ip)
    : api_key_(api_key)
    , host_(host)
    , port_(port)
    , ip_(ip)
{
    CURL* curl = curl_easy_init();
    curl_ = curl;
    std::cout << "Running curl version: " << curl_version_info(CURLVERSION_NOW)->version << std::endl;

    if (certificate_path.has_value()) {
        curl_easy_setopt(curl_, CURLOPT_CAINFO, certificate_path.value().c_str());
    }

    struct curl_slist* resolve_list = nullptr;
    resolve_list = curl_slist_append(resolve_list, fmt::format("{}:{}:{}", host_, port_, ip_).c_str());
    if (!resolve_list) {
        std::cerr << "Failed to allocate memory for resolve list" << std::endl;
        curl_easy_cleanup(curl_);
        curl_ = nullptr;
        return;
    }

    curl_easy_setopt(curl_, CURLOPT_RESOLVE, resolve_list);
    curl_slist_free_all(resolve_list);
}

CurlClient::~CurlClient() {
    if (curl_) {
        curl_easy_cleanup(curl_);
    }
}

const std::string CurlClient::getHost() {
    return host_;
}

const std::string CurlClient::getApiKey() {
    return api_key_;
}

size_t CurlClient::writeResponseData(void* contents, size_t size, size_t nmemb, std::string* userp) {
    size_t totalSize = size * nmemb;
    userp->append((char*)contents, totalSize);
    return totalSize;
}

void CurlClient::parse_response(const std::string & response)
{
    try {
        response_ = json::parse(response);
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
    }
}

void CurlClient::getRequest(const std::string & url) {
    if (!curl_) {
        std::cerr << "Failed to initialize libcurl." << std::endl;
    }
    curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());

    struct curl_slist* resolve_list = nullptr;
    resolve_list = curl_slist_append(resolve_list, fmt::format("{}:{}:{}", host_, port_, ip_).c_str());
    if (!resolve_list) {
        std::cerr << "Failed to allocate memory for resolve list" << std::endl;
        curl_easy_cleanup(curl_);
        return;
    }
    curl_easy_setopt(curl_, CURLOPT_RESOLVE, resolve_list);

    // curl_easy_setopt(curl_, CURLOPT_VERBOSE, 1L);

    std::string read_buffer;
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, CurlClient::writeResponseData);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &read_buffer);

    CURLcode res = curl_easy_perform(curl_);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    } else {
        parse_response(read_buffer);
    }

    curl_slist_free_all(resolve_list);
}

json CurlClient::getResponse()
{
    return response_;
}

void CurlClient::putRequest(const std::string & url, const std::string & body)
{
    if (!curl_) {
        std::cerr << "Failed to initialize libcurl." << std::endl;
        return;
    }

    curl_easy_setopt(curl_, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "PUT");
    curl_easy_setopt(curl_, CURLOPT_POSTFIELDS, body.c_str());

    std::string read_buffer;
    curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, CurlClient::writeResponseData);
    curl_easy_setopt(curl_, CURLOPT_WRITEDATA, &read_buffer);

    CURLcode res = curl_easy_perform(curl_);
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    } else {
        std::cout << "PUT request response: " << read_buffer << std::endl;
    }
}
