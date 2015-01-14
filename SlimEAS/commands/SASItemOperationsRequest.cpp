//
//  SASItemOperationsRequest.cpp
//  SlimEAS
//
//  Created by Focus Lan on 1/6/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASItemOperationsRequest.h"
#include "SASItemOperationsResponse.h"

#include <iostream>
#include <libxml/xmlwriter.h>

using namespace std;
using namespace SlimEAS;

SASItemOperationsRequest::SASItemOperationsRequest()
: SASCommandRequest()
{
  _command = CMD_ITEMOPERATIONS;
}

SASItemOperationsRequest::~SASItemOperationsRequest() {

}

void SASItemOperationsRequest::generateXMLPayload() {
  
  _serializer.start();
  
  _serializer.startElement(_command);
  _serializer.writeAttribute("xmlns", "ItemOperations");
  _serializer.writeAttributeNS("airsync", "AirSync", "xmlns");
  _serializer.writeAttributeNS("airsyncbase", "AirSyncBase", "xmlns");
  
  generateXMLSubPayload();
  
  _serializer.endElement(); // end element for ItemOperations
  _serializer.done(); // end element for doc
  
  _xmlPayload = _serializer.outerXml();
  
  cout << _xmlPayload << endl;
}

SASHTTPResponse *SASItemOperationsRequest::initialResponse() {
  return new SASItemOperationsResponse(_resContext);;
}