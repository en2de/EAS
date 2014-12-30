//
//  SASProvisionResponse.h
//  SlimEAS
//
//  Created by envy.chen on 12/19/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>

#include "SASCommandResponse.h"

namespace SlimEAS {
  class SASProvisionResponse: public SASCommandResponse {
  public:
    SASProvisionResponse(SASHTTPRequest::SASHTTPResponseContext &ctx);
    ~SASProvisionResponse();
  };
}
