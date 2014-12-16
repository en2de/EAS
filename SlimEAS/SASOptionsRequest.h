//
//  SASOptionsRequest.h
//  SlimEAS
//
//  Created by envy.chen on 12/15/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <string>
#include <curl/curl.h>

#include "SASDefine.h"
namespace SlimEAS {
  
  typedef struct SASOptionsResponse {
    std::string supportedCommand;
    std::string supportedVersions;
    std::string highestSupportedVersion;
  } SASOptionsResponse;
  
  class SASOptionsRequest {
  private:
    CURL *_curl;
    
  public:
    SAS_PROPERTY(std::string, server);
    SAS_PROPERTY(bool, useSSL);
    SAS_PROPERTY(std::string, userName);
    SAS_PROPERTY(std::string, password);
    
    SASOptionsRequest();
    ~SASOptionsRequest();
    
    struct SASOptionsResponse getReponse();
  };
}
