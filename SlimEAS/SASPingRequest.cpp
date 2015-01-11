//
//  SASPingRequest.cpp
//  SlimEAS
//
//  Created by Focus Lan on 1/8/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASPingRequest.h"
#include "SASPingResponse.h"

#include <libxml/xmlwriter.h>

using namespace SlimEAS;
using namespace std;

SASPingRequest::SASPingRequest()
: SASCommandRequest()
{
  _command = CMD_PING;
}

SASPingRequest::~SASPingRequest() {
}

SASHTTPResponse *SASPingRequest::initialResponse() {
  return new SASPingResponse(_resContext);
}

void SASPingRequest::generateXMLPayload() {
  
  _serializer.start();
  
  _serializer.startElement(_command);
  _serializer.writeAttribute("xmlns", "Ping");
  
  _serializer.writeFormatElement("HeartbeatInterval", "%d", _heartbeatInterval);
  _serializer.startElement("Folders");
  
  for(auto &it : _simpleFolderList) {
    _serializer.startElement("Folder");
    _serializer.writeElement("Id", it.folderId);
    _serializer.writeElement("Class", it.className);
    _serializer.endElement();
  }
  
  _serializer.endElement();
  
  _serializer.endElement();;
  
  _serializer.done(); // end element for Ping
  
  _xmlPayload = _serializer.outerXml();
  
  printf("sendmail payload: \n%s\n", _xmlPayload.c_str());
}