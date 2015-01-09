//
//  SASFolderSyncRequest.cpp
//  SlimEAS
//
//  Created by guo gloria on 15/1/2.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include <iostream>

#include "SASFolderSyncRequest.h"
#include "SASFolderSyncResponse.h"

#include "libxml/xmlwriter.h"

using namespace SlimEAS;

SASFolderSyncRequest::SASFolderSyncRequest(const std::string &server, const std::string &user, const std::string &password, bool useSSL)
: SASCommandRequest(server, user, password, useSSL)
{
  _command = "FolderSync";
}

SASFolderSyncRequest::SASFolderSyncRequest()
{
  _command = "FolderSync";
}

SASFolderSyncRequest::~SASFolderSyncRequest(){
}

void SASFolderSyncRequest::generateXMLPayload()
{
  xmlBufferPtr buf = xmlBufferCreate();
  if (buf == nullptr) {
    throw std::invalid_argument("Error creating the xml buffer");
  }
  
  xmlTextWriterPtr writer;
  writer = xmlNewTextWriterMemory(buf, 0);
  if (writer == nullptr) {
    throw std::invalid_argument("Error creating the xml writer");
  }
  xmlTextWriterSetIndent(writer, 1);
  
  xmlTextWriterStartDocument(writer, "1.0", "utf-8", NULL);
  xmlTextWriterWriteDTD(writer, BAD_CAST "ActiveSync", BAD_CAST "-/MICROSOFT/DTD ActiveSync/EN", BAD_CAST "http://www.microsoft.com/", NULL);
  
  const xmlChar* xmlns = BAD_CAST "folderhierarchy";
  const xmlChar* namespaceURI = BAD_CAST "FolderHierarchy";
  xmlTextWriterStartElementNS(writer, xmlns, BAD_CAST "FolderSync", namespaceURI);
  xmlTextWriterWriteElementNS(writer, xmlns, BAD_CAST "SyncKey", nullptr, BAD_CAST _syncKey.c_str());
  xmlTextWriterEndElement(writer);
  
  xmlTextWriterEndDocument(writer);
  
  _xmlPayload = std::string((char *)buf->content);
  
  std::cout << _xmlPayload;

}

SASHTTPResponse *SASFolderSyncRequest::initialResponse()
{
  return new SASFolderSyncResponse(_resContext);
}