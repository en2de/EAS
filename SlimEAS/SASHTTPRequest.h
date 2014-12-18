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

#include "SASHTTPResponse.h"

#define SASRequestSetOptions(opt,args...)  curl_easy_setopt(_curl, opt, ##args);


namespace SlimEAS {
  class SASHTTPRequest {
  protected:
    CURL *_curl;
        
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
    
    virtual SASHTTPResponse *getResponse() = 0;
    
    SASHTTPResponse *perform();
  };
}
