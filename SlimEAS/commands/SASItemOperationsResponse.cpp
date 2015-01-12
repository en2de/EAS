//
//  SASItemOperationsResponse.cpp
//  SlimEAS
//
//  Created by Focus Lan on 1/6/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASItemOperationsResponse.h"
#include "SASMail.h"

#include <libxml/xmlreader.h>

using namespace SlimEAS;
using namespace std;

SASItemOperationsResponse::SASItemOperationsResponse(SASHTTPRequest::SASHTTPResponseContext &ctx)
: SASCommandResponse(ctx)
{
  xmlTextReaderPtr reader;
  
  uint8_t *buf = (uint8_t *)_xmlResponse.c_str();
  int buf_len = (int)strlen((const char *)buf);
  
  reader = xmlReaderForMemory((const char *)buf, buf_len, NULL, "utf-8", 0);
  if (reader != NULL) {
    const xmlChar *curr_elm = nullptr;
    const xmlChar *curr_elm_prefix = nullptr;
    const xmlChar *curr_ele_full = nullptr;
    int depth = 0;
    while (xmlTextReaderRead(reader)) {
      int n_type = xmlTextReaderNodeType(reader);
      if (n_type == XNT_Element) {
        depth = xmlTextReaderDepth(reader);
        curr_ele_full = xmlTextReaderConstName(reader);
        curr_elm = xmlTextReaderConstLocalName(reader);
        curr_elm_prefix = xmlTextReaderConstPrefix(reader);
        
        if (strcmp((const char *)curr_elm, "Properties") == 0) {
          string properties = string((const char*)xmlTextReaderReadOuterXml(reader));
          _mail.decode(properties);
          xmlTextReaderNext(reader);
        }
        printf("current element %s \n", curr_ele_full);
        continue;
      } else if (n_type == XNT_Text && curr_elm != NULL) {
        
#define ToI32(v) sscanf((const char *)value, "%d", &v)
#define ToSTR(v) (string((const char*)v))
        
        const xmlChar *value = xmlTextReaderConstValue(reader);
        if (strcmp((const char *)curr_elm, "Status") == 0 && depth == 0) {
          sscanf((const char *)value, "%d", &_status);
        } else if (depth == 3) {
          if (strcmp((const char *)curr_elm, "Status") == 0) {
            ToI32(_fetchResponse.status);
          } else if (strcmp((const char *)curr_elm, "CollectionId") == 0) {
            _fetchResponse.collectionId = ToSTR(value);
          } else if (strcmp((const char *)curr_elm, "ServerId") == 0) {
            _fetchResponse.serverId = ToSTR(value);
          } else if (strcmp((const char *)curr_elm, "Class") == 0) {
            _fetchResponse.className = ToSTR(value);
          } else if (strcmp((const char *)curr_elm, "FileReference") == 0) {
            _fetchResponse.fileReference = ToSTR(value);
          }
        }
      } else if (n_type == XNT_EndElement) {
        curr_elm = NULL;
      } else {
        continue;
      }
    }
    
    // we got a file
    if (!_fetchResponse.fileReference.empty()) {
      if (_mail.hasAttachment()) {
        vector<SASAttachment*> attchments = _mail.attachments();
        attchments[0]->setFileReference(_fetchResponse.fileReference);
        
      }
    }
    
    xmlFreeTextReader(reader);
  } else {
    throw std::invalid_argument("create xml reader failed!");
  }
}

SASItemOperationsResponse::~SASItemOperationsResponse() {
  
}