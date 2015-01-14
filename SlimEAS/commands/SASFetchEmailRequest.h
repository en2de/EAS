//
//  SASFetchEmailRequest.h
//  SlimCore
//
//  Created by Focus Lan on 1/13/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#pragma once

#include "SASItemOperationsRequest.h"
#include "SASFolder.h"

#include <string>

namespace SlimEAS {
  
  /**
   *  This class is used to fetch attachment
   */
  class SASFetchEmailRequest: public SASItemOperationsRequest {
    
  public:
    SASFetchEmailRequest();
    ~SASFetchEmailRequest();
    
    // getter/setter
  public:
    
    void setCollectionId(const std::string &collectionId) {_collectionId = collectionId;}
    const std::string &collectionId() {return _collectionId;}
    
    void setServerId(const std::string &serverId) {_serverId = serverId;}
    const std::string &serverId() {return _serverId;}
    
    virtual void setOptions(const FolderSyncOptions &options) { _options = options;}
    const FolderSyncOptions &options() {return _options;}
    
  protected:
    virtual void generateXMLSubPayload();
    
  protected:
    std::string       _collectionId;
    std::string       _serverId;
    FolderSyncOptions _options;
  };
}
