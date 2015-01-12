//
//  SASFolder.cpp
//  SlimEAS
//
//  Created by Focus Lan on 1/4/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASFolder.h"

#include <cassert>
#include <libxml/xmlreader.h>
#include <libxml/xmlsave.h>

using namespace std;
using namespace SlimEAS;

const static string _airSyncXmlns = "airsync";

const static string _airSyncNamespace = "AirSync";

const static char *_prefix = "airsync";

// The folder hierarchy is persisted as an XML file
// named FolderInfo.xml.
const static string _folderInfoFileName = "FolderInfo.xml";

// Since the root is not returned in a FolderSync response,
// we create it with the reserved name "Mailbox".
#define kMailBox "Mailbox"

// This is the basic XML structure used to initialize the XmlDocument
// if the hiearchy has not been synced before.
#define kEmptyFolderTree "<RootFolder><Path/><SyncKey/><LastSyncTime/><Folders/></RootFolder>"

#define STR2CharArray(val) (BAD_CAST val.c_str())
#define INT2CharArray(val) (BAD_CAST to_string(val).c_str())
#define WriteElementNS(eleName, value) xmlTextWriterWriteElementNS(writer, BAD_CAST _prefix, BAD_CAST eleName, nullptr, value)
#define StartElementNS(eleName) xmlTextWriterStartElementNS(writer, BAD_CAST _prefix, BAD_CAST eleName, nullptr)
#define WriteElement(eleName, value) xmlTextWriterWriteElement(writer, BAD_CAST eleName, value)
#define StartElement(eleName) xmlTextWriterStartElement(writer, BAD_CAST eleName)
#define EndElement() xmlTextWriterEndElement(writer)

SASFolder::SASFolder(const string &folderName, const string &folderId, FolderType folderType, SASFolder &parent)
: _name(folderName),
  _id(folderId),
  _type(folderType),
  _parentFolder(&parent)
{
  _saveLocation = _parentFolder->_saveLocation + "//" + _name;
}

SASFolder::SASFolder(const string &folderPath)
: _saveLocation(folderPath),
  _name(kMailBox),
  _id("0")
{
  _parentFolder = nullptr;
  checkDir(_saveLocation);
  loadFolderInfo();
}

const string SASFolder::generateXmlForPreference(const SlimEAS::BodyPreferences &bodyPreference, const std::string &prefix, const string &elementName) {
  xmlBufferPtr buf;
  buf = xmlBufferCreate();
  if (buf == NULL) {
    throw std::invalid_argument("Error creating the xml buffer");
  }
  
  xmlDocPtr doc;
  
  xmlTextWriterPtr writer;
  writer = xmlNewTextWriterDoc(&doc, 0);
  xmlTextWriterSetIndent(writer, 1);
  if (writer == NULL) {
    throw std::invalid_argument("Error creating the xml writer");
  }
  
  xmlTextWriterStartDocument(writer, nullptr, nullptr, nullptr);
  
  xmlTextWriterStartElementNS(writer, BAD_CAST prefix.c_str(), STR2CharArray(elementName), nullptr);
  
  xmlTextWriterWriteElementNS(writer, BAD_CAST prefix.c_str(), BAD_CAST "Type", nullptr, INT2CharArray(bodyPreference.Type));
  xmlTextWriterWriteElementNS(writer, BAD_CAST prefix.c_str(), BAD_CAST "TruncationSize", nullptr, INT2CharArray(bodyPreference.TruncationSize));
  xmlTextWriterWriteElementNS(writer, BAD_CAST prefix.c_str(), BAD_CAST "AllOrNone", nullptr, INT2CharArray(bodyPreference.AllOrNone ? 1 : 0));
  
  xmlTextWriterEndElement(writer); // end element for elementName
  
  xmlTextWriterEndDocument(writer);
  
  xmlSaveCtxtPtr saveCtx = xmlSaveToBuffer(buf, "utf-8", XML_SAVE_NO_DECL | XML_SAVE_FORMAT);
  xmlSaveDoc(saveCtx, doc);
  xmlSaveFlush(saveCtx);
  xmlSaveClose(saveCtx);
  
  string xml((char *)buf->content);
  
  xmlFreeTextWriter(writer);
  xmlFreeDoc(doc);
  xmlBufferFree(buf);
  
  printf("Preference payload:\n %s \n", xml.c_str());
  
  return xml;
}

const BodyPreferences SASFolder::generatePreferenceFromXml(const std::string &xml) {
  xmlTextReaderPtr reader;
  uint8_t *buf = (uint8_t *)xml.c_str();
  int buf_len = (int)strlen((const char *)buf);
  BodyPreferences preference;
  
  reader = xmlReaderForMemory((const char *)buf, buf_len, nullptr, "utf-8", 0);
  if (reader != NULL) {
    const xmlChar *curr_elm = nullptr;
    const xmlChar *curr_elm_prefix = nullptr;
    const xmlChar *curr_ele_full = nullptr;
    while (xmlTextReaderRead(reader)) {
      int n_type = xmlTextReaderNodeType(reader);
      if (n_type == XNT_Element) {
        curr_ele_full = xmlTextReaderConstName(reader);
        curr_elm = xmlTextReaderConstLocalName(reader);
        curr_elm_prefix = xmlTextReaderConstPrefix(reader);
        continue;
      } else if (n_type == XNT_Text && curr_elm != NULL) {
        
#define ToI32(v) sscanf((const char *)value, "%d", &v)
#define ToSTR(v) (string((const char*)v))
        
        const xmlChar *value = xmlTextReaderConstValue(reader);
        if (strcmp((const char *)curr_elm, "TruncationSize") == 0) {
          ToI32(preference.TruncationSize);
        }else if (strcmp((const char *)curr_elm, "AllOrNone") == 0) {
          preference.AllOrNone = true;
        } else if(strcmp((const char *)curr_elm, "PreviewSize") == 0 ){
          ToI32(preference.Preview);
        } else {
          // other property
        }
      } else if (n_type == XNT_EndElement) {
        curr_elm = nullptr;
      } else {
        continue;
      }
    }
    xmlFreeTextReader(reader);
  }
  return preference;
}

const SASFolder *SASFolder::findFolderById(const string &folderId) const {
  
  if (strcmp(folderId.c_str(), "0") == 0) {
    return rootFolder();
  }
  
  if (strcmp(folderId.c_str(), _id.c_str()) == 0) {
    return this;
  }
  
  if (_subFolders.size() == 0) {
    // how to return null
    return nullptr;
  }
  
  const SASFolder *returnFolder = nullptr;
  
  for (auto &it : _subFolders) {
    if (strcmp(folderId.c_str(), it._id.c_str()) == 0) {
      returnFolder = &it;
      break;
    } else {
      returnFolder = it.findFolderById(folderId);
      
      if (returnFolder != nullptr) {
        break;
      }
    }
  }
  
  return returnFolder;
}

void SASFolder::checkDir(const std::string &directory) {
  assert(!directory.empty());
  // TODO: create directory here, use shell to test
  string cmd  = "mkdir -p " + directory;
  system(cmd.c_str());
}

void SASFolder::remove() {
  if (_parentFolder != nullptr) {
    string cmd = "rm -rf " + _saveLocation;
    // TODO: delete directory here, use shell to test
    printf("Excute shell command : %s \n", cmd.c_str());
    _parentFolder->removeSubFolder(*this);
  }
}

void SASFolder::removeSubFolder(const SASFolder &removeFolder) {
  
  for(auto it = _subFolders.begin(); it != _subFolders.end();)
  {
    if(it->id() == removeFolder.id())
    {
      it = _subFolders.erase(it);
    } else {
      it++;
    }
  }

}

void SASFolder::removeAllSubFolders() {
  for(auto it : _subFolders) {
    it.remove();
  }
}

void SASFolder::update(const std::string &folderName, const FolderType folderType, SASFolder &parent)
{
  _name = folderName;
  _type = folderType;
  
  if (parent.id() != _parentFolder->id()) {
    parent.addSubFolder(this);
    _parentFolder->removeSubFolder(*this);
    _parentFolder = &parent;
  }
  
  // TODO: move to new location
}

void SASFolder::addSubFolder(const SASFolder *folder)

{
  if ( findFolderById(folder->id()) == nullptr) {
    _subFolders.push_back(*folder);
  }
}


void SASFolder::saveFolderInfo() {

  assert(strcmp(_id.c_str(), "0") == 0);
  
  xmlBufferPtr buf;
  buf = xmlBufferCreate();
  if (buf == NULL) {
    throw std::invalid_argument("Error creating the xml buffer");
  }
  
  xmlDocPtr doc;
  
  xmlTextWriterPtr writer;
  writer = xmlNewTextWriterDoc(&doc, 0);
  //  writer = xmlNewTextWriterMemory(buf, 0);
  xmlTextWriterSetIndent(writer, 1);
  if (writer == NULL) {
    throw std::invalid_argument("Error creating the xml writer");
  }
  
  xmlTextWriterStartDocument(writer, nullptr, nullptr, nullptr);
  StartElement("RootFolder");
  
  WriteElement("Path", STR2CharArray(_name));
  WriteElement("SyncKey", STR2CharArray(_syncKey));
  WriteElement("LastSyncTime", INT2CharArray(_lastSyncTime));
  
  StartElement("Folders");
  
  for (auto it : _subFolders) {
    string xmlForFolder = it.generateXmlForFolder();
    xmlTextWriterWriteRaw(writer, BAD_CAST xmlForFolder.c_str());
  }
  
  EndElement();// end for Folders
  
  EndElement(); // end for "RootFolder"
  
  xmlTextWriterEndDocument(writer);
  
  string filePath = _saveLocation + "//" + _folderInfoFileName;
  printf("saved to %s \n", filePath.c_str());
  xmlSaveCtxtPtr save2FileCtx = xmlSaveToFilename(filePath.c_str(), "utf-8", XML_SAVE_NO_DECL | XML_SAVE_FORMAT);
  xmlSaveDoc(save2FileCtx, doc);
  xmlSaveFlush(save2FileCtx);
  xmlSaveClose(save2FileCtx);
  
  xmlSaveCtxtPtr saveCtx = xmlSaveToBuffer(buf, "utf-8", XML_SAVE_NO_DECL | XML_SAVE_FORMAT);
  xmlSaveDoc(saveCtx, doc);
  xmlSaveFlush(saveCtx);
  xmlSaveClose(saveCtx);
  
  string xml((char *)buf->content);
  
  xmlFreeTextWriter(writer);
  xmlFreeDoc(doc);
  xmlBufferFree(buf);
  
  printf("saveFolderInfo payload:\n %s \n", xml.c_str());
  
}

void SASFolder::loadFolderInfo() {

  assert(strcmp(_id.c_str(), "0") == 0);
  xmlTextReaderPtr reader = nullptr;
  string fileName = _saveLocation + "//" + _folderInfoFileName;
  const xmlChar *curr_elm = nullptr;
  
  reader = xmlReaderForFile(fileName.c_str(), nullptr, 0);
  if (reader == nullptr) {
    reader = xmlReaderForMemory(kEmptyFolderTree, strlen(kEmptyFolderTree), nullptr, "utf-8", 0);
  }
  
  if (reader != nullptr) {
    while (xmlTextReaderRead(reader)) {
      int n_type = xmlTextReaderNodeType(reader);
      if (n_type == XNT_Element) {
        curr_elm = xmlTextReaderConstName(reader);
        printf("current element %s \n", curr_elm);
        continue;
      } else if (n_type == XNT_Text && curr_elm != nullptr) {

#define ToU8(v) sscanf((const char *)value, "%c", &v)
#define ToBool(v) do { \
uint8_t _  = 0; \
ToU8(_); \
v = _ == 0? false : true; \
} while(0)
#define ToI32(v) sscanf((const char *)value, "%d", &v)
#define ToI64(v) sscanf((const char *)value, "%lld", &v)
#define ToU32(v) sscanf((const char *)value, "%u", &v)
        const xmlChar *value = xmlTextReaderConstValue(reader);
        if (strcmp((const char *)curr_elm, "SyncKey") == 0) {
          _syncKey = string((const char *)value);
          if (_syncKey.empty()) {
            _syncKey = "0";
          }
        } else if (strcmp((const char *)curr_elm, "LastSyncTime") == 0) {
          if (strlen((const char *)value) == 0) {
            _lastSyncTime = INT64_MIN;
          }
          else {
            ToI64(_lastSyncTime);
          }
        } else if(strcmp((const char *)curr_elm, "Folder") == 0) {
          
        }
        
      }
    }
  }
  
  if (reader) {
    xmlFreeTextReader(reader);
  }
  reader = nullptr;
  
}

string SASFolder::generateXmlForFolder() {

  xmlBufferPtr buf;
  buf = xmlBufferCreate();
  if (buf == NULL) {
    throw std::invalid_argument("Error creating the xml buffer");
  }
  
  xmlDocPtr doc;
  
  xmlTextWriterPtr writer;
  writer = xmlNewTextWriterDoc(&doc, 0);
  //  writer = xmlNewTextWriterMemory(buf, 0);
  xmlTextWriterSetIndent(writer, 1);
  if (writer == NULL) {
    throw std::invalid_argument("Error creating the xml writer");
  }
  
  xmlTextWriterStartDocument(writer, nullptr, nullptr, nullptr);
  StartElement("Folder");
  
  WriteElement("Name", STR2CharArray(_name));
  WriteElement("Id", STR2CharArray(_id));
  WriteElement("Type", INT2CharArray(_type));
  WriteElement("SyncKey", STR2CharArray(_syncKey));
  WriteElement("LastSyncTime", INT2CharArray(_lastSyncTime));
  
  if (_areDeletesPermanent) {
    StartElement("PermanentDelete");
    EndElement();
  }
  
  if (_areChangesIgnored) {
    StartElement("IgnoreServerChanges");
    EndElement();
  }
  
  if (_areDeletesPermanent) {
    StartElement("ConversationMode");
    EndElement();
  }
  
  if (_windowSize > 0) {
    WriteElement("WindowSize", INT2CharArray(_windowSize));
  }
  
  if (_hasOptions) {
    if (_options.filterType != NoFilter) {
      WriteElement("FilterType",
                                INT2CharArray(_options.filterType));
    }
    
    if (_options.conflictHandling != LetServerDecide) {
      WriteElement("ConflictHandling",
                                INT2CharArray(_options.conflictHandling));
    }
    
    if (_options.mimeTruncation != NoTruncate) {
      WriteElement("MimeTruncation",
                                INT2CharArray(_options.mimeTruncation));
    }
    
    if (!_options.className.empty()) {
      WriteElement("ClassToSync",
                                STR2CharArray(_options.className));
    }
    
    if (_options.maxItems > -1) {
      WriteElement("MaxItems",
                                INT2CharArray(_options.maxItems));
    }
    
    if (_options.bodyPreferences.size() >= 1 ) {
      BodyPreferences bodyPreferences = _options.bodyPreferences[0];
      StartElement("BodyPreferences");
      xmlTextWriterWriteString(writer, INT2CharArray(bodyPreferences.Type));
      
      if (bodyPreferences.TruncationSize > 0) {
        xmlTextWriterWriteAttribute(writer, BAD_CAST "TruncationSize", INT2CharArray(bodyPreferences.Type));
      }
      
      if (bodyPreferences.AllOrNone == true) {
        xmlTextWriterWriteAttribute(writer, BAD_CAST "AllOrNone", INT2CharArray(1));
      }
      
      if (bodyPreferences.Preview > -1) {
        xmlTextWriterWriteAttribute(writer, BAD_CAST "PreviewSize", INT2CharArray(bodyPreferences.Preview));
      }
      
      EndElement();
    }
    
    if (_options.hasBodyPreferences()) {
      StartElement("BodyPartPreferences");
      
      xmlTextWriterWriteString(writer, INT2CharArray(_options.bodyPreference().Type));
      
      if (_options.bodyPreference().TruncationSize > 0) {
        xmlTextWriterWriteAttribute(writer, BAD_CAST "TruncationSize", INT2CharArray(_options.bodyPreference().TruncationSize));
      }
      
      if (_options.bodyPreference().AllOrNone == true) {
        xmlTextWriterWriteAttribute(writer, BAD_CAST "AllOrNone", INT2CharArray(1));
      }
      
      if (_options.bodyPreference().Preview > -1) {
        xmlTextWriterWriteAttribute(writer, BAD_CAST "PreviewSize", INT2CharArray(_options.bodyPreference().Preview));
      }
      
      EndElement();
    }
    
    if (_options.isRightsManagementSupported) {
      StartElement("RightsManagementSupported");
      EndElement();
    }
    
    if (_options.mimeSupportLevel != NeverSendMime) {
      WriteElement("MimeSupport", INT2CharArray(_options.mimeSupportLevel));
    }
  }
 
  /* need to check this code snippet start */
  StartElement("Folders");
  
  for (auto it : _subFolders) {
    string subFoderXml = it.generateXmlForFolder();
    if (!subFoderXml.empty()) {
      xmlTextWriterWriteRaw(writer, BAD_CAST subFoderXml.c_str());
    }
  }
  
  EndElement();
  /* need to check this code snippet end */
  
  EndElement();
  xmlTextWriterEndDocument(writer);

  xmlSaveCtxtPtr saveCtx = xmlSaveToBuffer(buf, "utf-8", XML_SAVE_NO_DECL | XML_SAVE_FORMAT);
  xmlSaveDoc(saveCtx, doc);
  xmlSaveFlush(saveCtx);
  xmlSaveClose(saveCtx);
  
  string xml((char *)buf->content);
  
  xmlFreeTextWriter(writer);
  xmlFreeDoc(doc);
  xmlBufferFree(buf);

  printf("Folder payload:\n %s \n", xml.c_str());
  
  return xml;
}

// folder xml to folder instance
void SASFolder::addSubFolderFromXml(const string &folderXml) {

  xmlTextReaderPtr reader;
  uint8_t *buf = (uint8_t *)folderXml.c_str();
  int buf_len = (int)strlen((const char *)buf);
  
  reader = xmlReaderForMemory((const char *)buf, buf_len, nullptr, "utf-8", 0);
  if (reader != NULL) {
    const xmlChar *curr_elm = nullptr;
    const xmlChar *curr_elm_prefix = nullptr;
    const xmlChar *curr_ele_full = nullptr;
    SASFolder folder("");
    string xml = "";
    vector<string> subFolderXmls;
    
    while (xmlTextReaderRead(reader)) {
      int n_type = xmlTextReaderNodeType(reader);
      if (n_type == XNT_Element) {
        curr_ele_full = xmlTextReaderConstName(reader);
        curr_elm = xmlTextReaderConstLocalName(reader);
        curr_elm_prefix = xmlTextReaderConstPrefix(reader);
        printf("current element %s \n", curr_ele_full);
        
        if (strcmp((const char *)curr_elm, "BodyPreferences") == 0) {
          xml = string((const char*)xmlTextReaderReadOuterXml(reader));
          folder._options.bodyPreferences.push_back(generatePreferenceFromXml(xml));
          xmlTextReaderNext(reader);
        } else if (strcmp((const char *)curr_elm, "BodyPartPreferences") == 0) {
          xml = string((const char*)xmlTextReaderReadOuterXml(reader));
          folder._options.setBodyPreferences(generatePreferenceFromXml(xml));
          xmlTextReaderNext(reader);
        } else if (strcmp((const char *)curr_elm, "Folder") == 0) {
          int depth = xmlTextReaderDepth(reader);
          if (depth > 0) {
            xml = string((const char*)xmlTextReaderReadOuterXml(reader));
            if (!xml.empty()) {
              subFolderXmls.push_back(xml);
            }
          }
          xmlTextReaderNext(reader);
        }
        continue;
      } else if (n_type == XNT_Text && curr_elm != NULL) {
    
#define ToI32(v) sscanf((const char *)value, "%d", &v)
#define ToSTR(v) (string((const char*)v))
        
        const xmlChar *value = xmlTextReaderConstValue(reader);
        if (strcmp((const char *)curr_elm, "Name") == 0) {
          folder._name = ToSTR(value);
        }else if (strcmp((const char *)curr_elm, "Id") == 0) {
          folder._id = ToSTR(value);
        } else if(strcmp((const char *)curr_elm, "Type") == 0 ) {
           ToI32(folder._type);
        } else if(strcmp((const char *)curr_elm, "SyncKey") == 0 ){
          folder._syncKey = ToSTR(value);
        } else if(strcmp((const char *)curr_elm, "LastSyncTime") == 0) {
          struct tm tm;
          time_t now;
          strptime((const char*)value, "%Y-%m-%dT%T", &tm);
          
          now = mktime(&tm);
          folder._lastSyncTime = now;
          
        } else if(strcmp((const char *)curr_elm, "PermanentDelete") == 0) {
          folder._areDeletesPermanent =  true;
        } else if(strcmp((const char *)curr_elm, "IgnoreServerChanges") == 0) {
          folder._areChangesIgnored = true;
        } else if(strcmp((const char *)curr_elm, "ConversationMode") == 0) {
          folder._useConversationMode = true;
        } else if(strcmp((const char *)curr_elm, "WindowSize") == 0) {
          ToI32(folder._windowSize);
        } else if(strcmp((const char *)curr_elm, "FilterType") == 0) {
          ToI32(folder._options.filterType);
        } else if(strcmp((const char *)curr_elm, "ConflictHandling") == 0) {
          ToI32(folder._options.conflictHandling);
        } else if(strcmp((const char *)curr_elm, "MimeTruncation") == 0) {
          ToI32(folder._options.mimeTruncation);
        } else if(strcmp((const char *)curr_elm, "ClassToSync") == 0) {
          folder._options.className = ToSTR(value);
        } else if(strcmp((const char *)curr_elm, "MaxItems") == 0) {
          ToI32(folder._options.maxItems);
        } else if(strcmp((const char *)curr_elm, "RightsManagementSupported") == 0) {
          folder._options.isRightsManagementSupported = true;
        } else if(strcmp((const char *)curr_elm, "MimeSupport") == 0) {
          ToI32(folder._options.mimeSupportLevel);
        } else if(strcmp((const char *)curr_elm, "MaxItems") == 0) {
          ToI32(folder._options.maxItems);
        } else {
          // other property
        }
      } else if (n_type == XNT_EndElement) {
        curr_elm = nullptr;
      } else {
        continue;
      }
    }
    
    if (!folder._id.empty()) {
      // different constructor, copy properties
      SASFolder newFolder(folder._name, folder._id, folder._type, *this);
      newFolder._options = folder._options;
      newFolder._hasOptions = true;
      newFolder._syncKey = folder._syncKey;
      newFolder._lastSyncTime = folder._lastSyncTime;
      newFolder._areDeletesPermanent = folder._areDeletesPermanent;
      newFolder._areChangesIgnored = folder._areChangesIgnored;
      newFolder._useConversationMode = folder._useConversationMode;
      newFolder._windowSize  = folder._windowSize;
      _subFolders.push_back(newFolder);
      
      for(auto it : subFolderXmls) {
        newFolder.addSubFolderFromXml(it);
      }
    }
    
    xmlFreeTextReader(reader);
  }
  
}

string SASFolder::generateOptionsXml() {

   xmlBufferPtr buf;
   buf = xmlBufferCreate();
   if (buf == NULL) {
   throw std::invalid_argument("Error creating the xml buffer");
   }
   
   xmlDocPtr doc;
   
   xmlTextWriterPtr writer;
   writer = xmlNewTextWriterDoc(&doc, 0);
   //  writer = xmlNewTextWriterMemory(buf, 0);
   xmlTextWriterSetIndent(writer, 1);
   if (writer == NULL) {
   throw std::invalid_argument("Error creating the xml writer");
   }
  
  StartElementNS("Options");
  
  if (_hasOptions) {
    if (_options.filterType != NoFilter) {
      WriteElementNS("FilterType", INT2CharArray(_options.filterType));
    }
    
    if (_options.mimeTruncation != NoTruncate) {
      WriteElementNS("MimeTruncation", INT2CharArray(_options.mimeTruncation));
    }
    
    if (!_options.className.empty()) {
      WriteElementNS("Class", STR2CharArray(_options.className));
    }
    
    if (_options.maxItems > -1) {
      WriteElementNS("MaxItems", INT2CharArray(_options.maxItems));
    }
    
    if (_options.bodyPreferences.size() >= 1 ) {
      BodyPreferences bodyPreferences = _options.bodyPreferences[0];
      StartElementNS("BodyPreference");
      WriteElementNS("Type", INT2CharArray(bodyPreferences.Type));
      
      if (bodyPreferences.TruncationSize > 0) {
        WriteElementNS("TruncationSize", INT2CharArray(bodyPreferences.Type));
      }
      
      if (bodyPreferences.AllOrNone == true) {
        WriteElementNS("AllOrNone", INT2CharArray(1));
      }
      
      if (bodyPreferences.Preview > -1) {
        WriteElementNS("PreviewSize", INT2CharArray(bodyPreferences.Preview));
      }
      
      EndElement();
    }
    
    if (_options.hasBodyPreferences()) {
      StartElementNS("BodyPartPreference");
      
      WriteElementNS("Type", INT2CharArray(_options.bodyPreference().Type));
      
      if (_options.bodyPreference().TruncationSize > 0) {
        WriteElementNS("TruncationSize", INT2CharArray(_options.bodyPreference().TruncationSize));
      }
      
      if (_options.bodyPreference().AllOrNone == true) {
        WriteElementNS("AllOrNone", INT2CharArray(1));
      }
      
      if (_options.bodyPreference().Preview > -1) {
        WriteElementNS("PreviewSize", INT2CharArray(_options.bodyPreference().Preview));
      }
      
      EndElement();
    }
    
    if (_options.isRightsManagementSupported) {
      StartElementNS("RightsManagementSupported");
      EndElement();
    }
    
    if (_options.mimeSupportLevel != NeverSendMime) {
      WriteElementNS("MimeSupport", INT2CharArray(_options.mimeSupportLevel));
    }
  }
  
  EndElement();
  xmlTextWriterEndDocument(writer);
  
   xmlSaveCtxtPtr saveCtx = xmlSaveToBuffer(buf, "utf-8", XML_SAVE_NO_DECL | XML_SAVE_FORMAT);
   xmlSaveDoc(saveCtx, doc);
   xmlSaveFlush(saveCtx);
   xmlSaveClose(saveCtx);
   
   string xml((char *)buf->content);
   
   xmlFreeTextWriter(writer);
   xmlFreeDoc(doc);
   xmlBufferFree(buf);

  printf("Options payload:\n %s \n", xml.c_str());
  
  return xml;
}

const SASFolder *SASFolder::rootFolder() const {
  if (_parentFolder != nullptr) {
    return _parentFolder->rootFolder();
  } else {
    return this;
  }
}

SASFolder::~SASFolder() {

  
}