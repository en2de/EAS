//
//  SASItemOperationsRequest.h
//  SlimEAS
//
//  Created by Focus Lan on 1/6/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//
#include "SASCommandRequest.h"
#include "SASFolder.h"

#include <string>

namespace SlimEAS {
  
  // enums for how to fetch email
  typedef enum {
    EmailItem = 0,
    MimeEmailItem,
  } FetchProfile;
  
  /**
   *  This class is used to fetch email and attachments
   */
  class SASItemOperationsRequest: public SASCommandRequest {
    
  public:
    SASItemOperationsRequest();
    ~SASItemOperationsRequest();
    
    // getter/setter
  public:
    void setFecthProfile(const FetchProfile &fetchProfile) {_fetchProfile = fetchProfile;}
    const FetchProfile &getFetchProfile() {return _fetchProfile;}
    
    void setStore(const std::string &store) {_store = store;}
    const std::string &store() {return _store;}
    
    void setCollectionId(const std::string &collectionId) {_collectionId = collectionId;}
    const std::string &collectionId() {return _collectionId;}
    
    void setServerId(const std::string &serverId) {_serverId = serverId;}
    const std::string &serverId() {return _serverId;}
    
    void setOptions(const FolderSyncOptions &options) { _options = options;}
    const FolderSyncOptions &options() {return _options;}
    
  protected:
    virtual void generateXMLPayload();
    virtual SASHTTPResponse *initialResponse();
    
  private:
    std::string       _store;
    std::string       _collectionId = "0";
    std::string       _serverId;
    FolderSyncOptions _options;
    FetchProfile      _fetchProfile;
  };
}