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


#define SAS_PROPERTY_PROTECTED(type, name) \
protected: type _##name; \
public: type name() const {return _##name;} \
public: type name(const type value) \
{return _##name, _##name = value;}

#define SAS_GETTER_SYNTHESIZE(type, name) \
public: type name() const {return _##name;}

#define SAS_SETTER_SYNTHESIZE(type, name) \
public: void name() { _##name = name;}

#define SAS_GETTER(type, name) \
public: type name()

#define SAS_SETTER(t,p) \
public: void p(t val)

namespace SlimEAS {
  extern std::string toUpper(const std::string &name);
}