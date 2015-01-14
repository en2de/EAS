//
//  SASSerializer.cpp
//  SlimEAS
//
//  Created by Focus Lan on 1/11/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASSerializer.h"

using namespace std;
using namespace SlimEAS;

SASSerializer::SASSerializer()
: _writer(nullptr),
  _buf(nullptr),
  _outerXml(),
  _intent(1)
{
}

SASSerializer::~SASSerializer() {

  free();
  
}

bool SASSerializer::create() {

  bool result = true;
  _buf = xmlBufferCreate();
  if (_buf == nullptr) {
    result = false;
    goto end;
  }
  
  _writer = xmlNewTextWriterMemory(_buf, 0);
  
  if (_writer == nullptr) {
    result = false;
    goto end;
  }
  
  xmlTextWriterSetIndent(_writer, _intent);
  
end:
  return result;
}

void SASSerializer::start() {
  
  xmlTextWriterStartDocument(_writer, "1.0", "utf-8", nullptr);
  xmlTextWriterWriteDTD(_writer, BAD_CAST "ActiveSync", BAD_CAST "-//MICROSOFT//DTD ActiveSync//EN", BAD_CAST "http://www.microsoft.com/", nullptr);
}

void SASSerializer::startElement(const std::string &name) {

  xmlTextWriterStartElement(_writer, BAD_CAST name.c_str());
  
}

void SASSerializer::startElementNS(const std::string &name, const std::string &prefix, const std::string &space) {

  xmlTextWriterStartElementNS(_writer, BAD_CAST prefix.c_str(), BAD_CAST name.c_str(), BAD_CAST space.c_str());
  
}

void SASSerializer::writeElement(const std::string &name, const std::string &value) {

  xmlTextWriterWriteElement(_writer, BAD_CAST name.c_str(), BAD_CAST value.c_str());
  
}

void SASSerializer::writeElementNS(const std::string &name, const std::string &value, const std::string &prefix, const std::string &space) {
  
  if (space.empty()) {
    xmlTextWriterWriteElementNS(_writer, BAD_CAST prefix.c_str(), BAD_CAST name.c_str(), nullptr, BAD_CAST value.c_str());
  } else {
    xmlTextWriterWriteElementNS(_writer, BAD_CAST prefix.c_str(), BAD_CAST name.c_str(), BAD_CAST space.c_str(), BAD_CAST value.c_str());
  }
  
}

void SASSerializer::writeEmptyElement(const std::string &name) {

  startElement(name);
  endElement();
  
}

void SASSerializer::writeFormatElement(const std::string &name, const char *format, ...) {

  va_list arglist;
  va_start(arglist, format);
  xmlTextWriterWriteVFormatElement(_writer, BAD_CAST name.c_str(), format, arglist);
  
}

void SASSerializer::writeFormatElementNS(const std::string &name, const std::string &prefix, const std::string &space, const char *format, ...) {

  va_list arglist;
  va_start(arglist, format);
  
  if (space.empty()) {
   xmlTextWriterWriteVFormatElementNS(_writer, BAD_CAST prefix.c_str(), BAD_CAST name.c_str(), nullptr, format, arglist);
  } else {
    xmlTextWriterWriteVFormatElementNS(_writer, BAD_CAST prefix.c_str(), BAD_CAST name.c_str(), BAD_CAST space.c_str(), format, arglist);
  }
  
}

void SASSerializer::writeCDATA(const std::string &value) {

  xmlTextWriterWriteCDATA(_writer, BAD_CAST value.c_str());
  
}

void SASSerializer::writeRaw(const std::string &value) {

  xmlTextWriterWriteRaw(_writer, BAD_CAST value.c_str());
  
}

void SASSerializer::writeAttribute(const std::string &name, const std::string &value) {
  
  xmlTextWriterWriteAttribute(_writer, BAD_CAST name.c_str(), BAD_CAST value.c_str());
  
}

void SASSerializer::writeAttributeNS(const std::string &name, const std::string &value, const std::string &prefix, const std::string &space) {

  if(space.empty()) {
    xmlTextWriterWriteAttributeNS(_writer, BAD_CAST prefix.c_str(), BAD_CAST name.c_str(), nullptr, BAD_CAST value.c_str());
  } else {
    xmlTextWriterWriteAttributeNS(_writer, BAD_CAST prefix.c_str(), BAD_CAST name.c_str(), BAD_CAST space.c_str(), BAD_CAST value.c_str());
  }
  
}

void SASSerializer::endElement() {

  xmlTextWriterEndElement(_writer);
  
}

void SASSerializer::done() {

  xmlTextWriterEndDocument(_writer);
  _outerXml = string((char*)_buf->content);
  
}

void SASSerializer::free() {

  if (_writer != nullptr) {
    xmlFreeTextWriter(_writer);
  }
  _writer = nullptr;
  
  if (_buf != nullptr) {
    xmlBufferFree(_buf);
  }
  _buf = nullptr;
  
}