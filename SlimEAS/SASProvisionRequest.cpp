//
//  SASProvisionRequest.cpp
//  SlimEAS
//
//  Created by envy.chen on 12/19/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#include "SASProvisionRequest.h"
#include "SASProvisionResponse.h"

#include <libxml/xmlwriter.h>
#include <iostream>

using namespace std;
using namespace SlimEAS;

#pragma mark - life cycle

SASProvisionRequest::SASProvisionRequest()
: SASCommandRequest()
{
  _command = "Provision";
}

SASProvisionRequest::~SASProvisionRequest() {
}

#pragma mark - override func

SASHTTPResponse *SASProvisionRequest::initialResponse() {
  return new SASProvisionResponse(_resContext);
}

void SASProvisionRequest::generateXMLPayload() {
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
  xmlTextWriterStartElement(writer, BAD_CAST "Provision");
  xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns", BAD_CAST "Provision:");
  if (!_isAcknowledgement) {
    xmlTextWriterWriteAttributeNS(writer, BAD_CAST "xmlns", BAD_CAST "settings", NULL, BAD_CAST "Settings:");
  }
  
  //remote wipe
  if (_isRemoteWipe) {
    xmlTextWriterStartElement(writer, BAD_CAST "RemoteWipe");
    xmlTextWriterWriteElement(writer, BAD_CAST "Status", BAD_CAST "1");
    xmlTextWriterEndElement(writer);
  }else {
    if (!_isAcknowledgement) {
      xmlTextWriterWriteRaw(writer, BAD_CAST _provisionDevice.payload().c_str());
    }
    xmlTextWriterStartElement(writer, BAD_CAST "Policies");
    xmlTextWriterStartElement(writer, BAD_CAST "Policy");
    xmlTextWriterWriteElement(writer, BAD_CAST "PolicyType", BAD_CAST "MS-EAS-Provisioning-WBXML");
    if (_isAcknowledgement) {
      xmlTextWriterWriteFormatElement(writer, BAD_CAST "PolicyKey", "%d", _policyKey);
      xmlTextWriterWriteFormatElement(writer, BAD_CAST "Status", "%d", _status);
    }
    
    xmlTextWriterEndElement(writer);
    xmlTextWriterEndElement(writer);
    xmlTextWriterEndElement(writer);
  }
  xmlTextWriterEndDocument(writer);

  _xmlPayload = string((char*)buf->content);
  
  std::cout << _xmlPayload;
  
  xmlFreeTextWriter(writer);
  xmlBufferFree(buf);
}



