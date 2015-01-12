//
//  SASProvisionRequest.cpp
//  SlimEAS
//
//  Created by envy.chen on 12/19/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#include "SASProvisionRequest.h"
#include "SASProvisionResponse.h"

using namespace std;
using namespace SlimEAS;

#pragma mark - life cycle

SASProvisionRequest::SASProvisionRequest()
: SASCommandRequest()
{
  _command = CMD_PROVISION;
}

SASProvisionRequest::~SASProvisionRequest() {
}

#pragma mark - override func

SASHTTPResponse *SASProvisionRequest::initialResponse() {
  return new SASProvisionResponse(_resContext);
}

void SASProvisionRequest::generateXMLPayload() {
  
  _serializer.start();
  
  _serializer.startElement(_command);
  _serializer.writeAttribute("xmlns", "Provision:");
  if (!_isAcknowledgement) {
    _serializer.writeAttributeNS("settings", "Settings:", "xmlns");
  }
  
  //remote wipe
  if (_isRemoteWipe) {
    _serializer.startElement("RemoteWipe");
    _serializer.writeElement("Status", "1");
    _serializer.endElement();
  }else {
    if (!_isAcknowledgement) {
      _serializer.writeRaw(_provisionDevice.payload());
    }
    _serializer.startElement("Policies");
    _serializer.startElement("Policy");
    _serializer.writeElement("PolicyType", "MS-EAS-Provisioning-WBXML");
    if (_isAcknowledgement) {
      _serializer.writeFormatElement("PolicyKey", "%d", _policyKey);
      _serializer.writeFormatElement("Status", "%d", _status);
    }
    
    _serializer.endElement();
    _serializer.endElement();
    _serializer.endElement();
  }
  
  _serializer.done();

  _xmlPayload = _serializer.outerXml();
  
  printf("SASProvisionRequest payload: \n%s\n", _xmlPayload.c_str());
}



