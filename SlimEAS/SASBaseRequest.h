//
//  SASBaseRequest.h
//  SlimEAS
//
//  Created by envy.chen on 12/17/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <curl/curl.h>

#include "SASHTTPResponse.h"

namespace SlimEAS {
  class SASBaseRequest {
  protected:
    CURL *_curl;
    
    virtual void requestBegin();
    virtual void requestEnd();
    
  public:
    SASBaseRequest();
    SASBaseRequest(const std::string& server,
                   const std::string& user,
                   const std::string& password,
                   bool useSSL = true);
    virtual ~SASBaseRequest();
    
    SAS_PROPERTY_PROTECTED(std::string, server);
    SAS_PROPERTY_PROTECTED(std::string, userName);
    SAS_PROPERTY_PROTECTED(std::string, password);
    SAS_PROPERTY_PROTECTED(bool, useSSL);
    
    SAS_PROPERTY(std::string, requestBody);
    
    virtual SASHTTPResponse *getResponse() = 0;
  };
}
