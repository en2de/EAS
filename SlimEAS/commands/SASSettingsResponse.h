//
//  SASSettingsResponse.h
//  SlimEAS
//
//  Created by guo gloria on 15/1/8.
//  Copyright (c) 2015年 Slim. All rights reserved.
//

#ifndef __SlimEAS__SASSettingsResponse__
#define __SlimEAS__SASSettingsResponse__

#include <stdio.h>
#include "SASCommandResponse.h"

namespace SlimEAS {
  class SASSettingsResponse: public SASCommandResponse
  {
  public:
    SASSettingsResponse(SASHTTPRequest::SASHTTPResponseContext &ctx);
    virtual ~SASSettingsResponse();
  };
}

#endif /* defined(__SlimEAS__SASSettingsResponse__) */
