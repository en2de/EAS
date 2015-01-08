//
//  SASSendMailResponse.cpp
//  SlimEAS
//
//  Created by Focus Lan on 1/7/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASSendMailResponse.h"

#include <iostream>
#include <libxml/xmlreader.h>

using namespace SlimEAS;

SASSendMailResponse::SASSendMailResponse(SASHTTPRequest::SASHTTPResponseContext &ctx)
: SASCommandResponse(ctx)
{
  std::cout << _xmlResponse;
}

SASSendMailResponse::~SASSendMailResponse() {
  
}
