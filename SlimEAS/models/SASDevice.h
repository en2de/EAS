//
//  SASDevice.h
//  SlimEAS
//
//  Created by envy.chen on 12/19/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <string>

#include "SASDefine.h"

namespace SlimEAS {
  class SASDevice {
  private:
    std::string _deviceID;
    std::string _deviceType;
    std::string _model;
    std::string _IMEINumber;
    std::string _friendlyName;
    std::string _operatingSystem;
    std::string _operatingSystemLanguage;
    std::string _phoneNumber;
    std::string _mobileOperator;
    std::string _userAgent;
    
  public:
    const std::string &deviceId() {return _deviceID;}
    void setDeviceId(const std::string &val) { _deviceID = val;}
    
    const std::string &deviceType() {return _deviceType;}
    void setDeviceType(const std::string &val) { _deviceType = val;}
    
    const std::string &model() {return _model;}
    void setModel(const std::string &val) { _model = val;}
    
    const std::string &IMEI() {return _IMEINumber;}
    void setIMEI(const std::string &val) { _IMEINumber = val;}
    
    const std::string &friendlyName() {return _friendlyName;}
    void setFriendlyName(const std::string &val) {_friendlyName = val;}
    
    const std::string &OS() {return _operatingSystem;}
    void setOS(const std::string &val) {_operatingSystem = val;}
    
    const std::string &OS_lang() {return _operatingSystemLanguage;}
    void setOS_Lang(const std::string &val) {_operatingSystemLanguage = val;}
    
    const std::string &phoneNumber() {return _phoneNumber;}
    void setPhoneNumber(const std::string &val) {_phoneNumber = val;}
    
    const std::string &mobileOperator() {return _mobileOperator;}
    void setMobileOperator(const std::string &val) {_mobileOperator = val;}
    
    const std::string &userAgent() {return _userAgent;}
    void setUserAgent(const std::string &val) {_userAgent = val;}
    
    std::string payload();
  };
}
