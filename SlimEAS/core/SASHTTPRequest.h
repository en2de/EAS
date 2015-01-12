//
//  SASHTTPRequest.h
//  SlimEAS
//
//  Created by envy.chen on 12/17/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <curl/curl.h>
#include <map>
#include <string>

#include "SASDefine.h"

#define SASRequestSetOptions(opt,args...)  curl_easy_setopt(_curl, opt, ##args);

namespace SlimEAS {
  
  class SASHTTPResponse;
  
  typedef std::map<std::string, std::string> SASHTTPResponseHeader;

  class SASHTTPRequest {
  public:
    typedef struct {
      uint8_t *buf;
      size_t buf_len;
      size_t use;
      std::map<std::string, std::string> headers;
    } SASHTTPResponseContext;
    
  protected:
    std::string _server;
    std::string _user;
    std::string _password;
    bool _useSSL;
    
    //handle response data
    SASHTTPResponseContext _resContext;
    
    CURL *_curl;
    
    //override this func to provide custom response extended from SASHTTPResponse
    virtual SASHTTPResponse *initialResponse();
    
    //perform request.
    SASHTTPResponse *perform();
  public:
    
    SASHTTPRequest();
    SASHTTPRequest(const std::string &server, const std::string &user, const std::string &password, bool useSSL = true);
    virtual ~SASHTTPRequest() = 0;
    
    std::string &server() {return _server;}
    void setServer(const std::string &server) {
      _server = server;
    }
    
    std::string &user() {return _user;}
    void setUser(const std::string &user) {
      _user = user;
    }
    
    std::string &password() {return _password;}
    void setPassword(const std::string &psw) {
      _password = psw;
    }
    
    bool useSSL() {return _useSSL;}
    void setUseSSL(bool useSSL) {
      _useSSL = useSSL;
    }
    
//    SAS_PROPERTY(std::string, requestBody);
    
    //You must implement this func.
    //setup custom request option.
    //then call this->perform()
    virtual SASHTTPResponse *getResponse() = 0;
  };
}
