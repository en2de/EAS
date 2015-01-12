//
//  SASUserInformation.h
//  SlimEAS
//
//  Created by guo gloria on 15/1/8.
//  Copyright (c) 2015年 Slim. All rights reserved.
//

#ifndef __SlimEAS__SASUserInformation__
#define __SlimEAS__SASUserInformation__

#include <stdio.h>
#include <string>
#include <list>

namespace SlimEAS {
  typedef enum {
    SASSettingsStatus_Success = 1,
    SASSettingsStatus_ProtocolError,
    SASSettingsStatus_AccessDenied,
    SASSettingsStatus_ServerUnavailable,
    SASSettingsStatus_InvalidArguments,
    SASSettingsStatus_ConflictingArguments,
    SASSettingsStatus_DeniedByPolicy
    
  } SASSettingsStatus;
  
  struct SASEmailAddresses {
    std::string SMTPAddress;
    std::string primarySmtpAddress;
  };
  
  class SASAccount {
    std::string _accountId;
    std::string _accountName;
    std::string _userDisplayName;
    bool _sendDisable;
    
    SASEmailAddresses _emailAddresses;
  };
  
  class SASUserInformation {
  public:
  private:
    SASSettingsStatus _status;
    std::list<SASEmailAddresses> _emailAddresses;
    std::list<SASAccount> _accounts;
  };
}

#endif /* defined(__SlimEAS__SASUserInformation__) */
