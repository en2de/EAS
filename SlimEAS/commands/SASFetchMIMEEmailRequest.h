//
//  SASFetchMIMEEmailRequest.h
//  SlimCore
//
//  Created by Focus Lan on 1/13/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#pragma once

#include "SASFetchEmailRequest.h"

#include <string>

namespace SlimEAS {
  
  /**
   *  This class is used to fetch attachment
   */
  class SASFetchMIMEEmailRequest: public SASFetchEmailRequest {
    
  public:
    SASFetchMIMEEmailRequest();
    ~SASFetchMIMEEmailRequest();
    
  public:
    void setOptions(const FolderSyncOptions &options) { }
    const FolderSyncOptions &options() {return _options;}
  };
}
