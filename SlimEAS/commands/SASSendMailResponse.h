//
//  SASSendMailResponse.h
//  SlimEAS
//
//  Created by Focus Lan on 1/7/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASCommandResponse.h"

namespace SlimEAS {
  class SASSendMailResponse: public SASCommandResponse {
    
  public:
    SASSendMailResponse(SASHTTPRequest::SASHTTPResponseContext &ctx);
    ~SASSendMailResponse();
  };
}
