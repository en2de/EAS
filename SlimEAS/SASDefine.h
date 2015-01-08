//
//  SASDefine.h
//  SlimEAS
//
//  Created by envy.chen on 12/15/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//


#pragma once

#define SAS_PROPERTY_SYNTHESIZE(type, name) \
type name() const {return _##name;} \
void name(const type value) \
{return _##name, _##name = value;}

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
  
  static std::string CLASS_EMAIL = "Email";
  static std::string CLASS_CALENDAR = "Calendar";
  static std::string CLASS_CONTACTS = "Contacts";
  static std::string CLASS_TASKS = "Tasks";
  static std::string CLASS_NOTES = "Notes";
    
  //use for xml reader
  //definition http://www.gnu.org/software/dotgnu/pnetlib-doc/System/Xml/XmlNodeType.html
  typedef enum {
    XNT_None = 0,
    
    XNT_Attribute = 2,
    XNT_CDATA = 4,
    XNT_Comment = 8,
    XNT_Document = 9,
    XNT_DocumentFragment = 11,
    XNT_DocumentType = 10,
    XNT_Element = 1,
    XNT_EndElement = 15,
    XNT_Entity = 6,
    XNT_EntityReference = 5,
    XNT_EndEntity = 16,
    XNT_Notation = 12,
    XNT_ProcessingInstruction = 7,
    XNT_SignificantWhitespace = 14,
    XNT_Text = 3,
    XNT_Whitespace = 13,
    XNT_Declaration = 17,
  } XmlNodeType;
  
}