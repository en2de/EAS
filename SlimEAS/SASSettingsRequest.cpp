//
//  SASSettingsRequest.cpp
//  SlimEAS
//
//  Created by guo gloria on 15/1/8.
//  Copyright (c) 2015年 Slim. All rights reserved.
//

#include "SASSettingsRequest.h"
#include "SASSettingsResponse.h"


using namespace SlimEAS;

SASSettingsRequest::SASSettingsRequest(const std::string &server, const std::string &user, const std::string &password, bool useSSL)
{
}

SASSettingsRequest::SASSettingsRequest()
{
}

SASSettingsRequest::~SASSettingsRequest()
{
}

void SASSettingsRequest::requestDeviceInformation(SASSettingsOperation op, SASDevice *deviceInfo)
{
}
void SASSettingsRequest::requestRightsManagementInformation()
{
}

void SASSettingsRequest::requestOof(SASSettingsOperation op, SASOof *oof)
{
}

void SASSettingsRequest::requestSetNewPassword(std::string password)
{
}

void SASSettingsRequest::requestUserInformation(SASSettingsOperation op, SASUserInformation *userInfo)
{
}

void SASSettingsRequest::generateXMLPayload()
{
}

SASHTTPResponse *SASSettingsRequest::initialResponse()
{
  return new SASSettingsResponse(_resContext);
}

