//
//  SASHTTPResponse.cpp
//  SlimEAS
//
//  Created by envy.chen on 12/16/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#include "SASHTTPResponse.h"

#include <vector>

using namespace std;
using namespace SlimEAS;

#define CRLF "\r\n"

#pragma mark - curl callbacks

namespace {
  size_t _writeHandler(char *data, size_t size, size_t nmemb, void *puser) {
    ostringstream *oss = (ostringstream *)puser;
    
    size_t result(0);
    if (oss) {
      oss->write(data, size * nmemb);
      result = size * nmemb;
    }
    
    return result;
  }
  
  size_t _headerHandler(char *data, size_t size, size_t nmemb, void *puser) {    
    SASHTTPResponse *rsp = (SASHTTPResponse *)puser;
    
    char *pd = data;
    size_t result(0);
    if (rsp) {
      string line(pd, size * nmemb);
      rsp->addHeader(line);
      result = size * nmemb;
    }
    
    return result;
  }
}

#pragma mark - for test

int SASHTTPResponse::debugCallback(CURL *handle, curl_infotype type, char *data, size_t size, void *puser){
  if (type == CURLINFO_HEADER_OUT) {
    
  }
  
  printf("[DEBUG] %s",data);
  return 0;
}

#pragma mark - life cycle

SASHTTPResponse::SASHTTPResponse(): _headerString(""), _body(""), _version(""), _statusCode(""), _status("") {
  _ostream = new ostringstream;
  _headers = new map<string, string>();
}

SASHTTPResponse::~SASHTTPResponse() {
  delete _ostream;
  delete _headers;
}

#pragma mark - member functions

void SASHTTPResponse::addHeader(const std::string &s){
  //empty line
  if (s.compare(CRLF) == 0){
    return;
  }
  
  _headerString.append(s);
  string line(s);
  
  //remote CR/Lf  LF: 0x0D CR: 0x0A
  size_t lfIdx = line.find_first_of(0x0D, 0);
  if (lfIdx != string::npos) {
    line.erase(lfIdx, line.length() - 1);
  }
  
  //delimiter pos
  size_t dIdx = line.find_first_of(":", 0);
  //first line
  if (dIdx == string::npos) {
    istringstream iss(line);
    vector<string> v;
    string str;
    while (iss) {
      iss >> str;
      v.push_back(str);
    }
    
    _version = v.front();
    _statusCode = v.at(1);
    _status = v.back();
  } else {
    string name = line.substr(0, dIdx);
    string val = line.substr(dIdx + 2, line.length() - 1);        //delimiter ": " 2 chars

    this->setHeader(name, string(this->getHeader(name)).append(val));
  }
}

CurlCallback SASHTTPResponse::writeHandler() {
  return _writeHandler;
}

CurlCallback SASHTTPResponse::headerHandler() {
  return _headerHandler;
}
