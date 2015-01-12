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
#include <list>

namespace SlimEAS {
  typedef enum {
    SASOofMessageBodyType_HTML,
    SASOofMessageBodyType_Text
  } SASOofMessageBodyType;
  
  /* Out of office */
  struct SASOofMessage {
    bool _isAppliesToInternal;
    bool _isAppliesToExternalKnown;
    bool _isAppliesToExternalUnknown;
    bool _isEnabled;
    std::string _replyMessasge;
    SASOofMessageBodyType _bodyType;
  };
  
  class SASOof
  {
  public:
    typedef enum {
      SASOofState_Unset = 0,
      SASOofState_Disabled = 1,
      SASOofState_Global,
      SASOofState_TimeBased
      
    } SASOofState;
    
  public:
    SASOof();
    ~SASOof();
    
    void setOofState(const SASOofState state) { _oofState = state; }
    SASOofState oofState() { return _oofState; }
    
    void setStartTime(const time_t startTime) { _startTime = startTime; }
    time_t startTime() { return _startTime; }
    
    void setEndTime(const time_t endTime) { _endTime = endTime; }
    time_t endTime() { return _endTime; }
    
    void addOofMessage(SASOofMessage *oofMessage);
    void addOofMessage(bool isAppliesToInternal,
                       bool isAppliesToExternalKnown,
                       bool isAppliesToExternalUnknown,
                       bool isEnabled,
                       const std::string &replyMessage,
                       SASOofMessageBodyType bodyType);
    std::list<SASOofMessage *> &oofMessages() { return _oofMessages; }
    
  private:
    SASOofState _oofState;
    time_t _startTime;
    time_t _endTime;
    std::list<SASOofMessage *> _oofMessages;
  };
}

#endif /* defined(__SlimEAS__SASOof__) */
