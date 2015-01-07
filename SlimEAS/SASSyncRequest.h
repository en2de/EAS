//
//  SASSyncRequest.h
//  SlimEAS
//
//  Created by guo gloria on 15/1/2.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <list>

#include "SASCommandRequest.h"


namespace SlimEAS {
  class SASFolder;
  
  typedef enum {
    Synchronizing  = 0,
    Fetching
  } Action;
  
  class SASSyncRequest: public SASCommandRequest {
    
  public:
    SASSyncRequest(const std::string &server, const std::string &user, const std::string &password, bool useSSL = true);
    SASSyncRequest();
    ~SASSyncRequest();
     
    inline int32_t wait() const {return _wait;}
    inline int32_t heartBeatInterval() const {return _heartBeatInterval;}
    inline int32_t windowSize() const {return _windowSize;}
    inline bool isPartial() const {return _isPartial;}
    inline std::list<SASFolder *> & folderList() {return _folderList;}
    
    inline void addFolder(SASFolder *folder) { _folderList.push_back(folder); }
    
  protected:
    virtual void generateXMLPayload();
    virtual SASHTTPResponse *initialResponse();
    
  private:
    int32_t _wait = 0;
    int32_t _heartBeatInterval = 0;
    int32_t _windowSize = 0;
    bool _isPartial = false;
    std::list<SASFolder *> _folderList;
    Action _action = Synchronizing;

  };
}
