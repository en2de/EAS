//
//  SASSyncResponse.cpp
//  SlimEAS
//
//  Created by Focus Lan on 1/6/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASSyncResponse.h"
#include "SASCollection.h"

#include <iostream>
#include <libxml/xmlreader.h>

using namespace std;
using namespace SlimEAS;

SASSyncResponse::SASSyncResponse(SASHTTPRequest::SASHTTPResponseContext &ctx)
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
        
        if (strcmp((const char *)curr_elm, "Collection") == 0) {
          string xml = string((const char*)xmlTextReaderReadOuterXml(reader));
          SASCollection collection;
          collection.decode(xml);
          collections.push_back(collection);
          xmlTextReaderNext(reader);
        }
        printf("current element %s \n", curr_ele_full);
        continue;
      } else if (n_type == XNT_Text && curr_elm != NULL) {
        
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

SASSyncResponse::~SASSyncResponse() {
  
}