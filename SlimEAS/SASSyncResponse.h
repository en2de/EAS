//
//  SASSyncResponse.h
//  SlimEAS
//
//  Created by Focus Lan on 1/6/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#pragma once

#include "SASCommandResponse.h"
#include "SASCollection.h"

#include <string>
#include <vector>

namespace SlimEAS {
  
  class SASSyncResponse: public SASCommandResponse {
  public:
    SASSyncResponse(SASHTTPRequest::SASHTTPResponseContext &ctx);
    ~SASSyncResponse();
    
    const std::string &response() {return _xmlResponse;}
    
  public:
    const std::vector<SASCollection> &collectionList() {return _collections;}
    
  private:
    int32_t                     _status = 0;
    std::vector<SASCollection>  _collections;
  };
}

