//
//  SASSyncResponse.cpp
//  SlimEAS
//
//  Created by Focus Lan on 1/6/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASSyncResponse.h"

#include <iostream>

using namespace std;
using namespace SlimEAS;

SASSyncResponse::SASSyncResponse(SASHTTPRequest::SASHTTPResponseContext &ctx)
: SASCommandResponse(ctx)
{
  
}

SASSyncResponse::~SASSyncResponse() {
  
}