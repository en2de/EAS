//
//  SASFetchEmailRequest.cpp
//  SlimCore
//
//  Created by Focus Lan on 1/13/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASFetchEmailRequest.h"

using namespace SlimEAS;
using namespace std;

SASFetchEmailRequest::SASFetchEmailRequest()
: SASItemOperationsRequest()
{
}

SASFetchEmailRequest::~SASFetchEmailRequest() {
}

void SASFetchEmailRequest::generateXMLSubPayload() {

  _serializer.startElement("Fetch");
  
  _serializer.writeElement("Store", _store);
  _serializer.writeElement("CollectionId", _collectionId);
  _serializer.writeElement("ServerId", _serverId);
  
  _serializer.startElement("Options");
  
  if (_options.mimeSupportLevel == SendMimeForSMime) {
    // const std::string &name, const std::string &prefix, const std::string &space, const char *format, ...
    _serializer.writeFormatElementNS("MIMESupport", "airsyncbase", "",  "%d", _options.mimeSupportLevel);
  }
  
  if (_options.hasBodyPreferences()) {
    _serializer.writeRaw(SASFolder::generateXmlForPreference(_options.bodyPreference(), "airsyncbase", "BodyPreference"));
  } else {
    printf("\nno body preference provided\n");
  }
  
  _serializer.endElement(); // end element for Options
  
  _serializer.endElement(); // end element for Fetch
  
}