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
using SlimEAS::SASBaseRequest;
using namespace std;

SASCommandRequest::SASCommandRequest(): SASBaseRequest() {
  this->_protocolVersion = "";
  this->_command = "";
  this->_deviceID = "";
  this->_deviceType = "";
}

SASCommandRequest::SASCommandRequest(const string& server,
                                     const string& user,
                                     const string& password,
                                     bool useSSL): SASBaseRequest(server, user, password, useSSL) {
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
    throw new std::invalid_argument("no wbxml has been encoded!");
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
  this->requestBegin();
  
  SASHTTPResponse *res = new SASHTTPResponse;
  
  string uri = _server + "/Microsoft-Server-ActiveSync?" + this->getRequestLine();
  
//  curl_easy_setopt(_curl, CURLOPT_PROXY, "127.0.0.1");
//  curl_easy_setopt(_curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);
//  curl_easy_setopt(_curl, CURLOPT_PROXYPORT, 8085);
  
  curl_easy_setopt(_curl, CURLOPT_URL, uri.c_str());
  curl_easy_setopt(_curl, CURLOPT_USERNAME, _user.c_str());
  curl_easy_setopt(_curl, CURLOPT_PASSWORD, _password.c_str());
  
//  curl_easy_setopt(_curl, CURLOPT_TLSAUTH_USERNAME, _userName.c_str());
//  curl_easy_setopt(_curl, CURLOPT_TLSAUTH_PASSWORD, _password.c_str());
//  curl_easy_setopt(_curl, CURLOPT_TLSAUTH_TYPE, CURL_TLSAUTH_SRP);
  
//  curl_easy_setopt(_curl, CURLOPT_TIMEOUT, 30);
  
  curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, res->writeHandler());
  curl_easy_setopt(_curl, CURLOPT_WRITEDATA, &(res->writeStream()));
  curl_easy_setopt(_curl, CURLOPT_HEADERFUNCTION, res->headerHandler());
  curl_easy_setopt(_curl, CURLOPT_HEADERDATA, res);
  curl_easy_setopt(_curl, CURLOPT_POST, 1L);
//  curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, "OPTIONS");
  
  curl_slist *header = NULL;
  // MS-ASHTTP section 2.2.1.1.2.2
  header = curl_slist_append(header, "Content-Type: application/vnd.ms-sync.wbxml");
  header = curl_slist_append(header, "Accept-Language: en-us");
  header = curl_slist_append(header, "Accept:");


  if (!_useEncodeRequestLine) {
    header = curl_slist_append(header, string("MS-ASProtocolVersion: " + _protocolVersion).c_str());
    header = curl_slist_append(header, string("X-MS-PolicyKey: " + std::to_string(_policyKey)).c_str());
  }
  curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, header);
  
  //append data
  unsigned int len = 0;
  uint8_t *wbxml = this->getWBXml(&len);
  
  curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, (long)len);
  curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, wbxml);
  
  CURLcode e = curl_easy_perform(_curl);
  if (e != CURLE_OK) {
    delete res;
    return nullptr;
  }
  
//  std::cout << res->_ostream->str();
  
  curl_slist_free_all(header);
  free(wbxml);
  
  this->requestEnd();
  return nullptr;
}

SASCommandRequest::~SASCommandRequest() {
  
}

