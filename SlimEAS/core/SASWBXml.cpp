//
//  SASWBXml.cpp
//  SlimEAS
//
//  Created by envy.chen on 12/14/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#include <stdlib.h>

#include <utility>
#include <string>

#include "SASWBXml.h"

using namespace SlimEAS;

SASWBXml::SASWBXml()
: _w2x(NULL),
  _x2w(NULL)
{
    
}

SASWBXml::~SASWBXml() {
}

string SASWBXml::toXml(unsigned char *wbxml, unsigned int wbxml_len){
  wbxml_conv_wbxml2xml_create(&_w2x);
  wbxml_conv_wbxml2xml_set_language(_w2x, WBXML_LANG_ACTIVESYNC);
  wbxml_conv_wbxml2xml_set_indent(_w2x, 0x02);
  
  WB_UTINY *xml;
  unsigned int xml_len;
  wbxml_conv_wbxml2xml_run(_w2x, wbxml, wbxml_len, &xml, &xml_len);
  
  string str;
  str.append((const char *)xml, xml_len);
  
  wbxml_conv_wbxml2xml_destroy(_w2x);
  _w2x = NULL;
  
  return str;
}

unsigned char *SASWBXml::toWBXml(string &xmlString, unsigned int *wbxml_len) {
  wbxml_conv_xml2wbxml_create(&_x2w);
//  wbxml_conv_xml2wbxml_enable_preserve_whitespaces(_x2w);
  
  const char *xml = xmlString.c_str();
  
  unsigned char *wbxml = NULL;
  unsigned int wb_len = 0;

  WBXMLError e = wbxml_conv_xml2wbxml_run(_x2w, (unsigned char *)xml, (unsigned int)xmlString.size(), &wbxml, &wb_len);
  if (e != WBXML_OK) {
    throw invalid_argument("xml to wbxml failed!");
  }
  
  *wbxml_len = wb_len;
  
  wbxml_conv_xml2wbxml_destroy(_x2w);
  _x2w = NULL;
  
  return wbxml;
}

