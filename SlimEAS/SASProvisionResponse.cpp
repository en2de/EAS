//
//  SASProvisionResponse.cpp
//  SlimEAS
//
//  Created by envy.chen on 12/19/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#include "SASProvisionResponse.h"

#include <iostream>
#include <libxml/xmlreader.h>

using namespace SlimEAS;

SASProvisionResponse::SASProvisionResponse(SASHTTPRequest::SASHTTPResponseContext &ctx)
: SASCommandResponse(ctx)
{
  xmlTextReaderPtr reader;
  std::cout << _xmlResponse;
  
  _isPolicyLoaded = _policy.loadXml(_xmlResponse);
  
  uint8_t *buf = (uint8_t *)_xmlResponse.c_str();
  int buf_len = (int)strlen((const char *)buf);
  
  reader = xmlReaderForMemory((const char *)buf, buf_len, NULL, "utf-8", 0);
  if (reader != NULL) {
    const xmlChar *curr_elm = NULL;
    while (xmlTextReaderRead(reader)) {
      int n_type = xmlTextReaderNodeType(reader);
      if (n_type == XNT_Element) {
        curr_elm = xmlTextReaderConstName(reader);
        continue;
      } else if (n_type == XNT_Text && curr_elm != NULL) {
        const xmlChar *value = xmlTextReaderConstValue(reader);
        if (strcmp((const char *)curr_elm, "Status")) {
          sscanf((const char *)value, "%d", &_status);
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

SASProvisionResponse::~SASProvisionResponse() {
  
}