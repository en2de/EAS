//
//  SASMailOperRequest.h
//  SlimEAS
//
//  Created by Focus Lan on 1/11/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#pragma once

#include "SASCommandRequest.h"
#include "SASMail.h"

#include <mimetic/mimeentity.h>

namespace SlimEAS {
  
  class SASMailBaseRequest: public SASCommandRequest {
    
    typedef struct {
      std::string  folderId;
      std::string  itemId;
      std::string  longId;
      std::string  instanceId;
    } SourceInfomation;
    
    typedef struct {
      std::string         clientId;
      std::string         accountId;
      std::string         templateID;
      bool                saveInSentItems;
      mimetic::MimeEntity *mime;
      mimetic::MimeEntity *replaceMime;
      SourceInfomation    source;
    } MailInformation;
    
  public:
    SASMailBaseRequest();
    ~SASMailBaseRequest();
    
  public:
    SASMail &mail() { return _mail;}
    
    MailInformation &mailInfo() {return _mailInfomation;}
    
  protected:
    MailInformation _mailInfomation;
    SASMail         _mail;
    
  protected:
    virtual void generateMailInfo();    
    virtual void generateXMLPayload();
    
  public:
    int32_t getReponseStatus();
  };
}
