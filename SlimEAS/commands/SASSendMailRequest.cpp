//
//  SASSendMailRequest.cpp
//  SlimEAS
//
//  Created by Focus Lan on 1/7/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASSendMailRequest.h"

using namespace SlimEAS;
using namespace std;

SASSendMailRequest::SASSendMailRequest()
: SASMailBaseRequest()
{
  _command = CMD_SENDMAIL;
}

SASSendMailRequest::~SASSendMailRequest() {

}