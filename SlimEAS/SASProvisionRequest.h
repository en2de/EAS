//
//  SASProvisionRequest.h
//  SlimEAS
//
//  Created by envy.chen on 12/19/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <string>

#include "SASDefine.h"

#include "SASCommandRequest.h"

#include "SASDevice.h"

namespace SlimEAS {
  /**
   *  This class represents a Provision command request
   *  ref : [MS-ASPROV] sec 2.2.
   */
  class SASProvisionRequest: public SASCommandRequest {
  public:
    
    /**
     *  This enumeration covers the acceptable values
     *  of the Status element in a Provision request
     *  when acknowledging a policy, as specified
     *  ref : [MS-ASPROV] sec 3.1.5.1.2.1.
     */
    typedef enum {
      AcknowledgementSuccess = 1,
      AcknowledgementPartialSucess = 2,
      AcknowledgementPolicyIgnored = 3,
      AcknowledgementExternalManagement = 4
    } SASPolicyAcknowledgement;
    
  private:
    static const std::string _policyType;
    
    bool _isAcknowledgement = false;
    bool _isRemoteWipe = false;
    int32_t _status = 0;
    SASDevice _provisionDevice;
    
  protected:
    virtual void generateXMLPayload();
    virtual SASHTTPResponse *initialResponse();
    
  public:
    SASProvisionRequest();
    ~SASProvisionRequest();
        
    bool isAcknowledgement() const {return _isAcknowledgement;}
    
    bool isRemoteWipe() const {return _isRemoteWipe;}
    
    int32_t status() const {return _status;}
    
    SASDevice &provisionDevice() {return _provisionDevice;}
    void setProvisionDevice(SASDevice device) {
      _provisionDevice = device;
    }
  };
}