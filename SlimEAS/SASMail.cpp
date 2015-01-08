//
//  SASMail.cpp
//  SlimEAS
//
//  Created by Focus Lan on 1/4/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASMail.h"

#include <cassert>
#include <time.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlsave.h>
#include <libxml/xmlreader.h>

using namespace std;
using namespace SlimEAS;

static const char *_prefix = "email";

void SASMail::decode(const string &xmlString) {

  xmlTextReaderPtr reader;
  uint8_t *buf = (uint8_t *)xmlString.c_str();
  int buf_len = (int)strlen((const char *)buf);
  _attachments.clear();
  SASAttachment att;
  SASBody body;
  
  reader = xmlReaderForMemory((const char *)buf, buf_len, nullptr, "utf-8", 0);
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
        if (strcmp((const char *)curr_elm, "Attachment") == 0) {
          string attXml = string((const char*)xmlTextReaderReadOuterXml(reader));
          att.decode(attXml);
          _attachments.push_back(att);
          xmlTextReaderNext(reader);
        } else if (strcmp((const char *)curr_elm, "Body") == 0) {
          string attXml = string((const char*)xmlTextReaderReadOuterXml(reader));
          _hasBody = true;
          _body.decode(attXml);
          xmlTextReaderNext(reader);
        }
        
        printf("current element %s \n", curr_ele_full);
        continue;
      } else if (n_type == XNT_Text && curr_elm != NULL) {
        
        if (curr_elm_prefix != nullptr &&
            strcmp((const char *)curr_elm_prefix, "email") != 0 &&
            strcmp((const char *)curr_elm_prefix, "email2") != 0 &&
            strcmp((const char *)curr_elm_prefix, "airsyncbase")) {
          continue;
        }
        
#define ToU8(v) sscanf((const char *)value, "%c", &v)
#define ToBool(v) do { \
uint8_t _  = 0; \
ToU8(_); \
v = _ == 0? false : true; \
} while(0)
#define ToI32(v) sscanf((const char *)value, "%d", &v)
#define ToI64(v) sscanf((const char *)value, "%lld", &v)
#define ToU32(v) sscanf((const char *)value, "%u", &v)
#define ToSTR(v) (string((const char*)v))
        
        const xmlChar *value = xmlTextReaderConstValue(reader);
        if (strcmp((const char *)curr_elm, "To") == 0) {
          _to = ToSTR(value);
        }else if (strcmp((const char *)curr_elm, "Cc") == 0) {
          _cc = ToSTR(value);
        } else if(strcmp((const char *)curr_elm, "From") == 0 ) {
          _from = ToSTR(value);
        } else if(strcmp((const char *)curr_elm, "Subject") == 0 ){
          _subject = ToSTR(value);
        } else if(strcmp((const char *)curr_elm, "DateReceived") == 0) {
          struct tm tm;
          time_t now;
          strptime((const char*)value, "%Y-%m-%dT%T", &tm);
          
          now = mktime(&tm);
          _dateReceived = now;
          
        } else if(strcmp((const char *)curr_elm, "DisplayTo") == 0) {
          _displayTo = ToSTR(value);
        } else if(strcmp((const char *)curr_elm, "ThreadTopic") == 0) {
          _threadTopic = ToSTR(value);
        } else if(strcmp((const char *)curr_elm, "Importance") == 0) {
          ToI32(_importance);
        } else if(strcmp((const char *)curr_elm, "Read") == 0) {
          ToI32(_read);
        } else if(strcmp((const char *)curr_elm, "MessageClass") == 0) {
          _messageClass = ToSTR(value);
        } else if(strcmp((const char *)curr_elm, "ConversationId") == 0) {
          _conversationId = ToSTR(value);
        } else if(strcmp((const char *)curr_elm, "ConversationIndex") == 0) {
          ToI32(_conversationIndex);
        } else if(strcmp((const char *)curr_elm, "ContentClass") == 0) {
          _contentClass = ToSTR(value);
        } else if(strcmp((const char *)curr_elm, "NativeBodyType") == 0) {
          ToI32(_nativeBodyType);
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

SASMail::SASMail()
: SASBaseEntity()
{

}

SASMail::~SASMail() {

}

void SASMail::addAttachment(const SASAttachment &attachment) {
  _attachments.push_back(attachment);
}

const string SASMail::encode() {
  xmlBufferPtr buf;
  buf = xmlBufferCreate();
  if (buf == NULL) {
    throw std::invalid_argument("Error creating the xml buffer");
  }
  
  xmlDocPtr doc;
  
  xmlTextWriterPtr writer;
  writer = xmlNewTextWriterDoc(&doc, 0);
  //  writer = xmlNewTextWriterMemory(buf, 0);
  xmlTextWriterSetIndent(writer, 1);
  if (writer == NULL) {
    throw std::invalid_argument("Error creating the xml writer");
  }
  
  xmlTextWriterStartDocument(writer, nullptr, nullptr, nullptr);
  xmlTextWriterStartElementNS(writer, BAD_CAST _prefix, BAD_CAST "ApplicationData", nullptr);
  
  if (!_to.empty()) {
    xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST "To", nullptr, BAD_CAST _to.c_str());
  }
  
  if (!_from.empty()) {
    xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST "From", nullptr, BAD_CAST _from.c_str());
  }
  
  if (!_subject.empty()) {
    xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST "Subject", nullptr, BAD_CAST _subject.c_str());
  }
  
  if (!_displayTo.empty()) {
    xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST "DisplayTo", nullptr, BAD_CAST _displayTo.c_str());
  }
  
  if (!_threadTopic.empty()) {
    xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST "ThreadTopic", nullptr, BAD_CAST _threadTopic.c_str());
  }
  
  if(_dateReceived > 0) {
    
    time_t time = _dateReceived;
    std::tm* now_tm= gmtime(&time);
    char buf[42];
    strftime(buf, 42, "%Y-%m-%dT%TZ", now_tm);
    
    xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST "DateReceived", nullptr, BAD_CAST buf);
  }
  
  xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST "Importance", nullptr, BAD_CAST to_string(_importance).c_str());
  xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST "Read", nullptr, BAD_CAST to_string(_read).c_str());
  
  // the body part
  if (_hasBody) {
    string bodyXml = _body.encode();
    if (!bodyXml.empty()) {
      xmlTextWriterWriteRaw(writer, BAD_CAST bodyXml.c_str());
    }
  }
  
  // attachments
  if (_attachments.size() > 0) {
    xmlTextWriterStartElementNS(writer, BAD_CAST _prefix, BAD_CAST "Attachments", nullptr);
    
    for (auto &it : _attachments) {
      xmlTextWriterWriteRaw(writer, BAD_CAST it.encode().c_str());
    }
    xmlTextWriterEndElement(writer);
  }
  
  xmlTextWriterEndElement(writer);
  xmlTextWriterEndDocument(writer);
  
  xmlSaveCtxtPtr saveCtx = xmlSaveToBuffer(buf, "utf-8", XML_SAVE_NO_DECL | XML_SAVE_FORMAT);
  xmlSaveDoc(saveCtx, doc);
  xmlSaveFlush(saveCtx);
  xmlSaveClose(saveCtx);
  
  string xml((char *)buf->content);
  
  xmlFreeTextWriter(writer);
  xmlFreeDoc(doc);
  xmlBufferFree(buf);
  
  return xml;
}

const string SASMail::asMIMEData() {
  string mimeData = "";
  
  assert(_from.empty() == false);
  assert(_to.empty() == false);
  
  mimeData.append("From: "    + _from     + "\n");
  mimeData.append("To: "      + _to       + "\n");
  mimeData.append("Cc: "      + _cc       + "\n");
  mimeData.append("Bcc: "     + _bcc      + "\n");
  mimeData.append("Subject:"  + _subject  + "\n");
  
  mimeData.append("MIME-Version: 1.0\n");
  
  mimeData.append("Content-Type: text/plain; charset=\"iso-8859-1\"\n");
  mimeData.append("Content-Transfer-Encoding: 7bit\n");
  mimeData.append("X-MimeOLE: Produced By Microsoft MimeOLE V6.00.2900.3350\n");
  
  mimeData.append(_body.mimeData());
  
  
  return  mimeData;
}