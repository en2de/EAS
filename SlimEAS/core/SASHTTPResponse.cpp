//
//  SASHTTPResponse.cpp
//  SlimEAS
//
//  Created by envy.chen on 12/16/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#include "SASHTTPResponse.h"
#include <libxml/xmlreader.h>
#include "SASWBXml.h"

using namespace std;
using namespace SlimEAS;

#pragma mark - life cycle

//simple implement just copy the response data.
SASHTTPResponse::SASHTTPResponse(SlimEAS::SASHTTPRequest::SASHTTPResponseContext &ctx)
: _headers(ctx.headers)
{
}

SASHTTPResponse::~SASHTTPResponse() {
  
}

string SASHTTPResponse::headerString() {
  string str;
  for (auto it = _headers.begin(); it != _headers.end(); it++) {
    str += it->first + " : " + it->second + "\n";
  }
  return str;
}