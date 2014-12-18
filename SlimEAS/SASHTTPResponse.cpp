//
//  SASHTTPResponse.cpp
//  SlimEAS
//
//  Created by envy.chen on 12/16/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#include "SASHTTPResponse.h"
#include <vector>
#include <libxml/xmlreader.h>
#include "SASWBXml.h"

using namespace std;
using namespace SlimEAS;

#pragma mark - curl callbacks

namespace {
  static const string CRLF = "\r\n";
  
  size_t _writeHandler(char *data, size_t size, size_t nmemb, void *puser) {
    SASHTTPResponse *rsp = (SASHTTPResponse *)puser;
    
    size_t data_len = size * nmemb;
    
    if (rsp->_buf_len == 0) {
      if (data_len != 0) {
        throw invalid_argument("buffer initialize failed!");
      }
      return data_len;
    }
    
    memcpy(rsp->_buf + rsp->_buf_offset, data, size * nmemb);
    rsp->_buf_offset += data_len;
    
    return data_len;
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

#pragma mark - life cycle

SASHTTPResponse::SASHTTPResponse(): _headerString(""), _body(""), _version(""), _statusCode(""), _status("") {
  
}

SASHTTPResponse::~SASHTTPResponse() {
  if (_buf) {
    free(_buf);
  }
}

#pragma mark - member functions

string *SASHTTPResponse::xmlData() {
  string *xml;
  SASWBXml w2x;
  xml = w2x.toXML(_buf, (int)_buf_len);
  
  if (xml->empty()) {
    throw std::invalid_argument("no xml has been encoded!");
  }
  
  return xml;
}

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
    
    _HTTPheaders[name] += val;
    
    //initialize body buffer
    if (name.compare("Content-Length") == 0) {
      _buf_len = std::stoi(val);
      if (_buf_len) {
        _buf = (uint8_t *)malloc(_buf_len);
        _buf_offset = 0;
        memset(_buf, 0, _buf_len);
      }
    }
  }
}

CurlCallback SASHTTPResponse::writeHandler() {
  return _writeHandler;
}

CurlCallback SASHTTPResponse::headerHandler() {
  return _headerHandler;
}
