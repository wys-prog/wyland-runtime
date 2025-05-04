#pragma once

#include <curl/curl.h>
#include <string>
#include <fstream>
#include <stdexcept>

#include "wylrt.hpp"
#include "../wmmbase.hpp"

WYLAND_BEGIN

namespace wnet {
  class CurlException : public runtime::wyland_runtime_error {
  public:
    CurlException(const std::string &what, const std::string &from) 
      : runtime::wyland_runtime_error(
          what.c_str(), "curl exception", from.c_str(), 
          typeid(this).name(), 0, 0, nullptr, nullptr, 0
        )
    {}
  };

  class HttpClient {
  public:
    HttpClient() {
      curl = curl_easy_init();
      if (!curl) throw CurlException("Unable to initialize curl", memberofstr);
    }
  
    ~HttpClient() {
      if (curl) curl_easy_cleanup(curl);
    }
    
    
    bool DownloadToFile(const std::string& url, const std::string& filePath) {
      std::ofstream outFile(filePath, std::ios::binary);
      if (!outFile)
        return false;
  
      SetCommonOptions(url);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outFile);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpClient::WriteToFile);
      CURLcode res = curl_easy_perform(curl);
  
      return res == CURLE_OK;
    }
    
    
    std::string DownloadToString(const std::string& url) {
      response.clear();
  
      SetCommonOptions(url);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpClient::WriteToString);
  
      CURLcode res = curl_easy_perform(curl);
      if (res != CURLE_OK)
        throw CurlException(curl_easy_strerror(res), memberofstr);
      return response;
    }
    
  private:
    CURL *curl;
    std::string response;
  
    void SetCommonOptions(const std::string& url) {
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
      curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    }
  
    static size_t WriteToFile(void* ptr, size_t size, size_t nmemb, void* stream) {
      std::ofstream *out = static_cast<std::ofstream*>(stream);
      out->write(static_cast<char*>(ptr), size * nmemb);
      return size * nmemb;
    }
  
    static size_t WriteToString(void* ptr, size_t size, size_t nmemb, void* userdata) {
      HttpClient *self = static_cast<HttpClient*>(userdata);
      self->response.append(static_cast<char*>(ptr), size * nmemb);
      return size * nmemb;
    }
  };
}

WYLAND_END