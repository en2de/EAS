//
//  SASGetAttachmentRequest.cpp
//  SlimCore
//
//  Created by Focus Lan on 1/13/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASFetchAttachmentRequest.h"

using namespace SlimEAS;
using namespace std;

SASFetchAttachmentRequest::SASFetchAttachmentRequest()
: SASItemOperationsRequest()
{
}

SASFetchAttachmentRequest::~SASFetchAttachmentRequest() {
}

void SASFetchAttachmentRequest::generateXMLSubPayload() {
  
  _serializer.startElement("Fetch");
  
  _serializer.writeElement("Store", _store);
  
  if (_fileReference.empty()) {
    printf("\nERROR!!! if you want to fetch file, please provide fileReference param!!!\n");
  }
  
  _serializer.writeElement("FileReference",  _fileReference);
  
  _serializer.endElement(); // end element for Fetch
  
}