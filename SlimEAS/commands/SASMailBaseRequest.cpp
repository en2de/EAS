//
//  SASMailBaseRequest.cpp
//  SlimEAS
//
//  Created by Focus Lan on 1/11/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASMailBaseRequest.h"

using namespace SlimEAS;
using namespace std;

SASMailBaseRequest::SASMailBaseRequest()
: SASCommandRequest()
{
  
}

SASMailBaseRequest::~SASMailBaseRequest() {

}

void SASMailBaseRequest::generateMailInfo() {

}

void SASMailBaseRequest::generateXMLPayload() {
  
  _serializer.start();
  
  _serializer.startElement(_command);
  _serializer.writeAttribute("xmlns", "ComposeMail");
  
  if (_mailInfomation.clientId.empty()) {
    throw std::invalid_argument("You must issue a unique client id first!!!");
  }
  
  _serializer.writeElement("ClientId", _mailInfomation.clientId);
  
  if (!_mailInfomation.accountId.empty()) {
    _serializer.writeElement("AccountId", _mailInfomation.accountId);
  }
  
  if (!_mailInfomation.templateID.empty()) {
    _serializer.writeElement("TemplateID", _mailInfomation.templateID);
  }
  
  if (_mailInfomation.saveInSentItems) {
    _serializer.writeEmptyElement("SaveInSentItems");
  }
  
  generateMailInfo();
  
  string mimeData = _mail.asMIMEString();
  
  // 1. Take care of this Element, it must be "MIME", not "Mime" or others.
  // 2. Must be CDATA element.
  // 3. WBXML lib will take  care of it and encode it as base64.
  if (!mimeData.empty()) {
    _serializer.startElement("MIME");
    _serializer.writeCDATA(mimeData.c_str());
    _serializer.endElement(); // end element for Mime
  }
  
  _serializer.done(); // end element for SendMail
  
  _xmlPayload = _serializer.outerXml();
  
  printf("SmartReply payload: \n%s\n", _xmlPayload.c_str());
  
  _serializer.done();
  
}

int32_t SASMailBaseRequest::getReponseStatus() {
  
  return 200;

}