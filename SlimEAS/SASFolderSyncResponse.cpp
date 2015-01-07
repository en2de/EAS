//
//  SASFolderSyncResponse.cpp
//  SlimEAS
//
//  Created by guo gloria on 15/1/5.
//  Copyright (c) 2015年 Slim. All rights reserved.
//

#include <iostream>
#include <stack>


#include "SASFolderSyncResponse.h"


using namespace SlimEAS;
static const int FOLDER_LIST_COUNT = 15;

// TODO:
SASFolderSyncResponse::SASFolderSyncResponse(SASHTTPRequest::SASHTTPResponseContext &ctx)
: SASCommandResponse(ctx)
{
  std::cout << "FolderSync Response: " << _xmlResponse;
  
  
  const char *buf = _xmlResponse.c_str();
  int bufLen = strlen(buf);
  _reader = xmlReaderForMemory(buf, bufLen, NULL, "utf-8", 0);
  //_reader = xmlReaderForFile("/Users/gy/Workspace/Slim-EAS/tmp/test.xml", "utf-8", 0);
  if (_reader == NULL) {
    throw std::invalid_argument("create xml reader failed!");
  }
  
  while (xmlTextReaderNext(_reader)) {
    buf = (const char *)xmlTextReaderReadInnerXml(_reader);
    //std::cout << "buf:" << buf << std::endl;

    
    if (buf != nullptr && strlen(buf) != 0) {
      if (parseInnerNode() == false ) {
        break;
      }      
    }
  }
}

void SASFolderSyncResponse::updateRootFolder(SASFolder *rootFolder)
{
  if (rootFolder == nullptr) {
    throw std::invalid_argument("Not valid root folder.");
  }
  _rootFolder = rootFolder;
  parseInnerNode();
}

bool SASFolderSyncResponse::parseInnerNode()
{
  const char *currentElement = NULL;
  enum {
    SASFolderSyncOperationUndefined = 0,
    SASFolderSyncOperationAdd,
    SASFolderSyncOperationDelete,
    SASFolderSyncOperationUpdate,
    SASFolderSyncOperationNA
  };
  int operation = SASFolderSyncOperationUndefined;
  std::string serverId;
  std::string parentId;
  std::string displayName;
  int type = 0;
  
  while(xmlTextReaderRead(_reader)) {
    int nodeType = xmlTextReaderNodeType(_reader);
    switch (nodeType) {
      case XNT_Element:
      {
        currentElement = (const char *)xmlTextReaderConstName(_reader);
        
        if (currentElement != NULL) {
          if (strcmp(currentElement, "Add") == 0) {
            operation = SASFolderSyncOperationAdd;
          } else if (strcmp(currentElement, "Delete") == 0) {
            operation = SASFolderSyncOperationDelete;
          } else if (strcmp(currentElement, "Update") == 0) {
            operation = SASFolderSyncOperationUpdate;
          } else {
            operation = SASFolderSyncOperationUndefined;
          }
        }
      }
        break;
        
      case XNT_Text:
        if (currentElement != NULL) {
          const char *value = (const char *)xmlTextReaderConstValue(_reader);
          std::cout << "element: " << currentElement << ", value: " << value << std::endl;
          
          if (strcmp(currentElement, "Status") == 0) {
            sscanf(value, "%d", &_status);
            return false;
          } else if (strcmp(currentElement, "SyncKey") == 0) {
            _syncKey = std::string(value);

          } else if (strcmp(currentElement, "ServerId") == 0) {
            serverId = std::string(value);
          }else if (strcmp(currentElement, "ParentId") == 0) {
            parentId = std::string(value);
          }else if (strcmp(currentElement, "DisplayName") == 0) {
            displayName = std::string(value);
          }else if (strcmp(currentElement, "Type") == 0) {
            sscanf(value, "%d", &type);
          }
        }
        break;
      case XNT_EndElement:
      {
        const char *end = (const char *)xmlTextReaderConstName(_reader);
        //std::cout << "end element: " << end << std::endl;
        
        SASFolder * parent = nullptr;
        if (strcmp(end, "Add") == 0) {
          if (parentId == "0") {
            parent = _rootFolder;
          } else {
            parent = const_cast<SASFolder *>(_rootFolder->findFolderById(parentId));
          }
          SASFolder folder(displayName, serverId, (FolderType)type, *parent);
          parent->addSubFolder(folder);
          operation = SASFolderSyncOperationUndefined;
        } else if (strcmp(end, "Delete") == 0) {
          // TODO:
        } else if (strcmp(end, "Update") == 0) {
          // TODO:
        }

        currentElement = NULL;
      }
        break;
        
      default:
        break;
    }
  }
  return true;
}


SASFolderSyncResponse::~SASFolderSyncResponse()
{
}