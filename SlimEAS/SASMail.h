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
  private:
    std::string   _to;
    std::string   _cc;
    std::string   _from;
    std::string   _displayTo;
    std::string   _subject;
    std::string   _threadTopic;
    std::string   _messageClass;
    int32_t       _importance;
    int32_t       _read;
    int32_t       _flag;
    int64_t       _dateReceived;
    
    // property under email2 namespace:
    std::string   _conversationId;
    int32_t       _conversationIndex;
    
    SASBody       _body;
    bool          _hasBody = false;
    
    std::vector<SASAttachment> _attachments;
    
  public:
    const std::string &to() {return _to;}
    void setTo(const std::string &val) { _to = val;}
    
    const SASBody &body() {return _body;}
    void setBody(const SASBody &val) { _hasBody = true; _body = val;}
    
    const std::string &cc() {return _cc;}
    void setCc(const std::string &val) { _cc = val;}
    
    const std::string &from() {return _from;}
    void setFrom(const std::string &val) { _from = val;}
    
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
    
    const int64_t &dateReceived() {return _dateReceived;}
    void setDateReceived(const int64_t &val) {_dateReceived = val;}
    
    void addAttachment(const SASAttachment &attachment);
    
    virtual void decode(const std::string &xml);
    // instance to xml
    virtual const std::string encode();
    
    SASMail();
    ~SASMail();
  };

}