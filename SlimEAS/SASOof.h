//
//  SASOof.h
//  SlimEAS
//
//  Created by guo gloria on 15/1/8.
//  Copyright (c) 2015年 Slim. All rights reserved.
//

#ifndef __SlimEAS__SASOof__
#define __SlimEAS__SASOof__

#include <stdio.h>
#include <string>


namespace SlimEAS {
  typedef enum {
    SASOofMessageBodyType_HTML,
    SASOofMessageBodyType_Text
  } SASOofMessageBodyType;
  
  /* Out of office */
  struct SASOofMessage {
    bool _isAppliesToInternal;
    bool _isAppliesToExternalKnown;
    bool _isAppliesToExternalUnknow;
    bool _enabled;
    std::string _replyMessasge;
    SASOofMessageBodyType _bodyType;
  };
  
  class SASOof
  {
  public:
    typedef enum {
      SASOofState_Disabled = 1,
      SASOofState_Global,
      SASOofState_TimeBased
      
    } SASOofState;
  private:
    SASOofState _oofState;
    time_t _startTime;
    time_t _endTime;
    SASOofMessage _oofMessage;
  };
}

#endif /* defined(__SlimEAS__SASOof__) */
