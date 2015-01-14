//
//  SASFetchMIMEEmailRequest.cpp
//  SlimCore
//
//  Created by Focus Lan on 1/13/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASFetchMIMEEmailRequest.h"

#include <string>

using namespace std;
using namespace SlimEAS;

SASFetchMIMEEmailRequest::SASFetchMIMEEmailRequest()
: SASFetchEmailRequest()
{
  BodyPreferences preference;
  preference.Type = MIME;
  _options.setBodyPreferences(preference);
  _options.mimeSupportLevel =  SendMimeForSMime;
}

SASFetchMIMEEmailRequest::~SASFetchMIMEEmailRequest() {
}