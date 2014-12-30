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
  public:
    
  private:
    std::map<std::string, std::string> _commandParams;
    
  protected:
    std::string _xmlPayload;
    
    //override to generate the xml payload based on
    //the XML payload based on the command's request schema.
    virtual void generateXMLPayload();
    
    std::string _protocolVersion;
    std::string _command;
    std::string _deviceId;
    std::string _deviceType;
    bool _useEncodeRequestLine;
    uint32_t _policyKey;
    
  public:
#pragma mark - constructor
    
    SASCommandRequest();
    virtual ~SASCommandRequest();
    
#pragma mark - properties
    
    std::string &protocolVersion() {return _protocolVersion;}
    void setProtocolVersion(const std::string &version) {
      _protocolVersion = version;
    }
    
    std::string &command() {return _command;}
    void setCommand(const std::string &value) {
      _command = value;
    }
    
    std::string &deviceId() {return _deviceId;}
    void setDeviceId(const std::string &value) {
      _deviceId = value;
    }
    
    std::string &deviceType() {return _deviceType;}
    void setDeviceType(const std::string &value) {
      _deviceType = value;
    }
    
    bool useEncodeRequestLine() {return _useEncodeRequestLine;}
    void setUseEncodeRequestLine(bool value) {
      _useEncodeRequestLine = value;
    }
    
    uint32_t policyKey() {return _policyKey;}
    void setUseEncodeRequestLine(uint32_t value) {
      _policyKey = value;
    }
    
#pragma mark member func
    
    const std::string &XMLPayload() { return _xmlPayload;};
    void setXMLPayload(const std::string &xml) {
      _xmlPayload = xml;
    }
    
    void setCommandParam(std::string &param, std::string &value){
      _commandParams[param] = value;
    }
    
    virtual std::string getRequestLine();
    
    virtual SASHTTPResponse *getResponse();
  };
}