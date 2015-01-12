//
//  SASSmartReplyRequest.h
//  SlimEAS
//
//  Created by Focus Lan on 1/8/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#pragma once

#include "SASMailBaseRequest.h"

namespace SlimEAS {
  
  class SASSmartForwardRequest: public SASMailBaseRequest {
    
  public:
    SASSmartForwardRequest();
    ~SASSmartForwardRequest();
    
  protected:
    virtual void generateMailInfo();
  };
}
