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
  _command = "Ping";
}

SASPingRequest::~SASPingRequest() {
}

SASHTTPResponse *SASPingRequest::initialResponse() {
  return new SASPingResponse(_resContext);
}

void SASPingRequest::generateXMLPayload() {
  
  xmlBufferPtr buf;
  buf = xmlBufferCreate();
  if (buf == NULL) {
    throw std::invalid_argument("Error creating the xml buffer");
  }
  
  xmlTextWriterPtr writer;
  writer = xmlNewTextWriterMemory(buf, 0);
  xmlTextWriterSetIndent(writer, 1);
  if (writer == NULL) {
    throw std::invalid_argument("Error creating the xml writer");
  }
  
  xmlTextWriterStartDocument(writer, "1.0", "utf-8", NULL);
  xmlTextWriterWriteDTD(writer, BAD_CAST "ActiveSync", BAD_CAST "-//MICROSOFT//DTD ActiveSync//EN", BAD_CAST "http://www.microsoft.com/", NULL);
  xmlTextWriterStartElement(writer, BAD_CAST "Ping");
  xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns", BAD_CAST "Ping");
  
  xmlTextWriterWriteElement(writer, BAD_CAST "HeartbeatInterval", BAD_CAST to_string(_heartbeatInterval).c_str());
  xmlTextWriterStartElement(writer, BAD_CAST "Folders");
  
  for(auto &it : _simpleFolderList) {
    xmlTextWriterStartElement(writer, BAD_CAST "Folder");
    xmlTextWriterWriteElement(writer, BAD_CAST "Id", BAD_CAST it.folderId.c_str());
    xmlTextWriterWriteElement(writer, BAD_CAST "Class", BAD_CAST it.className.c_str());
  }
  
  xmlTextWriterEndElement(writer);
  
  xmlTextWriterEndElement(writer);
  
  xmlTextWriterEndDocument(writer); // end element for Ping
  
  _xmlPayload = string((char*)buf->content);
  
  printf("sendmail payload: \n%s\n", _xmlPayload.c_str());
  
  xmlFreeTextWriter(writer);
  xmlBufferFree(buf);
}