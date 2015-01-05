//
//  SASBody.h
//  SlimEAS
//
//  Created by Focus Lan on 1/4/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include <stdio.h>
#include <string>

#include "SASBaseEntity.h"

namespace SlimEAS {
  
  class SASBody : SASBaseEntity {
  private:
    
    int32_t         _type = 0;
    int32_t         _estimatedDataSize = 0;
    std::string     _mimeData;
    bool            _isTruncated;
    
  public:
    const int32_t &getType() {return _type;}
    void setType(const int32_t &val) { _type = val;}
    
    const int32_t &estimatedDataSize() {return _estimatedDataSize;}
    void setEstimatedDataSize(const int32_t &val) {_estimatedDataSize = val;}
    
    const std::string &mimeData() {return _mimeData;}
    void setMimeData(const std::string &val) {_mimeData = val;}
    
    const bool &isTruncated() {return _isTruncated;}
    void setTruncated(const bool &val) { _isTruncated = val;}

    virtual void decode(const std::string &xml);
    // instance to xml
    virtual const std::string encode();
  };
  
}

