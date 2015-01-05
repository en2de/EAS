//
//  SASFolderSyncRequest.h
//  SlimEAS
//
//  Created by guo gloria on 15/1/2.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#ifndef __SlimEAS__SASFolderSyncRequest__
#define __SlimEAS__SASFolderSyncRequest__

#include <stdio.h>

#include "SASCommandRequest.h"

namespace SlimEAS {
  class SASFolderSyncRequest: public SASCommandRequest {
  public:
    SASFolderSyncRequest();
    ~SASFolderSyncRequest();
    
    inline std::string &syncKey() {return _syncKey;}
    
  protected:
    virtual void generateXMLPayload();
    virtual SASHTTPResponse *initialResponse();
    
  private:
    std::string _syncKey = "0";
  };
  
}

#endif /* defined(__SlimEAS__SASFolderSyncRequest__) */
