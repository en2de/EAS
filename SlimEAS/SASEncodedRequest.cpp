//
//  SASEncodedRequest.cpp
//  SlimEAS
//
//  Created by envy.chen on 12/17/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#include "SASEncodedRequest.h"
#include <algorithm>
#include <iostream>

#include "b64.h"

using namespace SlimEAS;
using namespace std;

string SlimEAS::toUpper(const std::string &name){
  string str(name);
  transform(str.begin(), str.end(),str.begin(), ::toupper);
  return str;
}

SASEncodedRequest::SASEncodedRequest() {
  //Load command dictionary
  //Values taken from [MS-ASHTTP] sec 2.2.1.1.1.1.2
  _commandDict["SYNC"] = 0;
  _commandDict["SENDMAIL"] = 1;
  _commandDict["SMARTFORWARD"] = 2;
  _commandDict["SMARTREPLY"] = 3;
  _commandDict["GETATTACHMENT"] = 4;
  _commandDict["FOLDERSYNC"] = 9;
  _commandDict["FOLDERCREATE"] = 10;
  _commandDict["FOLDERDELETE"] = 11;
  _commandDict["FOLDERUPDATE"] = 12;
  _commandDict["MOVEITEMS"] = 13;
  _commandDict["GETITEMESTIMATE"] = 14;
  _commandDict["MEETINGRESPONSE"] = 15;
  _commandDict["SEARCH"] = 16;
  _commandDict["SETTINGS"] = 17;
  _commandDict["PING"] = 18;
  _commandDict["ITEMOPERATIONS"] = 19;
  _commandDict["PROVISION"] = 20;
  _commandDict["RESOLVERECIPIENTS"] = 21;
  _commandDict["VALIDATECERT"] = 22;
  
  // Load parameter dictionary
  // Values taken from [MS-ASHTTP] sec 2.2.1.1.1.1.3
  _paramDict["ATTACHMENTNAME"] = 0;
  _paramDict["ITEMID"] = 3;
  _paramDict["LONGID"] = 4;
  _paramDict["OCCURRENCE"] = 6;
  _paramDict["OPTIONS"] = 7;
  _paramDict["USER"] = 8;
  
  // Load locale dictionary
  // TODO: Add other locales
  _localeDict["EN-US"] = 0x0409;
}

SASEncodedRequest::~SASEncodedRequest() {
  
}

string SASEncodedRequest::getBase64EncodingString() {
  uint8_t buf[256] = {0};
  uint8_t *pb = buf;
  
  *pb++ = _protocolVer;
  *pb++ = _commandCode;
  
  //endian swap
  uint8_t *pl = (uint8_t *)&_locale;
  uint16_t l = (*pl << 8) + *(pl + 1);
  
  memcpy(pb, &l, sizeof(_locale));
  pb += sizeof(_locale);
  
  *pb++ = _deviceIdLength;
  if (_deviceIdLength > 0) {
    memcpy(pb, _deviceId.c_str(), _deviceIdLength);
    pb += _deviceId.length();
  }
  
  *pb++ = _policyKeyLength;
  if (_policyKeyLength > 0) {
    memcpy(pb, &_policyKey, _policyKeyLength);
    pb += _policyKeyLength;
  }
  
  *pb++ = _deviceTypeLength;
  if (_deviceTypeLength > 0) {
    memcpy(pb, _deviceType.c_str(), _deviceTypeLength);
    pb += _deviceTypeLength;
  }
  
  if (_commandParams.size() > 0) {
    for (auto it = _commandParams.begin(); it != _commandParams.end(); it++) {
      *pb++ = (*it).tag;
      *pb++ = (*it).lenght;
      memcpy(pb, (*it).value.c_str(), (*it).value.length());
      pb += (*it).value.length();
    }
  }
  
  size_t buf_len = pb - buf;
  pb = NULL;
  
  char *b64 = NULL;
  b64 = b64_encode(buf, buf_len);
  string result(b64);
  
  return result;
}

bool SASEncodedRequest::addCommandParam(const std::string &name, const std::string val) {
  SASEncodedParam param;
  uint8_t paramTag;
  try {
    paramTag = _paramDict.at(toUpper(name));
    
    param.tag = paramTag;
    param.value = val;
    param.lenght = val.length();
    
    if (_commandParams.size() == 0) {
      _commandParams.push_back(param);
    }
    
    return true;
  } catch (std::out_of_range e) {
    return false;
  }
}