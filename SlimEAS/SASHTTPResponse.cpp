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
SASHTTPResponse::SASHTTPResponse(SlimEAS::SASHTTPRequest::SASHTTPResponseContext &ctx) : _headers(ctx.headers) {
  
  if (ctx.buf_len <= 0) {
    return;
  }
  //decode wbxml to xml
  SASWBXml w2x;
  _xmlResponse = w2x.toXML(ctx.buf, (unsigned int)ctx.buf_len);

  if (_xmlResponse.empty()) {
    throw std::invalid_argument("no xml encoded failed!");
  }
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