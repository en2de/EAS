//
//  SASSendMailRequest.h
//  SlimEAS
//
//  Created by Focus Lan on 1/7/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#pragma once

#include "SASCommandRequest.h"
#include "SASMail.h"

namespace SlimEAS {
  
  class SASSendMailRequest: public SASCommandRequest {
    
  public:
    SASSendMailRequest();
    ~SASSendMailRequest();
    
  protected:
    virtual void generateXMLPayload();
    virtual SASHTTPResponse *initialResponse();
    
  public:
    void setMail(const SASMail &mail){ _mail = mail;}
    SASMail &mail() { return _mail;}
    
  private:
    SASMail _mail;
  };
}