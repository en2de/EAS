//
//  SASHTTPRequest.cpp
//  SlimEAS
//
//  Created by envy.chen on 12/17/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#include "SASHTTPRequest.h"
#include <iostream>
#include <vector>
#include <stdarg.h>

#include "SASHTTPResponse.h"

using namespace SlimEAS;
using namespace std;

#pragma mark - response data handlers

namespace {
  int debug_handler(CURL *handle, curl_infotype type, char *data,size_t size,void *userptr){
    std::cout << "[DEBUG]" << data;
    return 0;
  }
  
  size_t _writeHandler(char *data, size_t size, size_t nmemb, void *puser) {
    SASHTTPRequest::SASHTTPResponseContext *ctx = (SASHTTPRequest::SASHTTPResponseContext *)puser;
    
    size_t data_len = size * nmemb;
    if (ctx->buf_len == 0) {
      if (data_len != 0) {
        throw invalid_argument("buffer initialize failed");
      }
      return data_len;
    }
    
    memcpy(ctx->buf + ctx->use, data, size * nmemb);
    ctx->use += data_len;
    
    return data_len;
  }
  
  static const string CRLF = "\r\n";
  void _parseHeader(SASHTTPRequest::SASHTTPResponseContext *ctx, string &s){
    //empty line
    if (s.compare(CRLF) == 0){
      return;
    }
    
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
      return;
    } else {
      string name = line.substr(0, dIdx);
      string val = line.substr(dIdx + 2, line.length() - 1);        //delimiter ": " 2 chars
      
      ctx->headers[name] += val;
      
      //initialize body buffer
      if (name.compare("Content-Length") == 0) {
        ctx->buf_len = std::stoi(val);
        if (ctx->buf_len) {
          ctx->buf = (uint8_t *)malloc(ctx->buf_len);
          ctx->use = 0;
          memset(ctx->buf, 0, ctx->buf_len);
        }
      }
    }
  }
  
  size_t _headerHandler(char *data, size_t size, size_t nmemb, void *puser) {
    SASHTTPRequest::SASHTTPResponseContext *ctx = (SASHTTPRequest::SASHTTPResponseContext *)puser;
    
    char *pd = data;
    size_t result(0);
    if (ctx) {
      string line(pd, size * nmemb);
      _parseHeader(ctx, line);
      result = size * nmemb;
    }
    
    return result;
  }
}

#pragma mark - life cycle

SASHTTPRequest::SASHTTPRequest():
_server(""),
_user(""),
_password(""),
_useSSL(true){
  _curl = curl_easy_init();
  
  _resContext.buf = NULL;
}

SASHTTPRequest::~SASHTTPRequest() {
  if (_curl) {
    curl_easy_cleanup(_curl);
    _curl = NULL;
  }
  
  if (_resContext.buf) {
    free(_resContext.buf);
  }
}

#pragma mark - member func

SASHTTPResponse *SASHTTPRequest::initialResponse() {
  return new SASHTTPResponse(_resContext);
}

SASHTTPResponse * SASHTTPRequest::perform() {
#ifdef DEBUG
  //for debug
  SASRequestSetOptions(CURLOPT_VERBOSE, 1L);
  SASRequestSetOptions(CURLOPT_DEBUGFUNCTION, debug_handler);

#endif
  
  SASRequestSetOptions(CURLOPT_USERAGENT, "Slim-EAS");
  SASRequestSetOptions(CURLOPT_USE_SSL, (long)_useSSL);
  SASRequestSetOptions(CURLOPT_FOLLOWLOCATION, 1L);
  
  //set response handlers
  SASRequestSetOptions(CURLOPT_WRITEFUNCTION, _writeHandler);
  SASRequestSetOptions(CURLOPT_WRITEDATA, &(this->_resContext));
  SASRequestSetOptions(CURLOPT_HEADERFUNCTION, _headerHandler);
  SASRequestSetOptions(CURLOPT_HEADERDATA, &(this->_resContext));
    
  CURLcode e = curl_easy_perform(_curl);
  if (e != CURLE_OK) {
    throw std::invalid_argument("request failed");
  }
  
  SASHTTPResponse *res = this->initialResponse();
  return res;
}



