//
//  SASProvisionResponse.h
//  SlimEAS
//
//  Created by envy.chen on 12/19/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>

#include "SASCommandResponse.h"
#include "SASPolicy.h"

namespace SlimEAS {
  class SASProvisionResponse: public SASCommandResponse {
  public:
    typedef enum SASProvisionStatus {
      Provision_Success = 1,
      Provision_SyntaxError = 2,
      Provision_ServerError = 3,
      Provision_DeviceNotFullyProvisionable = 139,
      Provision_LegacyDeviceOnStrictPolicy = 141,
      Provision_ExternallyManagedDevicesNotAllowed = 145
    } SASProvisionRequest;
    
  private:
    bool _isPolicyLoaded = false;
    int32_t _status = 0;
    SASPolicy _policy;
    
  public:
    SASProvisionResponse(SASHTTPRequest::SASHTTPResponseContext &ctx);
    ~SASProvisionResponse();
  
    bool isPolicyLoad() {return _isPolicyLoaded;}
//    SASPolicy &policy() {return _policy;}
    int32_t status() {return _status;}
    const SASPolicy &policy() {return _policy;}
  };
}
