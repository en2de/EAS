//
//  SASSettingsRequest.h
//  SlimEAS
//
//  Created by guo gloria on 15/1/8.
//  Copyright (c) 2015年 Slim. All rights reserved.
//

#ifndef __SlimEAS__SASSettingsRequest__
#define __SlimEAS__SASSettingsRequest__

#include <stdio.h>

#include "SASCommandRequest.h"

namespace SlimEAS {
  class SASDevice;
  class SASOof;
  class SASUserInformation;
  
  class SASSettingsRequest: public SASCommandRequest {
  public:
    typedef enum {
      SASSettingsOperation_Get,
      SASSettingsOperation_Set
    } SASSettingsOperation;
  public:
    SASSettingsRequest(const std::string &server, const std::string &user, const std::string &password, bool useSSL = true);
    SASSettingsRequest();
    ~SASSettingsRequest();
    
    void requestDeviceInformation(SASSettingsOperation op, SASDevice *deviceInfo = nullptr);
    void requestRightsManagementInformation();
    void requestOof(SASSettingsOperation op, SASOof *oof = nullptr);
    void requestSetNewPassword(std::string password);
    void requestUserInformation(SASSettingsOperation op, SASUserInformation *userInfo);
    

  protected:
    virtual void generateXMLPayload() override;
    virtual SASHTTPResponse *initialResponse() override;
    
  private:
    SASDevice *_device;
    bool _isGetRightsManagementInformation;
    SASOof *_oof;
    std::string _devicePassword;
    SASUserInformation *_userInformation;
    
  };
}

#endif /* defined(__SlimEAS__SASSettingsRequest__) */
