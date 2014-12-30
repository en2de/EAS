//
//  SASHTTPResponse.h
//  SlimEAS
//
//  Created by envy.chen on 12/16/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <map>
#include <string>

#include "SASDefine.h"

#include "SASHTTPRequest.h"

namespace SlimEAS {
  class SASHTTPResponse {
  private:
    SlimEAS::SASHTTPResponseHeader _headers;
    
  public:
    SASHTTPResponse(SlimEAS::SASHTTPRequest::SASHTTPResponseContext &ctx);
    virtual ~SASHTTPResponse();
    
    SASHTTPResponseHeader &header() {
      return _headers;
    }
    
    std::string headerString();
  };
}
