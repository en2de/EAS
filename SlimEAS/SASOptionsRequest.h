//
//  SASOptionsRequest.h
//  SlimEAS
//
//  Created by envy.chen on 12/15/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <string>

#include "SASDefine.h"

using namespace std;

namespace SlimEAS {
  class SASOptionsRequest {
  private:
    
  SAS_PROPERTY(string, server);
  SAS_PROPERTY(bool, useSSL);
  SAS_PROPERTY(string, userName);
  SAS_PROPERTY(string, password);
    
  public:
    SASOptionsRequest();
    ~SASOptionsRequest();
  
    void testCurl();
  };
}
