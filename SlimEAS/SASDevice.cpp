//
//  SASDevice.cpp
//  SlimEAS
//
//  Created by envy.chen on 12/19/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#include "SASDevice.h"

#include <libxml/xmlwriter.h>

using namespace SlimEAS;
using namespace std;

static string _settingsXmlns = "settings";
static string _settingsNamespace = "Settings";

static const char *_prefix = "settings";

string SASDevice::payload() {
  
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
  
  xmlTextWriterStartDocument(writer, NULL, NULL, NULL);
  xmlTextWriterStartElementNS(writer, BAD_CAST _prefix, BAD_CAST "DeviceInformation", NULL);
  xmlTextWriterStartElementNS(writer, BAD_CAST _prefix, BAD_CAST "Set", NULL);
  if (!_model.empty()) {
    xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST "Model", NULL, BAD_CAST _model.c_str());
  }
  
  if (!_IMEINumber.empty()) {
    xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST "IMEI", NULL, BAD_CAST _IMEINumber.c_str());
  }
  
  if (!_friendlyName.empty()) {
    xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST "FriendlyName", NULL, BAD_CAST _friendlyName.c_str());
  }
  
  if (!_operatingSystem.empty()) {
    xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST "OS", NULL, BAD_CAST _operatingSystem.c_str());
  }
  
  if (!_operatingSystemLanguage.empty()) {
    xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST "OSLanguage", NULL, BAD_CAST _operatingSystemLanguage.c_str());
  }
  
  if (!_phoneNumber.empty()) {
    xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST "PhoneNumber", NULL, BAD_CAST _phoneNumber.c_str());
  }
  
  if (!_mobileOperator.empty()) {
    xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST "MobileOperator", NULL, BAD_CAST _mobileOperator.c_str());
  }
  
  if (!_userAgent.empty()) {
    xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST "UserAgent", NULL, BAD_CAST _mobileOperator.c_str());
  }

  xmlTextWriterEndElement(writer);
  xmlTextWriterEndElement(writer);
  xmlTextWriterEndDocument(writer);
  
  string xml((char *)buf->content);
  
//  std::cout << xml;
  
  return xml;
}