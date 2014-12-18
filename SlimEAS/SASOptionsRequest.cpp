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
using SlimEAS::SASHTTPRequest;
using namespace std;

//get httpResponse headers

SASOptionsRequest::SASOptionsRequest(): SASHTTPRequest() {
  
}

SASOptionsRequest::SASOptionsRequest(const string& server,
                                     const string& user,
                                     const string& password,
                                     bool useSSL): SASHTTPRequest(server, user, password, useSSL) {

}

SASOptionsRequest::~SASOptionsRequest() {
}

SASHTTPResponse *SASOptionsRequest::getResponse() {
  SASRequestSetOptions(CURLOPT_URL, _server.c_str());
  SASRequestSetOptions(CURLOPT_USERNAME, _user.c_str());
  SASRequestSetOptions(CURLOPT_PASSWORD, _password.c_str());
  
  SASRequestSetOptions(CURLOPT_CUSTOMREQUEST, "OPTIONS");
  
  try {
    return this->perform();
  } catch (exception &e) {
    throw e;
  }
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