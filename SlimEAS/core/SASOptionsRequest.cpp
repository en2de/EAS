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

#pragma mark - life cycle

SASOptionsRequest::SASOptionsRequest(): SASHTTPRequest() {
  
}

SASOptionsRequest::~SASOptionsRequest() {
}

#pragma mark - override func

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
    std::cout << "Response Header: \n" << res->headerString();
    std::cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
#endif
    response.supportedCommand = res->header()["MS-ASProtocolCommands"];
    response.supportedVersions = res->header()["MS-ASProtocolVersions"];
  
    delete res;
  }
  
  return response;
}