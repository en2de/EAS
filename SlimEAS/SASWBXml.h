//
//  SASWBXml.h
//  SlimEAS
//
//  Created by envy.chen on 12/14/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <string>
#include <cstdint>
#include <expat/expat.h>
#include <libwbxml/wbxml.h>


using namespace std;

namespace SlimEAS {
  /**
   *  This class contains all of the XML-to-WBXML and WBXML-to-XML functionality. It also contains the WBXML code pages defined in the ActiveSync WBXML protocol.
   */
  class SASWBXml {
  private:
    WBXMLConvWBXML2XML *_w2x;
    WBXMLConvXML2WBXML *_x2w;
    
  public:
    unsigned char *toWBXml(string &xmlString, unsigned int *wbxml_len);
    string *toXML(unsigned char *wbxml, unsigned int wbxml_len);
    
    SASWBXml();
    ~SASWBXml();
  };
}
