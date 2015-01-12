//
//  SASCollection.h
//  SlimEAS
//
//  Created by Focus Lan on 1/7/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#pragma once

#include <string>
#include <vector>

#include "SASMail.h"
#include "SASBaseEntity.h"

namespace SlimEAS {
  
  class SASCommand : public SASBaseEntity {
#pragma properties
  private:
    std::string _name;
    std::string _serverId;
    SASMail     _applicationData;
    
#pragma getter/setter
  public:
    const std::string &name() {return _name;}
    const std::string &serverId() {return _serverId;}
    const SASMail     &applicationData() {return _applicationData;}
    
  public:
#pragma virtual methods from base class
    virtual void decode(const std::string &xml);
    // instance to xml
    virtual const std::string encode();
    
#pragma constructor/deconstructor
  public:
    SASCommand();
    ~SASCommand();
  };
  
  class SASCollection : public SASBaseEntity {

#pragma properties
  private:
    std::string               _className;
    std::string               _syncKey;
    std::string               _collectionId;
    int32_t                   _status;
    bool                      _moreAvailable;
    std::vector<SASCommand*>  _commandCollection;
    
#pragma getter/setter
  public:
    const std::string &className() {return _className;}
    const std::string &syncKey() {return _syncKey;}
    const std::string &collectionId() {return _collectionId;}
    const int32_t     &status() {return _status;}
    const bool        &moreAvailable() {return _moreAvailable;}
    const std::vector<SASCommand*> &commandCollection() {return _commandCollection;}
  
  public:
#pragma virtual methods from base class
    virtual void decode(const std::string &xml);
    // instance to xml
    virtual const std::string encode();
    
#pragma constructor/deconstructor
  public:
    SASCollection();
    ~SASCollection();
  };

}
