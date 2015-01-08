//
//  SASSmartReplyRequest.cpp
//  SlimEAS
//
//  Created by Focus Lan on 1/8/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASSmartReplyRequest.h"
#include "SASSendMailResponse.h"

#include <libxml/xmlwriter.h>

using namespace SlimEAS;
using namespace std;

SASSmartReplyRequest::SASSmartReplyRequest()
: SASCommandRequest()
{
  _command = "SmartReply";
}

SASSmartReplyRequest::~SASSmartReplyRequest() {
}

SASHTTPResponse *SASSmartReplyRequest::initialResponse() {
  return new SASSendMailResponse(_resContext);
}

void SASSmartReplyRequest::generateXMLPayload() {
  
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
  xmlTextWriterStartElement(writer, BAD_CAST "SmartReply");
  xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns", BAD_CAST "ComposeMail");
  
  xmlTextWriterWriteElement(writer, BAD_CAST "ClientId", BAD_CAST "E61068AD-10AE-4819-993B-C50B2FB0714E");
  
  xmlTextWriterStartElement(writer, BAD_CAST "SaveInSentItems");
  xmlTextWriterEndElement(writer); // end element for SaveInSentItems
  
  xmlTextWriterStartElement(writer, BAD_CAST "Source");
  xmlTextWriterWriteElement(writer, BAD_CAST "FolderId", BAD_CAST "1");
  xmlTextWriterWriteElement(writer, BAD_CAST "ItemId", BAD_CAST "1:5");
  xmlTextWriterEndElement(writer); // end element for Source
  
  string mimeData = _mail.asMIMEData();
  
  // 1. Take care of this Element, it must be "MIME", not "Mime" or others.
  // 2. Must be CDATA element.
  // 3. WBXML lib will take  care of it and encode it as base64.
  xmlTextWriterStartElement(writer, BAD_CAST "MIME");
  xmlTextWriterWriteCDATA(writer, BAD_CAST mimeData.c_str());
  xmlTextWriterEndElement(writer); // end element for Mime
  
  xmlTextWriterEndDocument(writer); // end element for SendMail
  
  _xmlPayload = string((char*)buf->content);
  
  printf("SmartReply payload: \n%s\n", _xmlPayload.c_str());
  
  xmlFreeTextWriter(writer);
  xmlBufferFree(buf);
}