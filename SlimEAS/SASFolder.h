//
//  SASFolder.h
//  SlimEAS
//
//  Created by Focus Lan on 1/4/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#pragma once

#include <stdio.h>
#include <string>
#include <vector>

#include <libxml/xmlwriter.h>

#include "SASDefine.h"

namespace SlimEAS {
  
  // This enumeration covers the allowable
  // body types specified in MS-ASAIRS section
  // 2.2.2.22
  typedef enum {
    NoType = 0,
    PlainText,
    HTML,
    RTF,
    MIME
  } BodyType;
  
  // This enumeration covers the available
  // sync filter types specified in MS-ASCMD
  // section 2.2.3.64.2
  typedef enum {
    NoFilter = 0,
    OneDayBack,
    ThreeDaysBack,
    OneWeekBack,
    TwoWeeksBack,
    OneMonthBack,
    ThreeMonthsBack,
    SixMonthsBack,
    IncompleteTasks
  } SyncFilterType;
  
  // This enumeration covers the possible
  // values for the Conflict element specified
  // in MS-ASCMD 2.2.3.34
  typedef enum {
    KeepClientVersion = 0,
    KeepServerVersion,
    LetServerDecide
  } ConflictResolution;
  
  // This enumeration covers the possible
  // values for the MIMETruncation element
  // specified in MS-ASCMD section 2.2.3.101
  typedef enum {
    TruncateAll = 0,
    Truncate4k,
    Truncate5k,
    Truncate7k,
    Truncate10k,
    Truncate20k,
    Truncate50k,
    Truncate100k,
    NoTruncate
  } MimeTruncationType;
  
  // This enumeration covers the possible
  // values for the MIMESupport element
  // specified in MS-ASCMD section 2.2.3.100.3
  typedef enum {
    NeverSendMime = 0,
    SendMimeForSMime,
    SendMimeForAll
  } MimeSupport;
  
  // This class represents body or body part
  // preferences included in a <BodyPreference> or
  // <BodyPartPreference> element.
  class BodyPreferences {
  public:
    BodyType  Type            = NoType;
    uint32_t  TruncationSize  = 0;
    bool      AllOrNone       = false;
    uint32_t  Preview         = -1;
  };
  
  // This class represents the sync options
  // that are included under the <Options> element
  // in a Sync command request.
  class FolderSyncOptions {
  public:
    SyncFilterType                filterType = NoFilter;
    ConflictResolution            conflictHandling = LetServerDecide;
    MimeTruncationType            mimeTruncation = NoTruncate;
    std::string                   className;
    int32_t                       maxItems = -1;
    std::vector<BodyPreferences>  bodyPreferences;
    bool                          isRightsManagementSupported = false;
    MimeSupport                   mimeSupportLevel = NeverSendMime;
    
    void setBodyPreferences(const BodyPreferences &bodyPreferences) {
      _bodyPreference = bodyPreferences;
      _hasBodyPreference = true;
    }
    
    const BodyPreferences &bodyPreference() {
      return _bodyPreference;
    }
    
    bool hasBodyPreferences() {
      return _hasBodyPreference;
    }
    
  private:
    BodyPreferences               _bodyPreference;
    bool                          _hasBodyPreference = false;
  };
  
  // This enumeration indicates the folder type, and is based on
  // the allowed values for the Type element specified in
  // [MS-ASCMD] section 2.2.3.170.3.
  typedef enum {
    UserGeneric = 1,
    DefaultInbox,
    DefaultDrafts,
    DefaultDeletedItems,
    DefaultSentItems,
    DefaultOutbox,
    DefaultTasks,
    DefaultCalendar,
    DefaultContacts,
    DefaultNotes,
    DefaultJournal,
    UserMail,
    UserCalendar,
    UserContacts,
    UserTasks,
    UserJournal,
    UserNotes,
    Unknown,
    RecipientInfoCache
  } FolderType;
  
  // This class represents a single folder
  // in a mailbox. It is also used to represent
  // the root of the mailbox's folder hierarchy
  class SASFolder {
  private:
    #pragma mark - properties
    // The name of the folder
    std::string _name = "";
    
    // The id of the folder
    std::string _id = "";
    
    // The type of the folder
    FolderType _type = Unknown;
    
    // The location on disk where this folder is saved
    std::string _saveLocation = "";
    
    // A list of subfolders
    std::vector<SASFolder> _subFolders;
    
    // The current sync key for this folder
    std::string _syncKey = "0";
    
    // The last time the contents of this folder were synced
    int64_t _lastSyncTime;
    
    // Should items deleted from this folder
    // be permanently deleted?
    bool _areDeletesPermanent;
    
    // Should changes be ignored?
    bool _areChangesIgnored;
    
    // The max number of changes that should be
    // returned in a sync.
    int32_t _windowSize = 0;
    
    // Conversation mode
    bool _useConversationMode = false;
    
    // Optional sync options
    FolderSyncOptions _options;
    bool  _hasOptions = false;
    
    // The Folder object that represents this folder's parent
    const SlimEAS::SASFolder *_parentFolder;
    
  private:
    void checkDir(const std::string &directory);
    
  public:
    #pragma mark - constructor
    SASFolder(const std::string &folderName, const std::string &folderId, FolderType folderType, const SASFolder &parent);
    SASFolder(const std::string &folderPath);
    ~SASFolder();
    
  public:
    void loadFolderInfo();
    void saveFolderInfo();
    
    const SASFolder &findFolderById(const std::string &folderId);
    
    std::string generateOptionsXml();
    std::string generateXmlForFolder();
    
    void setFolderSyncOptions(const FolderSyncOptions &options) {
      _hasOptions = true;
      _options = options;
    }
    
    SASFolder addSubFolder(const std::string &folderName, const std::string &folderId, FolderType folderType);
    
    void addSubFolder(const SASFolder &folder) {
      _subFolders.push_back(folder);
    }
  };

}