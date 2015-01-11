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

using namespace SlimEAS;
using namespace std;

SASFolderSyncRequest::SASFolderSyncRequest(const std::string &server, const std::string &user, const std::string &password, bool useSSL)
: SASCommandRequest(server, user, password, useSSL)
{
  _command = CMD_FOLDERSYNC;
}

SASFolderSyncRequest::SASFolderSyncRequest()
{
  _command = CMD_FOLDERSYNC;
}

SASFolderSyncRequest::~SASFolderSyncRequest(){
}

void SASFolderSyncRequest::generateXMLPayload()
{
  string namespaceURI = "FolderHierarchy";
  
  _serializer.start();
  
  _serializer.startElement(_command);
  _serializer.writeAttribute("xmlns", namespaceURI);
  
  _serializer.writeElement("SyncKey", _syncKey);
  _serializer.endElement();
  
  _serializer.done();
  
  _xmlPayload = _serializer.outerXml();
  
  std::cout << _xmlPayload;

}

SASHTTPResponse *SASFolderSyncRequest::initialResponse()
{
  return new SASFolderSyncResponse(_resContext);
}