//
//  SASSyncRequest.cpp
//  SlimEAS
//
//  Created by guo gloria on 15/1/2.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASSyncRequest.h"
#include "SASFolder.h"
#include "SASSyncResponse.h"

#include <iostream>
#include <libxml/xmlreader.h>

using namespace SlimEAS;
using namespace std;

static const int FOLDER_LIST_COUNT = 15;

SASSyncRequest::SASSyncRequest(const std::string &server, const std::string &user, const std::string &password, bool useSSL)
: SASCommandRequest(server, user, password, useSSL)
{
  _command = "Sync";
}

SASSyncRequest::SASSyncRequest()
: _folderList(FOLDER_LIST_COUNT)
{
  _command = "Sync";
}

SASSyncRequest::~SASSyncRequest()
{
  for (auto it : _folderList) {
    if (it != nullptr) {
      delete it;
    }
    it = nullptr;
  }
  
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
  
  xmlTextWriterWriteAttributeNS(writer, BAD_CAST "xmlns", BAD_CAST "airsync", nullptr, BAD_CAST "AirSync");
  xmlTextWriterWriteAttributeNS(writer, BAD_CAST "xmlns", BAD_CAST "airsyncbase", nullptr, BAD_CAST "AirSyncBase");
  xmlTextWriterWriteAttributeNS(writer, BAD_CAST "xmlns", BAD_CAST "email", nullptr, BAD_CAST "Email");
  
  if (_action == AddContact) {
    xmlTextWriterWriteAttributeNS(writer, BAD_CAST "xmlns", BAD_CAST "contacts", nullptr, BAD_CAST "Contacts");
  }
  
  if (_folderList.size() == 0 && _isPartial == false) {
    throw std::invalid_argument("Sync request must specify collections or include the Partial element.");
  }

  xmlTextWriterStartElement(writer, BAD_CAST "Collections");

  for (auto folder: _folderList) {
    
    if (folder == nullptr)
      continue;
    
    if (_action == Synchronize) {
    
      xmlTextWriterStartElement(writer, BAD_CAST "Collection");
      xmlTextWriterWriteElement(writer, BAD_CAST "Class", BAD_CAST "Email"); // TODO: "Email" as default. should be a parameter.
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
    
    } else if (_action == Fetch) {
      
      xmlTextWriterStartElement(writer, BAD_CAST "Commands");
      
      xmlTextWriterStartElement(writer, BAD_CAST "Fetch");
      
      xmlTextWriterWriteElement(writer, BAD_CAST "ServerId", BAD_CAST folder->id().c_str());
      
      xmlTextWriterEndElement(writer);
      
      xmlTextWriterEndElement(writer);
      
    } else if (_action == AddContact) { // TODO: need test case for this action, UNFINISHED
      
      xmlTextWriterStartElement(writer, BAD_CAST "Collection");
      
      xmlTextWriterWriteElement(writer, BAD_CAST "SyncKey", BAD_CAST folder->syncKey().c_str());
      xmlTextWriterWriteElement(writer, BAD_CAST "CollectionId", BAD_CAST folder->id().c_str());
      
      xmlTextWriterStartElement(writer, BAD_CAST "Commands");
      
      xmlTextWriterStartElement(writer, BAD_CAST "Add");
      
      xmlTextWriterWriteElement(writer, BAD_CAST "ClientId", BAD_CAST "633916348086136194"); // test ClientId
      
      string applicationData =
      "<ApplicationData>\
        <contacts:Email1Address>focus@slim.so</contacts:Email1Address>\
        <contacts:FirstName>Focus</contacts:FirstName> \
        <contacts:MiddleName>W</contacts:MiddleName> \
        <contacts:LastName>Lan</contacts:LastName> \
        <contacts:Title>Sr Marketing Manager</contacts:Title>\
      </ApplicationData>";
      
      xmlTextWriterWriteRaw(writer, BAD_CAST applicationData.c_str());
      
      xmlTextWriterEndElement(writer); // end element for Collection
      
      xmlTextWriterEndElement(writer); // end element for Commands
      
      xmlTextWriterEndElement(writer); // end element for Add
    }
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