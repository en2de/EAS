//
//  SASHTTPResponse.h
//  SlimEAS
//
//  Created by envy.chen on 12/16/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <sstream>
#include <map>
#include <string>
#include <curl/curl.h>
#include "SASDefine.h"


namespace SlimEAS {
  
  typedef size_t (*CurlCallback)(char *data, size_t size, size_t nmemb, void *puser);
  typedef std::map<std::string, std::string> SASHTTPResponseHeader;
  
  class SASHTTPResponse {
  private:
    std::ostringstream *_ostream;
    SASHTTPResponseHeader *_headers;
    
  public:
    SASHTTPResponse();
    ~SASHTTPResponse();
    
    SAS_PROPERTY(std::string, version);
    SAS_PROPERTY(std::string, statusCode);
    SAS_PROPERTY(std::string, status);
    
    SAS_PROPERTY(std::string, headerString);
    SAS_PROPERTY(std::string, body);
    
    const std::ostringstream * writeStream() const {
      return _ostream;
    }
    
    SASHTTPResponseHeader * const headers() const {
      return _headers;
    }
    
    void addHeader(const std::string& line);
    
    const std::string& setHeader(const std::string &name, const std::string &val) const {
      return (*_headers)[name] = val;
    }

    const std::string &getHeader(const std::string &name) const {
      return (*_headers)[name];
    }
    
    //debug
    static int debugCallback(CURL *handle, curl_infotype type, char *data, size_t size, void *puser);
    
    CurlCallback writeHandler();
    CurlCallback headerHandler();
  };
}
