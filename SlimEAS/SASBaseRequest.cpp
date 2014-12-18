//
//  SASBaseRequest.cpp
//  SlimEAS
//
//  Created by envy.chen on 12/17/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#include "SASBaseRequest.h"
#include <iostream>

using namespace SlimEAS;
using namespace std;

namespace {
  int debug_handler(CURL *handle, curl_infotype type, char *data,size_t size,void *userptr){
    std::cout << "[DEBUG]" << data;
    return 0;
  }
}

#pragma mark - life cycle

SASBaseRequest::SASBaseRequest():
_server(""),
_userName(""),
_password(""),
_useSSL(true){
}

SASBaseRequest::SASBaseRequest(const std::string& server,
                               const std::string& user,
                               const std::string& password,
                               bool useSSL):
_server(server),
_userName(user),
_password(password),
_useSSL(useSSL){
  
}

SASBaseRequest::~SASBaseRequest() {
  
}

#pragma mark - member functions

void SASBaseRequest::requestBegin() {
  _curl = curl_easy_init();
#ifdef DEBUG
  curl_easy_setopt(_curl, CURLOPT_VERBOSE, 1L);
  curl_easy_setopt(_curl, CURLOPT_DEBUGFUNCTION, debug_handler);
#endif
  
  curl_easy_setopt(_curl, CURLOPT_USERAGENT, "Slim-EAS");
  curl_easy_setopt(_curl, CURLOPT_USE_SSL, 1L);
  curl_easy_setopt(_curl, CURLOPT_FOLLOWLOCATION, 1L);  
}

void SASBaseRequest::requestEnd() {
  if (_curl) {
    curl_easy_cleanup(_curl);
  }
}


