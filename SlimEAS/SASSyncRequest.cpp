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

using namespace SlimEAS;
using namespace std;

static const int FOLDER_LIST_COUNT = 15;

SASSyncRequest::SASSyncRequest(const std::string &server, const std::string &user, const std::string &password, bool useSSL)
: SASCommandRequest(server, user, password, useSSL)
{
  _command = CMD_SYNC;
}

SASSyncRequest::SASSyncRequest()
: _folderList(FOLDER_LIST_COUNT)
{
  _command = CMD_SYNC;
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
  _serializer.start();
  
  string xmlns = "airsync";
  string namespaceURI = "AirSync";

  _serializer.startElementNS("Sync", xmlns, namespaceURI);
  
  _serializer.writeAttributeNS(namespaceURI, xmlns, "xmlns");
  
  _serializer.writeAttributeNS("airsyncbase", "AirSyncBase", "xmlns");
  _serializer.writeAttributeNS("email", "Email", "xmlns");
  
  if (_action == AddContact) {
    _serializer.writeAttributeNS("contacts", "Contacts", "xmlns");
  }
  
  if (_folderList.size() == 0 && _isPartial == false) {
    throw std::invalid_argument("Sync request must specify collections or include the Partial element.");
  }

  _serializer.startElement("Collections");

  for (auto folder: _folderList) {
    
    if (folder == nullptr)
      continue;
    
    if (_action == Synchronize) {
    
      _serializer.startElement("Collection");
      _serializer.writeElement("Class", CLASS_EMAIL); // TODO: "Email" as default. should be a parameter.
      _serializer.writeElement("SyncKey", folder->syncKey());
      _serializer.writeElement("CollectionId", folder->id());
      
      if (folder->areDeletesPermanent()) {
        _serializer.writeElement("DeletesAsMoves", "0");
      } else {
        _serializer.writeEmptyElement("DeletesAsMoves");
      }
      
      if (folder->areChangesIgnored()) {
        _serializer.writeElement("GetChanges", "0");
      } else {
        _serializer.writeEmptyElement("GetChanges");
      }
      
      if (folder->windowSize() > 0) {
        _serializer.writeFormatElement("WindowSize", "%d", folder->windowSize());
      }
      
      if (folder->useConversationMode()) {
        _serializer.writeElement("ConversationMode", "1");
      }
      
      if (folder->hasOptions()) {
        std::string options = folder->generateOptionsXml();
        _serializer.writeRaw(options);
      }

      _serializer.endElement();
    
    } else if (_action == Fetch) {
      
      _serializer.startElement("Commands");
      
      _serializer.startElement("Fetch");
      
      _serializer.writeElement("ServerId", folder->id());
      
      _serializer.endElement();
      
      _serializer.endElement();
      
    } else if (_action == AddContact) { // TODO: need test case for this action, UNFINISHED
      
      _serializer.startElement("Collection");
      
      _serializer.writeElement("SyncKey", folder->syncKey());
      _serializer.writeElement("CollectionId", folder->id());
      
      _serializer.startElement("Commands");
      
      _serializer.startElement("Add");
      
      _serializer.writeElement("ClientId", "633916348086136194"); // test ClientId
      
      string applicationData =
      "<ApplicationData>\
        <contacts:Email1Address>focus@slim.so</contacts:Email1Address>\
        <contacts:FirstName>Focus</contacts:FirstName> \
        <contacts:MiddleName>W</contacts:MiddleName> \
        <contacts:LastName>Lan</contacts:LastName> \
        <contacts:Title>Sr Marketing Manager</contacts:Title>\
      </ApplicationData>";
      
      _serializer.writeRaw(applicationData);
      
      _serializer.endElement(); // end element for Collection
      
      _serializer.endElement(); // end element for Commands
      
      _serializer.endElement(); // end element for Add
    }
  }
  
  if (_wait > 0) {
    _serializer.writeFormatElement("Wait", "%d", _wait);
  }
  
  if (_heartBeatInterval > 0) {
    _serializer.writeFormatElement("HearbeatInterval", "%d", _heartBeatInterval);
  }
  
  // TODO: need to check this element.
  if (_windowSize > 0) {
    _serializer.writeFormatElement("WindowSize", "%d", _windowSize);
  }
  
  if (_isPartial) {
    _serializer.writeEmptyElement("Partial");
  }
  
  _serializer.endElement();
  _serializer.endElement();
  
  _serializer.endElement();
  
  _serializer.done();
  
  _xmlPayload = _serializer.outerXml();

}

SASHTTPResponse *SASSyncRequest::initialResponse()
{
  return new SASSyncResponse(_resContext);
}