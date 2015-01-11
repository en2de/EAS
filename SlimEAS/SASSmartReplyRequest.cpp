//
//  SASSmartReplyRequest.cpp
//  SlimEAS
//
//  Created by Focus Lan on 1/8/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASSmartReplyRequest.h"

using namespace SlimEAS;
using namespace std;

SASSmartReplyRequest::SASSmartReplyRequest()
: SASMailBaseRequest()
{
  _command = CMD_SMARTREPLY;
}

SASSmartReplyRequest::~SASSmartReplyRequest() {

}

void SASSmartReplyRequest::generateMailInfo() {
  
  _serializer.startElement("Source");
  _serializer.writeElement("FolderId", _mailInfomation.source.folderId);
  _serializer.writeElement("ItemId", _mailInfomation.source.itemId);
  _serializer.endElement(); // end element for Source
  
}