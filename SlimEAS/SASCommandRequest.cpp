//
//  SASCommandRequest.cpp
//  SlimEAS
//
//  Created by envy.chen on 12/17/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#include "SASCommandRequest.h"
#include "SASEncodedRequest.h"

#include "SASWBXml.h"

#include <iostream>
#include <stdlib.h>

using namespace SlimEAS;
using SlimEAS::SASHTTPRequest;
using namespace std;

SASCommandRequest::SASCommandRequest(): SASHTTPRequest() {
  this->_protocolVersion = "";
  this->_command = "";
  this->_deviceID = "";
  this->_deviceType = "";
}

SASCommandRequest::SASCommandRequest(const string& server,
                                     const string& user,
                                     const string& password,
                                     bool useSSL): SASHTTPRequest(server, user, password, useSSL) {
  this->_protocolVersion = "";
  this->_command = "";
  this->_deviceID = "";
  this->_deviceType = "";
}

uint8_t * SASCommandRequest::getWBXml(unsigned int *olen) {
  uint8_t *wbxml = NULL;
  
  string xml = this->requestBody();
  SASWBXml x2w;
  unsigned int len = 0;
  wbxml = x2w.toWBXml(xml, &len);
  *olen = len;
  
  if (wbxml == NULL) {
    throw std::invalid_argument("no wbxml has been encoded!");
  }
  
  return wbxml;
}

/**
 *  build a request line from the class properties
 *
 *  @return request line in base64
 */
string SASCommandRequest::getRequestLine() {
  if (_command.empty() || _user.empty() || _deviceID.empty() || _deviceType.empty())
    throw new std::invalid_argument("need fill all request fields");
  
  string reqLine = "";
  
  if (_useEncodeRequestLine == true) {
    
    // Use the EncodedRequest class to generate
    // an encoded request line
    SASEncodedRequest encodedReq;
    encodedReq.protocolVer(std::stof(_protocolVersion)  * 10);
    encodedReq.setCommandCode(_command);
    encodedReq.setLocale("en-us");
    encodedReq.deviceId(_deviceID);
    encodedReq.deviceType(_deviceType);
    encodedReq.policyKey(_policyKey);
    
    // Add the User parameter to the request line
    encodedReq.addCommandParam("User", _user);
    
    // Add any command-specific parameters
    if (_commandParams.size() > 0) {
      for (auto it = _commandParams.begin(); it != _commandParams.end(); it++) {
        encodedReq.addCommandParam((*it).first, (*it).second);
      }
    }
    
    reqLine = encodedReq.getBase64EncodingString();
  }else{
    reqLine += "Cmd=" + _command + "&";
    reqLine += "User=" + _user + "&";
    reqLine += "DeviceId=" + _deviceID + "&";
    reqLine += "DeviceType=" + _deviceType;
  
    if (_commandParams.size() > 0) {
      for (auto it = _commandParams.begin(); it != _commandParams.end(); it++) {
        reqLine = reqLine + "&" + (*it).first + "=" + (*it).second;
      }
    }
  }
  
  return reqLine;
}

SASHTTPResponse *SASCommandRequest::getResponse() {
  string uri = _server + "/Microsoft-Server-ActiveSync?" + this->getRequestLine();
  
  SASRequestSetOptions(CURLOPT_URL, uri.c_str());
  SASRequestSetOptions(CURLOPT_USERNAME, _user.c_str());
  SASRequestSetOptions(CURLOPT_PASSWORD, _password.c_str());
  SASRequestSetOptions(CURLOPT_POST, 1L);
  
  // MS-ASHTTP section 2.2.1.1.2.2 headers
  curl_slist *header = NULL;
  header = curl_slist_append(header, "Content-Type: application/vnd.ms-sync.wbxml");
  header = curl_slist_append(header, "Accept-Language: en-us");
  header = curl_slist_append(header, "Accept:");

  if (!_useEncodeRequestLine) {
    header = curl_slist_append(header, string("MS-ASProtocolVersion: " + _protocolVersion).c_str());
    header = curl_slist_append(header, string("X-MS-PolicyKey: " + std::to_string(_policyKey)).c_str());
  }
  SASRequestSetOptions(CURLOPT_HTTPHEADER, header);
  
  //append data
  unsigned int len = 0;
  uint8_t *wbxml = this->getWBXml(&len);
  SASRequestSetOptions(CURLOPT_POSTFIELDSIZE, (long)len);
  SASRequestSetOptions(CURLOPT_POSTFIELDS, wbxml);
  
  try {
    return this->perform();
  } catch (exception &e) {
    throw e;
  }
}

SASCommandRequest::~SASCommandRequest() {
  
}

