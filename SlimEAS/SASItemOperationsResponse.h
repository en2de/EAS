//
//  SASItemOperationsResponse.h
//  SlimEAS
//
//  Created by Focus Lan on 1/6/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>

#include "SASCommandResponse.h"
#include "SASMail.h"

namespace SlimEAS {
  
  struct FetchResponse {
    int status;
    std::string collectionId;
    std::string serverId;
    std::string className;
  };
  
  class SASItemOperationsResponse : public SASCommandResponse {
    
  private:
    SASMail         _mail;
    FetchResponse   _fetchResponse;
    int32_t         _status;
  
#pragma constructor
  public:
    SASItemOperationsResponse(SASHTTPRequest::SASHTTPResponseContext &ctx);
    ~SASItemOperationsResponse();

#pragma getter/setter
  public:
    const std::string     &xmlResponse() {return _xmlResponse;}
    const SASMail         &mail() {return _mail;}
    const FetchResponse   &fetchResponse() {return _fetchResponse;}
  };
}
