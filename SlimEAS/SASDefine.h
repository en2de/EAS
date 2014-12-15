//
//  SASDefine.h
//  SlimEAS
//
//  Created by envy.chen on 12/15/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//


#pragma once

#define SAS_PROPERTY(type, name) \
private: type _##name; \
public: type name() const {return _##name;} \
public: type name(const type value) \
{return _##name, _##name = value;}
