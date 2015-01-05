//
//  SASFolderSyncResponse.cpp
//  SlimEAS
//
//  Created by guo gloria on 15/1/5.
//  Copyright (c) 2015年 Slim. All rights reserved.
//

#include <iostream>

#include "SASFolderSyncResponse.h"

using namespace SlimEAS;

SASFolderSyncResponse::SASFolderSyncResponse(SASHTTPRequest::SASHTTPResponseContext &ctx)
: SASCommandResponse(ctx)
{
  std::cout << "FolderSync Response: " << _xmlResponse;
}

SASFolderSyncResponse::~SASFolderSyncResponse()
{
}