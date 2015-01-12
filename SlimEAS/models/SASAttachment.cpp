//
//  SASAttachment.cpp
//  SlimEAS
//
//  Created by Focus Lan on 1/4/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASAttachment.h"

#include <libxml/xmlwriter.h>
#include <libxml/xmlsave.h>
#include <libxml/xmlreader.h>

using namespace std;
using namespace SlimEAS;

static const char *_prefix = "airsyncbase";

SASAttachment::SASAttachment()
: SASBaseEntity()
{
  
}

SASAttachment::~SASAttachment() {
  
}

void SASAttachment::decode(const std::string &xml) {
  xmlTextReaderPtr reader;
  uint8_t *buf = (uint8_t *)xml.c_str();
  int buf_len = (int)strlen((const char *)buf);
  
  reader = xmlReaderForMemory((const char *)buf, buf_len, nullptr, "utf-8", 0);
  if (reader != NULL) {
    const xmlChar *curr_elm = nullptr;
    const xmlChar *curr_elm_prefix = nullptr;
    const xmlChar *curr_ele_full = nullptr;
    while (xmlTextReaderRead(reader)) {
      int n_type = xmlTextReaderNodeType(reader);
      if (n_type == XNT_Element) {
        curr_ele_full = xmlTextReaderConstName(reader);
        curr_elm = xmlTextReaderConstLocalName(reader);
        curr_elm_prefix = xmlTextReaderConstPrefix(reader);
        continue;
      } else if (n_type == XNT_Text && curr_elm != NULL) {

#define ToI32(v) sscanf((const char *)value, "%d", &v)
#define ToSTR(v) (string((const char*)v))
        
        const xmlChar *value = xmlTextReaderConstValue(reader);
        if (strcmp((const char *)curr_elm, "DisplayName") == 0) {
          _displayName = ToSTR(value);
        }else if (strcmp((const char *)curr_elm, "FileReference") == 0) {
          _fileReference = ToSTR(value);
        } else if(strcmp((const char *)curr_elm, "Method") == 0 ) {
          _method = ToSTR(value);
        } else if(strcmp((const char *)curr_elm, "EstimatedDataSize") == 0 ){
          ToI32(_estimatedDataSize);
        } else {
          // other property
        }
      } else if (n_type == XNT_EndElement) {
        curr_elm = nullptr;
      } else {
        continue;
      }
    }
    xmlFreeTextReader(reader);
  }
}

const string SASAttachment::encode() {
  xmlBufferPtr buf;
  buf = xmlBufferCreate();
  if (buf == NULL) {
    throw std::invalid_argument("Error creating the xml buffer");
  }
  
  xmlDocPtr doc;
  
  xmlTextWriterPtr writer;
  writer = xmlNewTextWriterDoc(&doc, 0);
  //  writer = xmlNewTextWriterMemory(buf, 0);
  xmlTextWriterSetIndent(writer, 1);
  if (writer == NULL) {
    throw std::invalid_argument("Error creating the xml writer");
  }
  
  xmlTextWriterStartDocument(writer, nullptr, nullptr, nullptr);
  xmlTextWriterStartElementNS(writer, BAD_CAST _prefix, BAD_CAST "Attachment", nullptr);
  
  if (!_displayName.empty()) {
    xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST "DisplayName", nullptr, BAD_CAST _displayName.c_str());
  }
  
  if (!_fileReference.empty()) {
    xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST "FileReference", nullptr, BAD_CAST _fileReference.c_str());
  }
  
  if (!_method.empty()) {
    xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST "Method", nullptr, BAD_CAST _method.c_str());
  }
  
  xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST "EstimatedDataSize", nullptr, BAD_CAST to_string(_estimatedDataSize).c_str());
  
  xmlTextWriterEndElement(writer);
  xmlTextWriterEndDocument(writer);
  
  xmlSaveCtxtPtr saveCtx = xmlSaveToBuffer(buf, "utf-8", XML_SAVE_NO_DECL | XML_SAVE_FORMAT);
  xmlSaveDoc(saveCtx, doc);
  xmlSaveFlush(saveCtx);
  xmlSaveClose(saveCtx);
  
  string xml((char *)buf->content);
  
  xmlFreeTextWriter(writer);
  xmlFreeDoc(doc);
  xmlBufferFree(buf);
  
  return xml;
}