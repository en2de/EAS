//
//  SASEncodedRequest.h
//  SlimEAS
//
//  Created by envy.chen on 12/17/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>

#include <map>
#include <string>
#include <vector>

#include "SASDefine.h"

namespace SlimEAS {
  
  /**
   *  This struct represent an encoded parameter
   *  ref : [MS-ASHTTP] sec 2.2.1.1.1.1.1
   */
  typedef struct SASEncodedParam {
    uint8_t tag;
    uint8_t lenght;
    std::string value;
  } SASEncodedParam;
  
  /**
   *  This class represents a base64-encoded query value.
   *  ref : [MS-ASHTTP] sec 2.2.1.1.1
   *  url : http://msdn.microsoft.com/en-us/library/ee202650(v=exchg.80).aspx
   */
  class SASEncodedRequest {
  private:
    uint8_t _commandCode = 0;
    uint16_t _locale = 0;
    
    uint8_t _deviceIdLength = 0;
    std::string _deviceId = "";
    
    uint8_t _policyKeyLength = 0;
    uint32_t _policyKey;
    
    uint8_t _deviceTypeLength = 0;
    std::string _deviceType = "";
    
    std::vector<SASEncodedParam> _commandParams;
    
    std::map<std::string, uint8_t> _commandDict;
    std::map<std::string, uint8_t> _paramDict;
    std::map<std::string, uint16_t> _localeDict;
    
    SAS_PROPERTY(uint8_t, protocolVer);
    
    SAS_GETTER_SYNTHESIZE(uint16_t, locale);
    bool setLocale(const std::string &strLocale) {
      try {
        _locale = _localeDict.at(toUpper(strLocale));
        return true;
      } catch (std::out_of_range e) {
        return false;
      }
    }
    
    SAS_GETTER_SYNTHESIZE(uint8_t, commandCode);
    bool setCommandCode(const std::string &strCmd) {
      try {
        _commandCode = _commandDict.at(toUpper(strCmd));
        return true;
      } catch (std::out_of_range e) {
        return false;
      }
    }
    SAS_GETTER_SYNTHESIZE(int32_t, deviceIdLength);
  
    SAS_GETTER_SYNTHESIZE(const std::string&, deviceId);
    SAS_SETTER(const std::string&, deviceId){
      _deviceId = val;
      _deviceIdLength = _deviceId.length();
    }
    
    SAS_GETTER_SYNTHESIZE(int32_t, policyKeyLength);

    SAS_GETTER_SYNTHESIZE(uint32_t, policyKey);
    SAS_SETTER(uint32_t, policyKey){
      _policyKey = val;
      _policyKeyLength = sizeof(int32_t);
    }
    
    SAS_GETTER_SYNTHESIZE(int32_t, deviceTypeLength);
    
    SAS_GETTER_SYNTHESIZE(const std::string, deviceType);
    SAS_SETTER(const std::string&, deviceType){
      _deviceType = val;
      _deviceTypeLength = _deviceType.length();
    }
    
    SAS_GETTER_SYNTHESIZE(const std::vector<SASEncodedParam>&, commandParams);
    
  public:
    SASEncodedRequest();
    ~SASEncodedRequest();
    
    std::string getBase64EncodingString();
    bool addCommandParam(const std::string &name, const std::string val);
  };
}
