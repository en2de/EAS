//
//  SASItemOperationsRequest.h
//  SlimEAS
//
//  Created by Focus Lan on 1/6/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//
#pragma once

#include "SASCommandRequest.h"

#include <string>

namespace SlimEAS {
  
  /**
   *  This class is used to fetch email and attachments
   */
  class SASItemOperationsRequest: public SASCommandRequest {
    
  public:
    SASItemOperationsRequest();
    ~SASItemOperationsRequest() = 0;
    
  protected:
    virtual void generateXMLPayload();
    virtual SASHTTPResponse *initialResponse();
    virtual void generateXMLSubPayload() = 0;
  
  public:
    void setStore(const std::string &store) {_store = store;}
    const std::string &store() {return _store;}
  
  protected:
    std::string       _store;
  };
}