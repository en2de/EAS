//
//  SASMail.h
//  SlimEAS
//
//  Created by Focus Lan on 1/4/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <string>
#include <vector>

#include "SASBody.h"
#include "SASBaseEntity.h"
#include "SASAttachment.h"

namespace SlimEAS {

  class SASMail : public SASBaseEntity {
    
  public:
    // 2.2.2.17 NativeBodyType
    typedef enum {
      PlainText = 1,
      HTML = 2,
      RTF = 3
    } NativeBodyType;
    
  private:
    std::string     _to;
    std::string     _cc;
    std::string     _bcc;
    std::string     _from;
    std::string     _displayFrom;
    std::string     _displayTo;
    std::string     _subject;
    std::string     _threadTopic;
    std::string     _messageClass;
    std::string     _contentClass;
    NativeBodyType  _nativeBodyType;
    int32_t         _importance;
    int32_t         _read;
    int32_t         _flag;
    int64_t         _dateReceived;
    
    // property under email2 namespace:
    std::string   _conversationId;
    int32_t       _conversationIndex;
    
    SASBody       _body;
    bool          _hasBody = false;
    
    std::vector<SASAttachment*> _attachments;
    
  public:
    const std::string &to() {return _to;}
    void setTo(const std::string &val) { _to = val;}
    
    const SASBody &body() {return _body;}
    void setBody(const SASBody &val) { _hasBody = true; _body = val;}
    
    const std::string &cc() {return _cc;}
    void setCc(const std::string &val) { _cc = val;}
    
    const std::string &from() {return _from;}
    void setFrom(const std::string &val) { _from = val;}
    
    const std::string &displayFrom() {return _displayFrom;}
    void setDisplayFrom(const std::string &val) {_displayFrom = val;}
    
    const std::string &displayTo() {return _displayTo;}
    void setDisplayTo(const std::string &val) { _displayTo = val;}
    
    const std::string &subject() {return _subject;}
    void setSubject(const std::string &val) {_subject = val;}
    
    const std::string &threadTopic() {return _threadTopic;}
    void setThreadTopic(const std::string &val) {_threadTopic = val;}
    
    const int32_t &importance() {return _importance;}
    void setImportance(const int32_t &val) {_importance = val;}
    
    const int32_t &read() {return _read;}
    void setRead(const int32_t &val) {_read = val;}
    
    const int32_t &flag() {return _flag;}
    void setFlag(const int32_t &val) {_flag = val;}
    
    const bool hasAttachment() { return _attachments.size() != 0;}
    const std::vector<SASAttachment*> &attachments() {return _attachments;}
    const size_t attachmentCount() {return _attachments.size();}
    
    const int64_t &dateReceived() {return _dateReceived;}
    void setDateReceived(const int64_t &val) {_dateReceived = val;}
    
    const std::string &conversationId() {return _conversationId;}
    void setConversationId(const std::string &val) { _conversationId = val;}
    
    const int32_t &conversationIndex() {return _conversationIndex;}
    void setConversationId(int32_t &val) { _conversationIndex = val;}
    
    virtual void decode(const std::string &xml);
    // instance to xml
    virtual const std::string encode();
    
    // get a MIME data
    const std::string asMIMEData();
    
    SASMail();
    ~SASMail();
  };

}
