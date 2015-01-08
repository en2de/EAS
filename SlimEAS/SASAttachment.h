//
//  SASAttachment.h
//  SlimEAS
//
//  Created by Focus Lan on 1/4/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include <stdio.h>
#include <string>

#include "SASBaseEntity.h"

namespace SlimEAS {
  
  class SASAttachment : SASBaseEntity {
  private:
    std::string     _displayName;
    std::string     _fileReference;
    std::string     _method;
    int32_t         _estimatedDataSize;
    std::string     _contentId;
    std::string     _contentLocation;
    bool            _isInline = false;
    std::string     _contentType;
    std::string     _data;
    
    // property under email2 namespace:
    
  public:
    const std::string &getContentType() {return _contentType;}
    void setContentType(const std::string &val) { _contentType = val;}
    
    const std::string &data() {return _data;}
    void setData(const std::string &val) {_data = val;}
    
    const std::string &fileReference() {return _fileReference;}
    void setFileReference(const std::string &val) {_fileReference = val;}
    
    const std::string &method() {return _method;}
    void setMethod(const std::string &val) {_method = val;}
    
    const int32_t &estimatedDataSize() {return _estimatedDataSize;}
    void setEstimatedDataSize(const int32_t &val) {_estimatedDataSize = val;}
    
    const std::string &contentId() {return _contentId;}
    void  setContentId(const std::string &val) {_contentId = val;}
    
    const std::string &displayName() {return _displayName;}
    void  setDisplayName(const std::string &val) {_displayName = val;}
    
    const std::string &contentLocation() {return _contentLocation;}
    void  setContentLocation(const std::string &val) {_contentLocation = val;}
    
    virtual void decode(const std::string &xml);
    // instance to xml
    virtual const std::string encode();
    
    SASAttachment();
    ~SASAttachment();
  };
  
}

