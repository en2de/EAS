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
  
  class SASSmartReplyRequest: public SASMailBaseRequest {
    
  public:
    SASSmartReplyRequest();
    ~SASSmartReplyRequest();
    
  protected:
    virtual void generateMailInfo();
    
  };
}
