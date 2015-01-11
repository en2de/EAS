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
  
  _serializer.startElement("Fetch");
  
  _serializer.writeElement("Store", _store);
  _serializer.writeElement("CollectionId", _collectionId);
  _serializer.writeElement("ServerId", _serverId);
  
  if (_fetchProfile == FileItem) {
    
    if (_fileReference.empty()) {
      printf("\nERROR!!! if you want to fetch file, please provide fileReference param!!!\n");
    }
    _serializer.writeElement("FileReference",  _fileReference);
  }
  
  _serializer.startElement("Options");
  
  if (_fetchProfile == EmailItem) {
    if (_options.hasBodyPreferences()) {
      _serializer.writeRaw(SASFolder::generateXmlForPreference(_options.bodyPreference(), "airsyncbase", "BodyPreference"));
    } else {
      printf("\n!!!no body preference provided!!!\n");
    }
  } else if (_fetchProfile == MimeEmailItem) {
    _serializer.writeElementNS("MIMESupport", "1", "airsync");
    _serializer.writeElementNS("BodyPreference", "1", "airsyncbase");
    _serializer.writeElementNS("Type", "1", "4");
    _serializer.endElement(); // end element for BodyPreference
  }
  
  _serializer.endElement();
  
  _serializer.endElement(); // end element for Fetch
  
  _serializer.endElement(); // end element for ItemOperations
  _serializer.done(); // end element for doc
  
  _xmlPayload = _serializer.outerXml();
  
  cout << _xmlPayload << endl;
}

SASHTTPResponse *SASItemOperationsRequest::initialResponse() {
  return new SASItemOperationsResponse(_resContext);;
}