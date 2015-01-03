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
#include "SASHTTPRequest.h"

namespace SlimEAS {
  
  typedef struct SASOptionsResponse {
    std::string supportedCommand;
    std::string supportedVersions;
    std::string highestSupportedVersion;
  } SASOptionsResponse;
  
  class SASOptionsRequest : public SASHTTPRequest {
  public:
    SASOptionsRequest();
    ~SASOptionsRequest();
    
    struct SASOptionsResponse getReponse();
    
    SASHTTPResponse *getResponse();
  };
}
