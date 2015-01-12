//
//  SASSettingsResponse.cpp
//  SlimEAS
//
//  Created by guo gloria on 15/1/8.
//  Copyright (c) 2015年 Slim. All rights reserved.
//

#include <iostream>

#include "SASSettingsResponse.h"

using namespace SlimEAS;

SASSettingsResponse::SASSettingsResponse(SASHTTPRequest::SASHTTPResponseContext &ctx)
:SASCommandResponse(ctx)
{
  std::cout << "Settings Response: " << _xmlResponse << std::endl;
}

SASSettingsResponse::~SASSettingsResponse()
{
}