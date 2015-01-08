//
//  SASPingResponse.cpp
//  SlimEAS
//
//  Created by Focus Lan on 1/8/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASPingResponse.h"

#include <iostream>
#include <libxml/xmlreader.h>

using namespace SlimEAS;
using namespace std;

SASPingResponse::SASPingResponse(SASHTTPRequest::SASHTTPResponseContext &ctx)
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
        if (strcmp((const char *)curr_elm, "Status") == 0) {
          ToI32(_pingStatus);
        } else if (strcmp((const char *)curr_elm, "HeartbeatInterval") == 0) {
          ToI32(_heartbeatInterval);
        } else if (strcmp((const char *)curr_elm, "Folder") == 0) {
          _changedFolderIds.push_back(ToSTR(value));
        }
        
      } else if (n_type == XNT_EndElement) {
        curr_elm = NULL;
      } else {
        continue;
      }
    }
    
    xmlFreeTextReader(reader);
  } else {
    throw std::invalid_argument("create xml reader failed!");
  }
}

SASPingResponse::~SASPingResponse() {
  
}