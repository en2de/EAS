//
//  SASSyncRequest.cpp
//  SlimEAS
//
//  Created by guo gloria on 15/1/2.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASSyncRequest.h"
#include "SASSyncResponse.h"

#include <iostream>
#include <libxml/xmlreader.h>

using namespace SlimEAS;
using namespace std;

static const int FOLDER_LIST_COUNT = 15;

SASSyncRequest::SASSyncRequest()
: _folderList(FOLDER_LIST_COUNT)
{
  _command = "Sync";
}

SASSyncRequest::~SASSyncRequest()
{
}

void SASSyncRequest::generateXMLPayload()
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
  
  xmlTextWriterStartDocument(writer, "1.0", "utf-8", nullptr);
  xmlTextWriterWriteDTD(writer, BAD_CAST "ActiveSync", BAD_CAST "-/MICROSOFT/DTD ActiveSync/EN", BAD_CAST "http://www.microsoft.com/", nullptr);
  
  const xmlChar* xmlns = BAD_CAST "airSync";
  const xmlChar* namespaceURI = BAD_CAST "AirSync";
  
  xmlTextWriterStartElementNS(writer, xmlns, BAD_CAST "Sync", namespaceURI);
  
  if (_folderList.size() == 0 && _isPartial == false) {
    throw std::invalid_argument("Sync request must specify collections or include the Partial element.");
  }
  
  if (_action == Synchronizing) {

    xmlTextWriterStartElement(writer, BAD_CAST "Collections");

    for (auto folder: _folderList) {
      if (folder == nullptr)
        continue;
      
      xmlTextWriterStartElement(writer, BAD_CAST "Collection");
      xmlTextWriterWriteElement(writer, BAD_CAST "SyncKey", BAD_CAST folder->syncKey().c_str());
      xmlTextWriterWriteElement(writer, BAD_CAST "CollectionId", BAD_CAST folder->id().c_str());
      
      if (folder->areDeletesPermanent()) {
        xmlTextWriterWriteElement(writer, BAD_CAST "DeletesAsMoves", BAD_CAST "0");
      } else {
        xmlTextWriterStartElement(writer, BAD_CAST "DeletesAsMoves");
        xmlTextWriterEndElement(writer);
      }
      
      if (folder->areChangesIgnored()) {
        xmlTextWriterWriteElement(writer, BAD_CAST "GetChanges", BAD_CAST "0");
      } else {
        xmlTextWriterStartElement(writer, BAD_CAST "GetChanges");
        xmlTextWriterEndElement(writer);
      }
      
      if (folder->windowSize() > 0) {
        xmlTextWriterWriteElement(writer, BAD_CAST "WindowSize", BAD_CAST to_string(folder->windowSize()).c_str());
      }
      
      if (folder->useConversationMode()) {
        xmlTextWriterWriteElement(writer, BAD_CAST "ConversationMode", BAD_CAST "1");
      }
      
      if (folder->hasOptions()) {
        std::string options = folder->generateOptionsXml();
        xmlTextWriterWriteRaw(writer, BAD_CAST options.c_str());
      }

      xmlTextWriterEndElement(writer);
    }
  } else if (_action == Fetching) {
    xmlTextWriterStartElement(writer, BAD_CAST "Commands");
    
    xmlTextWriterStartElement(writer, BAD_CAST "Fetch");
    
    xmlTextWriterWriteElement(writer, BAD_CAST "ServerId", BAD_CAST "1:150");
    
    xmlTextWriterEndElement(writer);
    
    xmlTextWriterEndElement(writer);
  }
  
  if (_wait > 0) {
    xmlTextWriterWriteElement(writer, BAD_CAST "Wait", BAD_CAST std::to_string(_wait).c_str());
  }
  
  if (_heartBeatInterval > 0) {
    xmlTextWriterWriteElement(writer, BAD_CAST "HearbeatInterval", BAD_CAST std::to_string(_heartBeatInterval).c_str());
  }
  
  if (_windowSize > 0) {
    xmlTextWriterWriteElement(writer, BAD_CAST "WindowSize", BAD_CAST std::to_string(_windowSize).c_str());
  }
  
  if (_isPartial) {
    xmlTextWriterWriteElement(writer, BAD_CAST "Partial", NULL);
  }
  
  xmlTextWriterEndElement(writer);
  xmlTextWriterEndElement(writer);
  
  xmlTextWriterEndDocument(writer);
  
  _xmlPayload = string((char*)buf->content);
  
  cout << _xmlPayload;
  
  xmlFreeTextWriter(writer);
  xmlBufferFree(buf);

}

SASHTTPResponse *SASSyncRequest::initialResponse()
{
  return new SASSyncResponse(_resContext);
}