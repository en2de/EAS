//
//  SASHTTPResponse.h
//  SlimEAS
//
//  Created by envy.chen on 12/16/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <sstream>
#include <map>
#include <string>
#include <curl/curl.h>
#include "SASDefine.h"


namespace SlimEAS {
  
  typedef size_t (*CurlCallback)(char *data, size_t size, size_t nmemb, void *puser);
  typedef std::map<std::string, std::string> SASHTTPResponseHeader;
  
  class SASHTTPResponse {
  protected:
    SASHTTPResponseHeader _HTTPheaders;
    
    std::string _headerString;
    
    std::string *xmlData();        //get data decode to xml.
  public:
    SASHTTPResponse();
    virtual ~SASHTTPResponse();
    
    uint8_t *_buf;
    size_t _buf_offset;
    size_t _buf_len;
    
    
    SAS_PROPERTY(std::string, version);
    SAS_PROPERTY(std::string, statusCode);
    SAS_PROPERTY(std::string, status);
    SAS_PROPERTY(std::string, body);
    
    SAS_GETTER(const std::string &, headerString) {
      return _headerString;
    };
    
    SAS_GETTER(const SASHTTPResponseHeader &, HTTPHeader) {
      return _HTTPheaders;
    }
    
    void addHeader(const std::string& line);
    
    void setHeader(const std::string &name, const std::string &val) {
      _HTTPheaders[name] = val;
    }

    const std::string &getHeader(const std::string &name) const {
      return _HTTPheaders.at(name);
    }
        
    virtual CurlCallback writeHandler();
    virtual CurlCallback headerHandler();
  };
}
