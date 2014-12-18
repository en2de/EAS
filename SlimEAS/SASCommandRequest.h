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
    std::string *getXml(uint8_t* data, int data_len);
    
    void setCommandParam(std::string &param, std::string &value){
      _commandParams[param] = value;
    }
    
    virtual std::string getRequestLine();
    
    virtual SASHTTPResponse *getResponse();
  };
}