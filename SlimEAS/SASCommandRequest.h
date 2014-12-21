//
//  SASCommandRequest.h
//  SlimEAS
//
//  Created by envy.chen on 12/17/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <string>
#include <map>

#include "SASDefine.h"
#include "SASHTTPRequest.h"

namespace SlimEAS {
  
  class SASCommandRequest : public SASHTTPRequest {
  private:
    std::map<std::string, std::string> _commandParams;
    
  public:
    SAS_PROPERTY_PROTECTED(std::string, protocolVersion);
    SAS_PROPERTY_PROTECTED(std::string, command);
    SAS_PROPERTY_PROTECTED(std::string, deviceID);
    SAS_PROPERTY_PROTECTED(std::string, deviceType);
    SAS_PROPERTY_PROTECTED(bool, useEncodeRequestLine);
    SAS_PROPERTY_PROTECTED(uint32_t, policyKey);
    
    SASCommandRequest();
    SASCommandRequest(const std::string& server,
                      const std::string& user,
                      const std::string& password,
                      bool useSSL = true);
    ~SASCommandRequest();
    
    uint8_t * getWBXml(unsigned int *olen);
    
    void setCommandParam(std::string &param, std::string &value){
      _commandParams[param] = value;
    }
    
    virtual std::string getRequestLine();
    
    virtual SASHTTPResponse *getResponse();
  };
}