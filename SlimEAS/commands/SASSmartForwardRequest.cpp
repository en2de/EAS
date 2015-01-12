//
//  SASSmartReplyRequest.cpp
//  SlimEAS
//
//  Created by Focus Lan on 1/8/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASSmartForwardRequest.h"

using namespace SlimEAS;
using namespace std;

SASSmartForwardRequest::SASSmartForwardRequest()
: SASMailBaseRequest()
{
  _command = CMD_SMARTFORWARD;
}

SASSmartForwardRequest::~SASSmartForwardRequest() {

}

void SASSmartForwardRequest::generateMailInfo() {
  
  _serializer.startElement("Source");
  _serializer.writeElement("LongId", _mailInfomation.source.longId);
  _serializer.endElement(); // end element for Source
  
}