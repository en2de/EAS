//
//  SASSerializer.h
//  SlimEAS
//
//  Created by Focus Lan on 1/11/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <map>
#include <string>
#include <libxml/xmlwriter.h>

#include "SASDefine.h"

#include "SASHTTPRequest.h"

namespace SlimEAS {
  class SASSerializer {
  private:
    xmlTextWriterPtr  _writer;
    xmlBufferPtr      _buf;
    std::string       _outerXml;
    int32_t           _intent;
    
  public:
    SASSerializer();
    ~SASSerializer();
    
    bool create();
    
    void start();
    
    void setIntent(const int32_t &intent = 1) {_intent = intent;}
    
    void startElement(const std::string &name);
    void startElementNS(const std::string &name, const std::string &prefix, const std::string &space);
    
    void writeElement(const std::string &name, const std::string &value);
    void writeElementNS(const std::string &name, const std::string &value, const std::string &prefix, const std::string &space = "");
    void writeEmptyElement(const std::string &name);
    void writeFormatElement(const std::string &name, const char *format, ...);
    void writeCDATA(const std::string &value);
    void writeRaw(const std::string &value);
    
    void writeAttribute(const std::string &name, const std::string &value);
    void writeAttributeNS(const std::string &name, const std::string &value, const std::string &prefix, const std::string &space = "");
    
    void endElement();
    
    void done();
    
    void free();
    
    const std::string &outerXml() {return _outerXml;}
    const int32_t &intent() {return _intent;}
  };
}

