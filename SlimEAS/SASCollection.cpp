//
//  SASCollection.cpp
//  SlimEAS
//
//  Created by Focus Lan on 1/7/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASCollection.h"

#include <libxml/xmlreader.h>

using namespace SlimEAS;
using namespace std;

#pragma SASCommand implement

SASCommand::SASCommand() {
  
}

// haven't implemented yet
const string SASCommand::encode() {
  return "";
}

void SASCommand::decode(const std::string &xml) {
  xmlTextReaderPtr reader;
  uint8_t *buf = (uint8_t *)xml.c_str();
  int buf_len = (int)strlen((const char *)buf);
  
  reader = xmlReaderForMemory((const char *)buf, buf_len, nullptr, "utf-8", 0);
  if (reader != NULL) {
    const xmlChar *curr_elm = nullptr;
    const xmlChar *curr_elm_prefix = nullptr;
    const xmlChar *curr_ele_full = nullptr;
    int depth = -1;
    
#define ToSTR(v) (string((const char*)v))
    
    while (xmlTextReaderRead(reader)) {
      int n_type = xmlTextReaderNodeType(reader);
      if (n_type == XNT_Element) {
        curr_ele_full = xmlTextReaderConstName(reader);
        curr_elm = xmlTextReaderConstLocalName(reader);
        curr_elm_prefix = xmlTextReaderConstPrefix(reader);
        depth = xmlTextReaderDepth(reader);
        if (depth == 0) {
          _name = ToSTR(curr_elm); // top level element name
        }
        if (strcmp((const char *)curr_elm, "ApplicationData") == 0) {
          string xml = string((const char*)xmlTextReaderReadOuterXml(reader));
          _applicationData.decode(xml);
          xmlTextReaderNext(reader);
        }
        continue;
      } else if (n_type == XNT_Text && curr_elm != NULL) {
      
        const xmlChar *value = xmlTextReaderConstValue(reader);
        if (strcmp((const char *)curr_elm, "ServerId") == 0) {
          _serverId = ToSTR(value);
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

SASCommand::~SASCommand() {
  
}


#pragma SASCollection implement

SASCollection::SASCollection() {

}

const string SASCollection::encode() {
  return "";
}

void SASCollection::decode(const std::string &xml) {

  xmlTextReaderPtr reader;
  uint8_t *buf = (uint8_t *)xml.c_str();
  int buf_len = (int)strlen((const char *)buf);
  
  reader = xmlReaderForMemory((const char *)buf, buf_len, nullptr, "utf-8", 0);
  if (reader != NULL) {
    const xmlChar *curr_elm = nullptr;
    const xmlChar *curr_elm_prefix = nullptr;
    const xmlChar *curr_ele_full = nullptr;
    int depth = -1;
    
#define ToI32(v) sscanf((const char *)value, "%d", &v)
#define ToSTR(v) (string((const char*)v))
    
    while (xmlTextReaderRead(reader)) {
      int n_type = xmlTextReaderNodeType(reader);
      if (n_type == XNT_Element) {
        curr_ele_full = xmlTextReaderConstName(reader);
        curr_elm = xmlTextReaderConstLocalName(reader);
        curr_elm_prefix = xmlTextReaderConstPrefix(reader);
        depth = xmlTextReaderDepth(reader);
        if (strcmp((const char *)curr_elm, "Add") == 0) {
          string xml = string((const char*)xmlTextReaderReadOuterXml(reader));
          SASCommand *cmd = new SASCommand();
          cmd->decode(xml);
          _commandCollection.push_back(cmd);
          xmlTextReaderNext(reader);
        } else if (strcmp((const char *)curr_elm, "MoreAvailable") == 0) {
          _moreAvailable = true;
        }
        continue;
      } else if (n_type == XNT_Text && curr_elm != NULL) {
        
        const xmlChar *value = xmlTextReaderConstValue(reader);
        if (strcmp((const char *)curr_elm, "Class") == 0) {
          _className = ToSTR(value);
        } if (strcmp((const char *)curr_elm, "SyncKey") == 0) {
          _syncKey = ToSTR(value);
        } if (strcmp((const char *)curr_elm, "CollectionId") == 0) {
          _collectionId = ToSTR(value);
        } if (strcmp((const char *)curr_elm, "Status") == 0) {
          ToI32(_status);
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

SASCollection::~SASCollection() {

}