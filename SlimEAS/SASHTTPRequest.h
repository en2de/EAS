//
//  SASHTTPRequest.h
//  SlimEAS
//
//  Created by envy.chen on 12/17/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <curl/curl.h>
#include <map>
#include <string>

#include "SASDefine.h"

#define SASRequestSetOptions(opt,args...)  curl_easy_setopt(_curl, opt, ##args);

namespace SlimEAS {
  
  class SASHTTPResponse;
  
  typedef std::map<std::string, std::string> SASHTTPResponseHeader;

  class SASHTTPRequest {
  public:
    typedef struct SASHTTPResponseContext {
      uint8_t *buf;
      size_t buf_len;
      size_t use;
      std::map<std::string, std::string> headers;
    } SASHTTPResponseContext;
    
  private:
    //handle response data
    struct SASHTTPResponseContext _resContext;
    
  protected:
    CURL *_curl;
    
    //override this func to provide custom response extended from SASHTTPResponse
    virtual SASHTTPResponse *initialResponse();
    
    //perform request.
    SASHTTPResponse *perform();
  public:
    
    SASHTTPRequest();
    SASHTTPRequest(const std::string& server,
                   const std::string& user,
                   const std::string& password,
                   bool useSSL = true);
    virtual ~SASHTTPRequest();
    
    SAS_PROPERTY_PROTECTED(std::string, server);
    SAS_PROPERTY_PROTECTED(std::string, user);
    SAS_PROPERTY_PROTECTED(std::string, password);
    SAS_PROPERTY_PROTECTED(bool, useSSL);
    
    SAS_PROPERTY(std::string, requestBody);
    
    //You must implement this func.
    //setup custom request option.
    //then call this->perform()
    virtual SASHTTPResponse *getResponse() = 0;
  };
}
