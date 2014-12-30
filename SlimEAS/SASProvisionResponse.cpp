//
//  SASProvisionResponse.cpp
//  SlimEAS
//
//  Created by envy.chen on 12/19/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#include "SASProvisionResponse.h"

#include <libxml/xmlreader.h>

using namespace SlimEAS;

SASProvisionResponse::SASProvisionResponse(SASHTTPRequest::SASHTTPResponseContext &ctx) : SASCommandResponse(ctx) {
  
}

SASProvisionResponse::~SASProvisionResponse() {
  
}