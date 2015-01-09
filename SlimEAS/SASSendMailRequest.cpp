//
//  SASSendMailRequest.cpp
//  SlimEAS
//
//  Created by Focus Lan on 1/7/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASSendMailRequest.h"
#include "SASSendMailResponse.h"
#include "SASWBXml.h"

#include <libxml/xmlwriter.h>

using namespace SlimEAS;
using namespace std;

SASSendMailRequest::SASSendMailRequest()
: SASCommandRequest()
{
  _command = "SendMail";
}

SASSendMailRequest::~SASSendMailRequest() {
}

SASHTTPResponse *SASSendMailRequest::initialResponse() {
  return new SASSendMailResponse(_resContext);
}

void SASSendMailRequest::generateXMLPayload() {
  
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
  xmlTextWriterStartElement(writer, BAD_CAST "SendMail");
  xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns", BAD_CAST "ComposeMail");

  xmlTextWriterWriteElement(writer, BAD_CAST "ClientId", BAD_CAST "633916348086136194");
  xmlTextWriterStartElement(writer, BAD_CAST "SaveInSentItems");
  xmlTextWriterEndElement(writer);
  
  string mimeData = _mail.asMIMEString();
  
  // 1. Take care of this Element, it must be "MIME", not "Mime" or others.
  // 2. Must be CDATA element.
  // 3. WBXML lib will take  care of it and encode it as base64.
  xmlTextWriterStartElement(writer, BAD_CAST "MIME");
  xmlTextWriterWriteCDATA(writer, BAD_CAST mimeData.c_str());
  xmlTextWriterEndElement(writer);

  xmlTextWriterEndDocument(writer); // end element for SendMail
  
  _xmlPayload = string((char*)buf->content);
  
  printf("sendmail payload: \n%s\n", _xmlPayload.c_str());
  
  xmlFreeTextWriter(writer);
  xmlBufferFree(buf);
}