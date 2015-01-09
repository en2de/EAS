//
//  SASSmartReplyRequest.h
//  SlimEAS
//
//  Created by Focus Lan on 1/8/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#pragma once

#include "SASCommandRequest.h"
#include "SASMail.h"

namespace SlimEAS {
  
  class SASSmartReplyRequest: public SASCommandRequest {
    
  public:
    SASSmartReplyRequest();
    ~SASSmartReplyRequest();
    
  protected:
    virtual void generateXMLPayload();
    virtual SASHTTPResponse *initialResponse();
    
  public:
    void setMail(const SASMail &mail){ _mail = mail;}
    
  private:
    SASMail _mail;
  };
}
