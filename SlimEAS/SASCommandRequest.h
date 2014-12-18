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
#include "SASBaseRequest.h"


namespace SlimEAS {
  
  class SASCommandRequest : public SASBaseRequest {
    SAS_PROPERTY(std::string, protocolVersion);
    SAS_PROPERTY(std::string, command);
    SAS_PROPERTY(std::string, deviceID);
    SAS_PROPERTY(std::string, deviceType);
    SAS_PROPERTY(bool, useEncodeRequestLine);
    SAS_PROPERTY(uint32_t, policyKey);
    
  private:
    std::map<std::string, std::string> _commandParams;
    
  public:
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