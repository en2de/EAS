//
//  SASPingResponse.h
//  SlimEAS
//
//  Created by Focus Lan on 1/8/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASCommandResponse.h"

#include <string>
#include <vector>

namespace SlimEAS {
  
  // enum for Ping Status, 2.2.3.162.10 (MS-ASCMD.pdf)
  typedef enum {
    UnKnown                     = 0,
    HeartbeatIntervalExpired    = 1,
    ChangesFound                = 2,
    OmittedParametersRequired   = 3,
    SyntaxErrorInRequest        = 4,
    HeartbeatIntervalOutOfRange = 5,
    MaxNumberOfFoldersExceed    = 6,
    FolderHierarchySyncRequired = 7,
    ErrorOccurredOnTheServer    = 8
    
  } PingStatus;
  
  class SASPingResponse: public SASCommandResponse {
    
  private:
    PingStatus                _pingStatus = HeartbeatIntervalExpired;
    int32_t                   _heartbeatInterval = 0;
    int32_t                   _maxFolder = 0;
    std::vector<std::string>  _changedFolderIds;
    
  public:
    const PingStatus &status() {return _pingStatus;}
    const std::vector<std::string> &changedFolderIds() {return _changedFolderIds;}
    const int32_t &maxFolder() {return _maxFolder;}
    const int32_t &heartbeatInterval() {return _heartbeatInterval;}
    
    const bool isChangedFound() { return _pingStatus == ChangesFound && _changedFolderIds.size() > 0; }
    
  public:
    SASPingResponse(SASHTTPRequest::SASHTTPResponseContext &ctx);
    ~SASPingResponse();
  };
}

