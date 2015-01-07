//
//  SASFolderSyncResponse.h
//  SlimEAS
//
//  Created by guo gloria on 15/1/5.
//  Copyright (c) 2015年 Slim. All rights reserved.
//

#ifndef __SlimEAS__SASFolderSyncResponse__
#define __SlimEAS__SASFolderSyncResponse__

#include <stdio.h>
#include <list>
#include <libxml/xmlreader.h>

#include "SASCommandResponse.h"

#include "SASFolder.h"

namespace SlimEAS {
  
  class SASFolderSyncResponse: public SASCommandResponse {
  public:
    enum FolderSyncStatus {
      FolderSync_Success = 1,
      FolderSync_ServerError = 6,
      FolderSync_InvalidSyncKey = 9,
      FolderSync_InvalidFormat = 10,
      FolderSync_UnknownError = 11,
      FolderSync_UnknownCode = 12
    };
  public:
    SASFolderSyncResponse(SASHTTPRequest::SASHTTPResponseContext &ctx);
    ~SASFolderSyncResponse();
    
    void updateRootFolder(SASFolder *rootFolder);
    int32_t status() {return _status;}
    
  private:
    bool parseInnerNode();

    
  private:
    int32_t _status = 0;
    std::string _syncKey = "";
    
    SASFolder* _rootFolder = nullptr;
    xmlTextReaderPtr _reader = NULL;
  };
}

#endif /* defined(__SlimEAS__SASFolderSyncResponse__) */
