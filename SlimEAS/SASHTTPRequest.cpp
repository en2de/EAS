//
//  SASHTTPRequest.cpp
//  SlimEAS
//
//  Created by envy.chen on 12/17/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#include "SASHTTPRequest.h"
#include <iostream>

#include <stdarg.h>

using namespace SlimEAS;
using namespace std;

namespace {
  int debug_handler(CURL *handle, curl_infotype type, char *data,size_t size,void *userptr){
    std::cout << "[DEBUG]" << data;
    return 0;
  }
}

#pragma mark - life cycle

SASHTTPRequest::SASHTTPRequest():
_server(""),
_user(""),
_password(""),
_useSSL(true){
}

SASHTTPRequest::SASHTTPRequest(const std::string& server,
                               const std::string& user,
                               const std::string& password,
                               bool useSSL):
_server(server),
_user(user),
_password(password),
_useSSL(useSSL){
  //use easy
  _curl = curl_easy_init();
}

SASHTTPRequest::~SASHTTPRequest() {
  if (_curl) {
    curl_easy_cleanup(_curl);
    _curl = NULL;
  }
}

SASHTTPResponse * SASHTTPRequest::perform() {
  SASHTTPResponse *res = new SASHTTPResponse;
  
#ifdef DEBUG
  //for debug
  SASRequestSetOptions(CURLOPT_VERBOSE, 1L);
  SASRequestSetOptions(CURLOPT_DEBUGFUNCTION, debug_handler);
  
#endif
  SASRequestSetOptions(CURLOPT_USERAGENT, "Slim-EAS");
  SASRequestSetOptions(CURLOPT_USE_SSL, 1L);
  SASRequestSetOptions(CURLOPT_FOLLOWLOCATION, (long)_useSSL);
  
  //set response handlers
  SASRequestSetOptions(CURLOPT_WRITEFUNCTION, res->writeHandler());
  SASRequestSetOptions(CURLOPT_WRITEDATA, &(res->writeStream()));
  SASRequestSetOptions(CURLOPT_HEADERFUNCTION, res->headerHandler());
  SASRequestSetOptions(CURLOPT_HEADERDATA, res);
  
  CURLcode e = curl_easy_perform(_curl);
  if (e != CURLE_OK) {
    delete res;
    
    throw std::invalid_argument("request failed");
  }
  
  return res;
}

#pragma mark - member functions

void SASHTTPRequest::requestBegin() {
  _curl = curl_easy_init();
}

void SASHTTPRequest::requestEnd() {
  if (_curl) {
    curl_easy_cleanup(_curl);
  }
}


