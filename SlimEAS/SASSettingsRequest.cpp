//
//  SASSettingsRequest.cpp
//  SlimEAS
//
//  Created by guo gloria on 15/1/8.
//  Copyright (c) 2015年 Slim. All rights reserved.
//

#include <iostream>
#include <time.h>

#include "SASSettingsRequest.h"
#include "SASSettingsResponse.h"
#include "SASDevice.h"
#include "SASOof.h"
#include "SASUserInformation.h"

using namespace SlimEAS;
static const int DATETIME_LENGTH = 24;

SASSettingsRequest::SASSettingsRequest(const std::string &server, const std::string &user, const std::string &password, bool useSSL)
: SASCommandRequest(server, user, password, useSSL)
{
  _command = CMD_SETTINGS;
  
  _buf = xmlBufferCreate();
  _writer = xmlNewTextWriterMemory(_buf, 0);
  xmlTextWriterSetIndent(_writer, 1);
  xmlTextWriterStartDocument(_writer, "1.0", "utf-8", NULL);
  xmlTextWriterWriteDTD(_writer, BAD_CAST "ActiveSync", BAD_CAST "-/MICROSOFT/DTD ActiveSync/EN", BAD_CAST "http://www.microsoft.com/", NULL);
  xmlTextWriterStartElementNS(_writer, NULL, BAD_CAST "Settings", BAD_CAST "Settings");
  
}

SASSettingsRequest::SASSettingsRequest()
{
}

SASSettingsRequest::~SASSettingsRequest()
{
  delete _device;
  _device = nullptr;
  
  delete _oof;
  _oof = nullptr;
  
  delete _userInformation;
  _userInformation = nullptr;
}

void SASSettingsRequest::requestDeviceInformation(SASDevice *deviceInfo)
{
  xmlTextWriterStartElement(_writer, BAD_CAST "DeviceInformation");
  // request to get device information
  if (deviceInfo == nullptr) {
    xmlTextWriterStartElement(_writer, BAD_CAST "Get");
    xmlTextWriterEndElement(_writer);
  } else {
    xmlTextWriterStartElement(_writer, BAD_CAST "Set");
    
    xmlTextWriterWriteElement(_writer, BAD_CAST "Model", BAD_CAST deviceInfo->model().c_str());
    xmlTextWriterWriteElement(_writer, BAD_CAST "IMEI", BAD_CAST deviceInfo->IMEI().c_str());
    xmlTextWriterWriteElement(_writer, BAD_CAST "FriendlyName", BAD_CAST deviceInfo->friendlyName().c_str());
    xmlTextWriterWriteElement(_writer, BAD_CAST "OS", BAD_CAST deviceInfo->OS().c_str());
    xmlTextWriterWriteElement(_writer, BAD_CAST "OSLanguage", BAD_CAST deviceInfo->OS_lang().c_str());
    xmlTextWriterWriteElement(_writer, BAD_CAST "PhoneNumber", BAD_CAST deviceInfo->phoneNumber().c_str());
    xmlTextWriterWriteElement(_writer, BAD_CAST "MobileOperator", BAD_CAST deviceInfo->mobileOperator().c_str());
    xmlTextWriterWriteElement(_writer, BAD_CAST "UserAgent", BAD_CAST deviceInfo->userAgent().c_str());
    
    xmlTextWriterEndElement(_writer);
    
    xmlTextWriterEndElement(_writer);
  }
  
  xmlTextWriterEndElement(_writer);
}

void SASSettingsRequest::requestRightsManagementInformation()
{
  xmlTextWriterStartElement(_writer, BAD_CAST "RightsManagementInformation");
  xmlTextWriterStartElement(_writer, BAD_CAST "Get");
  xmlTextWriterEndElement(_writer);
  xmlTextWriterEndElement(_writer);
}

void SASSettingsRequest::requestGetOof(int bodyType)
{
  std::string type;
  if (bodyType == SASOofMessageBodyType_HTML) {
    type = "HTML";
  } else if (bodyType == SASOofMessageBodyType_Text) {
    type = "Text";
  } else {
    std::invalid_argument("Wrong type to get Oof.");
  }
  xmlTextWriterStartElement(_writer, BAD_CAST "Oof");
  
  xmlTextWriterStartElement(_writer, BAD_CAST "Get");
  xmlTextWriterWriteElement(_writer, BAD_CAST "BodyType", BAD_CAST type.c_str());
  xmlTextWriterEndElement(_writer);
  
  xmlTextWriterEndElement(_writer);

}

void SASSettingsRequest::requestSetOof(SASOof *oof)
{
  if (oof == nullptr)  {
    throw std::invalid_argument("Wrong argument to set Oof.");
  }
  xmlTextWriterStartElement(_writer, BAD_CAST "Oof");
  
  xmlTextWriterStartElement(_writer, BAD_CAST "Set");
  if (oof->oofState() != SASOof::SASOofState_Unset) {
    xmlTextWriterWriteFormatElement(_writer, BAD_CAST "OofState", "%d", oof->oofState());
  }

  if (oof->startTime() >= 0) {
    char buf[DATETIME_LENGTH];
    time_t tmpTime = oof->startTime();
    // the minimum unit of time_t is second, so just set milliseconds to "000".
    std::strftime(buf, DATETIME_LENGTH, "%Y-%m-%dT%T.000Z\0", gmtime(&tmpTime));
    xmlTextWriterWriteElement(_writer, BAD_CAST "StartTime", BAD_CAST buf);
  }
  
  if (oof->endTime() >= 0) {
    char buf[DATETIME_LENGTH+1];
    time_t tmpTime = oof->endTime();
    // the minimum unit of time_t is second, so just set milliseconds to "000".
    strftime(buf, DATETIME_LENGTH,  "%Y-%m-%dT%T.000Z\0", gmtime(&tmpTime));
    xmlTextWriterWriteElement(_writer, BAD_CAST "EndTime", BAD_CAST buf);
  }
  
  for (auto oofMessage: oof->oofMessages()) {
    xmlTextWriterStartElement(_writer, BAD_CAST "OofMessage");  // <OofMessage>
    
    if (oofMessage->_isAppliesToInternal) {
      xmlTextWriterStartElement(_writer, BAD_CAST "AppliesToInternal");
      xmlTextWriterEndElement(_writer);
    }
    
    if (oofMessage->_isAppliesToExternalKnown) {
      xmlTextWriterStartElement(_writer, BAD_CAST "AppliesToExternalKnown");
      xmlTextWriterEndElement(_writer);
    }
    
    if (oofMessage->_isAppliesToExternalUnknown) {
      xmlTextWriterStartElement(_writer, BAD_CAST "AppliesToExternalUnknown");
      xmlTextWriterEndElement(_writer);
    }
    
    if (oofMessage->_isEnabled) {
      xmlTextWriterWriteElement(_writer, BAD_CAST "Enabled", BAD_CAST std::to_string(oofMessage->_isEnabled).c_str());
    }
    
    if (!oofMessage->_replyMessasge.empty()) {
      xmlTextWriterWriteElement(_writer, BAD_CAST "ReplyMessage", BAD_CAST oofMessage->_replyMessasge.c_str());
    }
    
    if (oofMessage->_bodyType == SASOofMessageBodyType_HTML) {
      xmlTextWriterWriteElement(_writer, BAD_CAST "BodyType", BAD_CAST "HTML");
    } else if (oofMessage->_bodyType == SASOofMessageBodyType_Text) {
      xmlTextWriterWriteElement(_writer, BAD_CAST "BodyType", BAD_CAST "Text");
    }
    xmlTextWriterEndElement(_writer); // </OofMessage>
  }
  
  xmlTextWriterEndElement(_writer);
  
  xmlTextWriterEndElement(_writer);
}

void SASSettingsRequest::requestSetNewPassword(std::string password)
{
  xmlTextWriterStartElement(_writer, BAD_CAST "DevicePassword");
  xmlTextWriterStartElement(_writer, BAD_CAST "Set");
  
  xmlTextWriterWriteElement(_writer, BAD_CAST "Password", BAD_CAST password.c_str());
  
  xmlTextWriterEndElement(_writer);
  xmlTextWriterEndElement(_writer);
}

void SASSettingsRequest::requestUserInformation(SASUserInformation *userInfo)
{
}

void SASSettingsRequest::generateXMLPayload()
{
  
  
  // end element and document which started in constructor
  xmlTextWriterEndElement(_writer); // </Settings>
  xmlTextWriterEndDocument(_writer);  // end xml
  
  _xmlPayload = std::string((char *)_buf->content);
  
  std::cout << "Settings Request: " << _xmlPayload << std::endl;
}

SASHTTPResponse *SASSettingsRequest::initialResponse()
{
  return new SASSettingsResponse(_resContext);
}

