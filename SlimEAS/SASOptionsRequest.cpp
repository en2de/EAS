//
//  SASOptionsRequest.cpp
//  SlimEAS
//
//  Created by envy.chen on 12/15/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#include "SASOptionsRequest.h"

#include "SASHTTPResponse.h"
#include <iostream>
#include <sstream>

using namespace SlimEAS;
using SlimEAS::SASBaseRequest;
using namespace std;

//get httpResponse headers

SASOptionsRequest::SASOptionsRequest(): SASBaseRequest() {
  
}

SASOptionsRequest::SASOptionsRequest(const string& server,
                                     const string& user,
                                     const string& password,
                                     bool useSSL): SASBaseRequest(server, user, password, useSSL) {

}

SASOptionsRequest::~SASOptionsRequest() {
}

SASHTTPResponse *SASOptionsRequest::getResponse() {
  this->requestBegin();
  
  SASHTTPResponse *res = new SASHTTPResponse;
  
//  curl_easy_setopt(_curl, CURLOPT_PROXY, "127.0.0.1");
//  curl_easy_setopt(_curl, CURLOPT_PROXYTYPE, CURLPROXY_SOCKS5);
//  curl_easy_setopt(_curl, CURLOPT_PROXYPORT, 8085);
  
  curl_easy_setopt(_curl, CURLOPT_URL, _server.c_str());
  curl_easy_setopt(_curl, CURLOPT_USERNAME, _userName.c_str());
  curl_easy_setopt(_curl, CURLOPT_PASSWORD, _password.c_str());
  
  curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, res->writeHandler());
  curl_easy_setopt(_curl, CURLOPT_WRITEDATA, res->writeStream());
  curl_easy_setopt(_curl, CURLOPT_HEADERFUNCTION, res->headerHandler());
  curl_easy_setopt(_curl, CURLOPT_HEADERDATA, res);
  curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, "OPTIONS");
  CURLcode e = curl_easy_perform(_curl);
  if (e != CURLE_OK) {
    delete res;
    return nullptr;
  }
  
  this->requestEnd();
  return res;
}

struct SASOptionsResponse SASOptionsRequest::getReponse() {
  SASHTTPResponse *res = this->getResponse();
  SASOptionsResponse response;
  if (res != nullptr) {
#ifdef DEBUG
    std::cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    std::cout << "Reponse Header: \n" << res->headerString();
    std::cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
#endif
    response.supportedCommand = res->getHeader("MS-ASProtocolCommands");
    response.supportedVersions = res->getHeader("MS-ASProtocolVersions");
  
    delete res;
  }
  
  return response;
}