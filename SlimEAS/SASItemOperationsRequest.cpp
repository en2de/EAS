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
  _command = "ItemOperations";
}

SASItemOperationsRequest::~SASItemOperationsRequest() {
}

void SASItemOperationsRequest::generateXMLPayload() {
  
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
  xmlTextWriterStartElement(writer, BAD_CAST "ItemOperations");
  xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns", BAD_CAST "ItemOperations");
  xmlTextWriterWriteAttributeNS(writer, BAD_CAST "xmlns", BAD_CAST "airsync", nullptr, BAD_CAST "AirSync");
  xmlTextWriterWriteAttributeNS(writer, BAD_CAST "xmlns", BAD_CAST "airsyncbase", nullptr, BAD_CAST "AirSyncBase");
  
  xmlTextWriterStartElement(writer, BAD_CAST "Fetch");
  
  xmlTextWriterWriteElement(writer, BAD_CAST "Store", BAD_CAST _store.c_str());
  xmlTextWriterWriteElement(writer, BAD_CAST "CollectionId", BAD_CAST _collectionId.c_str());
  xmlTextWriterWriteElement(writer, BAD_CAST "ServerId", BAD_CAST _serverId.c_str());
  
  if (_fetchProfile == FileItem) {
    
    if (_fileReference.empty()) {
      printf("\nERROR!!! if you want to fetch file, please provide fileReference param!!!\n");
    }
    xmlTextWriterWriteElement(writer, BAD_CAST "FileReference", BAD_CAST _fileReference.c_str());
  }
  
  xmlTextWriterStartElement(writer, BAD_CAST "Options");
  
  if (_fetchProfile == EmailItem) {
    if (_options.hasBodyPreferences()) {
      xmlTextWriterWriteRaw(writer, BAD_CAST SASFolder::generateXmlForPreference(_options.bodyPreference(), "airsyncbase", "BodyPreference").c_str());
    } else {
      printf("\n!!!no body preference provided!!!\n");
    }
  } else if (_fetchProfile == MimeEmailItem) {
    xmlTextWriterWriteElementNS(writer, BAD_CAST "airsync", BAD_CAST "MIMESupport", nullptr, BAD_CAST "1");
    xmlTextWriterStartElementNS(writer, BAD_CAST "airsyncbase", BAD_CAST "BodyPreference", nullptr);
    xmlTextWriterWriteElementNS(writer, BAD_CAST "airsyncbase", BAD_CAST "Type", nullptr, BAD_CAST "4");
    xmlTextWriterEndElement(writer); // end element for BodyPreference
  }
  
  xmlTextWriterEndElement(writer);
  
  xmlTextWriterEndElement(writer); // end element for Fetch
  
  xmlTextWriterEndElement(writer); // end element for ItemOperations
  xmlTextWriterEndDocument(writer); // end element for doc
  
  _xmlPayload = string((char*)buf->content);
  
  cout << _xmlPayload << endl;
  
  xmlFreeTextWriter(writer);
  xmlBufferFree(buf);
}

SASHTTPResponse *SASItemOperationsRequest::initialResponse() {
  return new SASItemOperationsResponse(_resContext);;
}