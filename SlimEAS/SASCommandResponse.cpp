//
//  SASCommandResponse.cpp
//  SlimEAS
//
//  Created by envy.chen on 12/18/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#include "SASCommandResponse.h"
#include "SASWBXml.h"

using namespace std;
using namespace SlimEAS;

SASCommandResponse::SASCommandResponse(SASHTTPRequest::SASHTTPResponseContext &ctx) : SASHTTPResponse::SASHTTPResponse(ctx) {
  if (ctx.buf_len > 0) {
    SASWBXml w2x;
    _xmlResponse = w2x.toXML(ctx.buf, (unsigned int)ctx.buf_len);
    
    if (_xmlResponse.empty()) {
      throw invalid_argument("response xml decode failed!");
    }
  }
}

SASCommandResponse::~SASCommandResponse() {
  
}
