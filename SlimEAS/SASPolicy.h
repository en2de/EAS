//
//  SASPolicy.h
//  SlimEAS
//
//  Created by envy.chen on 12/30/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#pragma once

#include <string>

#include "SASDefine.h"

namespace SlimEAS {
  class SASPolicy {
  public:
#pragma mark - Policy Enums
    typedef enum {
      TripleDES = 0,
      DES = 1,
      RC2_128bit = 2,
      RC2_64bit = 3,
      RC2_40bit = 4
    } SASEncryptionAlgorithm;
  
    typedef enum {
      SHA1 = 0,
      MD5 = 1
    } SASSigningAlgorithm;
    
    typedef enum {
      CalendarAge_all = 0,
      CalendarAge_two_weeks = 4,
      CalendarAge_one_month = 5,
      CalendarAge_three_months = 6,
      CalendarAge_six_months = 7
    } SASCalendarAgeFilter;
    
    typedef enum {
      MailAge_all = 0,
      MailAge_one_day = 1,
      MailAge_three_days = 2,
      MailAge_one_week = 3,
      MailAge_two_weeks = 4,
      MailAge_one_month = 5
    } SASMailAgeFilter;
  
    typedef enum {
      Policy_success = 1,
      Policy_no_defined = 2,
      Policy_unknown = 3,
      Policy_data_corrupt = 4,
      Policy_key_mismatch = 5
    } SASPolicyStatus;
  
  private:
    
  public:
#pragma mark - properties
    int32_t status = 0;
    uint32_t policyKey = 0;
    uint8_t allowBlueTooth = 0;
    bool allowBrowser = false;
    bool allowCamera = false;
    bool allowConsumerEmail = false;
    bool allowDesktopSync = false;
    bool allowHTMLEmail = false;
    bool allowInternetSharing = false;
    bool allowIrDA = false;
    bool allowPOPIMAPEmail = false;
    bool allowRemoteDesktop = false;
    bool allowSimpleDevicePassword = false;
    int32_t allowSMIMEEncryptionAlgorithmNegotiation = 0;
    bool allowSMIMESoftCerts = false;
    bool allowStorageCard = false;
    bool allowTextMessaging = false;
    bool allowUnsignedApplications = false;
    bool allowUnsignedInstallationPackages = false;
    bool allowWifi = false;
    bool alphanumericDevicePasswordRequired = false;
    bool attachmentsEnabled = false;
    bool devicePasswordEnabled = false;
    uint32_t devicePasswordExpiration = 0;
    uint32_t devicePasswordHistory = 0;
    uint32_t maxAttachmentSize = 0;
    uint32_t maxCalendarAgeFilter = 0;
    uint32_t maxDevicePasswordFailedAttempts = 0;
    uint32_t maxEmailAgeFilter = 0;
    int32_t maxEmailBodyTruncationSize = -1;
    int32_t maxEmailHTMLBodyTruncationSize = -1;
    uint32_t maxInactivityTimeDeviceLock = 0;
    uint8_t minDevicePasswordComplexCharacters = 1;
    uint8_t minDevicePasswordLength = 1;
    bool passwordRecoveryEnabled = false;
    bool requireDeviceEncryption = false;
    bool requireEncryptedSMIMEMessages = false;
    int32_t requireEncryptionSMIMEAlgorithm = 0;
    bool requireManualSyncWhenRoaming = false;
    int32_t requireSignedSMIMEAlgorithm = 0;
    bool requireSignedSMIMEMessages = false;
    bool requireStorageCardEncryption = false;
    
    
#pragma mark - constructor
    SASPolicy();
    ~SASPolicy();
    
    bool loadXml(const std::string &xml);
  };
}