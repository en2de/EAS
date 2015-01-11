//
//  SASSendMailRequest.h
//  SlimEAS
//
//  Created by Focus Lan on 1/7/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#pragma once

#include "SASMailBaseRequest.h"

namespace SlimEAS {
  
  class SASSendMailRequest: public SASMailBaseRequest {
    
  public:
    SASSendMailRequest();
    ~SASSendMailRequest();
  };
}