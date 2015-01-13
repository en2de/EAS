//
//  SASGetAttachmentRequest.h
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
  class SASFetchAttachmentRequest: public SASItemOperationsRequest {
    
  public:
    SASFetchAttachmentRequest();
    ~SASFetchAttachmentRequest();
    
    // getter/setter
  public:
    
    void setFileReference(const std::string &fileReference) {_fileReference = fileReference;}
    const std::string &fileReference() {return _fileReference;}
    
  private:
    std::string       _fileReference; // the file reference. such as Attachment.
    
  protected:
    virtual void generateXMLSubPayload();
  };
}
