//
//  SASPingRequest.h
//  SlimEAS
//
//  Created by Focus Lan on 1/8/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#pragma once

#include "SASCommandRequest.h"
#include "SASFolder.h"

namespace SlimEAS {
  
  class SASPingRequest: public SASCommandRequest {
    
  public:    
    struct FolderToSync {
      std::string folderId;
      std::string className;
      
      FolderToSync()
      :
      folderId(""),
      className("Email")
      {
        
      }
      
      FolderToSync(const std::string &folderId, const std::string &className)
      :
      folderId(folderId),
      className(className)
      {
        
      }
      
    };
    
  public:
    SASPingRequest();
    ~SASPingRequest();
    
  private:
    int32_t                     _heartbeatInterval = 0;
    std::vector<FolderToSync>   _simpleFolderList;
    
  public:
    
    const int32_t &heartbeatInterval() {return _heartbeatInterval;}
    void setHeartbeatInterval(const int32_t &val) {_heartbeatInterval = val;}
    
    void addFolder(const FolderToSync &folderToSync) {_simpleFolderList.push_back(folderToSync);}
    
    void addFolder(const std::string &folderId, const std::string &className) {_simpleFolderList.push_back(FolderToSync(folderId, className));}
    
  protected:
    virtual void generateXMLPayload();
    virtual SASHTTPResponse *initialResponse();
  
  };
}
