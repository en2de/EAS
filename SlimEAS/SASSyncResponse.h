//
//  SASSyncResponse.h
//  SlimEAS
//
//  Created by Focus Lan on 1/6/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#ifndef __SlimEAS__SASSyncResponse__
#define __SlimEAS__SASSyncResponse__

#include "SASCommandResponse.h"
#include <string>

namespace SlimEAS {
  
  class SASSyncResponse: public SASCommandResponse {
  public:
    SASSyncResponse(SASHTTPRequest::SASHTTPResponseContext &ctx);
    ~SASSyncResponse();
    
    const std::string &response() {return _xmlResponse;}
    
  private:
    int32_t _status = 0;
  };
}

#endif /* defined(__SlimEAS__SASSyncResponse__) */
