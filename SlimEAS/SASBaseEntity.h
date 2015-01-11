//
//  SASBaseEntity.h
//  SlimEAS
//
//  Created by Focus Lan on 1/4/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <string>

#include "SASDefine.h"

namespace SlimEAS {
  class SASBaseEntity {
  public:
    virtual const std::string encode() = 0;
    virtual void decode(const std::string &xml) = 0;
    
  public:
    SASBaseEntity();
    virtual ~SASBaseEntity() = 0;
  };
}
