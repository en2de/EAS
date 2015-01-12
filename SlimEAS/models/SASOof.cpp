//
//  SASOof.cpp
//  SlimEAS
//
//  Created by guo gloria on 15/1/8.
//  Copyright (c) 2015年 Slim. All rights reserved.
//

#include "SASOof.h"

using namespace SlimEAS;

SASOof::SASOof()
: _oofState(SASOofState_Unset),
_startTime(-1),
_endTime(-1)
{
}

SASOof::~SASOof()
{
  for (auto iter: _oofMessages) {
    delete iter;
  }
  _oofMessages.clear();
}


void SASOof::addOofMessage(SASOofMessage *oofMessage)
{
  _oofMessages.push_back(oofMessage);
}

void SASOof::addOofMessage(bool isAppliesToInternal, bool isAppliesToExternalKnown, bool isAppliesToExternalUnknown, bool isEnabled, const std::string &replyMessage, SASOofMessageBodyType bodyType)
{
  SASOofMessage *oofMessage = new SASOofMessage({isAppliesToInternal, isAppliesToExternalKnown, isAppliesToExternalUnknown, isEnabled, replyMessage, bodyType});
  _oofMessages.push_back(oofMessage);
}
