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
using namespace std;

//get httpResponse headers

SASOptionsRequest::SASOptionsRequest():
_useSSL(true){}

SASOptionsRequest::~SASOptionsRequest(){
}

struct SASOptionsResponse SASOptionsRequest::getReponse() {
  _curl = curl_easy_init();
  SASHTTPResponse res;

    //open this for debuf.
  curl_easy_setopt(_curl, CURLOPT_VERBOSE, 1L);
  curl_easy_setopt(_curl, CURLOPT_DEBUGFUNCTION, SASHTTPResponse::debugCallback);
  curl_easy_setopt(_curl, CURLOPT_DEBUGDATA, &res);

  curl_easy_setopt(_curl, CURLOPT_URL, _server.c_str());
  curl_easy_setopt(_curl, CURLOPT_USERNAME, _userName.c_str());
  curl_easy_setopt(_curl, CURLOPT_PASSWORD, _password.c_str());
  curl_easy_setopt(_curl, CURLOPT_USE_SSL, (long)this->useSSL());
  
  curl_easy_setopt(_curl, CURLOPT_USERAGENT, "Slim-EAS");
  curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, res.writeHandler());
  curl_easy_setopt(_curl, CURLOPT_WRITEDATA, res.writeStream());
  curl_easy_setopt(_curl, CURLOPT_HEADERFUNCTION, res.headerHandler());
  curl_easy_setopt(_curl, CURLOPT_HEADERDATA, &res);
  curl_easy_setopt(_curl, CURLOPT_CUSTOMREQUEST, "OPTIONS");
  curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);
  
  CURLcode e = curl_easy_perform(_curl);
  SASOptionsResponse response;
  if (e == CURLE_OK) {
#ifdef DEBUG
    std::cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
    std::cout << "Reponse Header: \n" << res.headerString();
    std::cout << "\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
#endif
    response.supportedCommand = res.getHeader("MS-ASProtocolCommands");
    response.supportedVersions = res.getHeader("MS-ASProtocolVersions");
  }
  curl_easy_cleanup(_curl);
  
  return response;
}