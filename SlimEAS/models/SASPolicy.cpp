//
//  SASPolicy.cpp
//  SlimEAS
//
//  Created by envy.chen on 12/30/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#include "SASPolicy.h"

#include <libxml/xmlreader.h>

using namespace std;
using namespace SlimEAS;

SASPolicy::SASPolicy() {

}

bool SASPolicy::loadXml(const std::string &xml){
  xmlTextReaderPtr reader;
  uint8_t *buf = (uint8_t *)xml.c_str();
  int buf_len = (int)strlen((const char *)buf);
  
  reader = xmlReaderForMemory((const char *)buf, buf_len, NULL, "utf-8", 0);
  if (reader != NULL) {
    const xmlChar *curr_elm = NULL;
    while (xmlTextReaderRead(reader)) {
      int n_type = xmlTextReaderNodeType(reader);
      if (n_type == XNT_Element) {
        curr_elm = xmlTextReaderConstName(reader);
        printf("current element %s \n", curr_elm);
        continue;
      } else if (n_type == XNT_Text && curr_elm != NULL) {
        
#define ToU8(v) sscanf((const char *)value, "%c", &v)
#define ToBool(v) do { \
          uint8_t _  = 0; \
          ToU8(_); \
          v = _ == 0? false : true; \
        } while(0)
#define ToI32(v) sscanf((const char *)value, "%d", &v)
#define ToU32(v) sscanf((const char *)value, "%u", &v)
        
        const xmlChar *value = xmlTextReaderConstValue(reader);
        if (strcmp((const char *)curr_elm, "PolicyKey") == 0) {
          ToU32(policyKey);
        }else if (strcmp((const char *)curr_elm, "AllowBluetooth") == 0) {
          ToBool(allowBlueTooth);
        } else if(strcmp((const char *)curr_elm, "AllowBrowser") == 0 ) {
          ToBool(allowBrowser);
        } else if(strcmp((const char *)curr_elm, "AllowCamera") == 0 ){
          ToBool(allowCamera);
        } else if(strcmp((const char *)curr_elm, "AllowConsumerEmail") == 0) {
          ToBool(allowConsumerEmail);
        } else if(strcmp((const char *)curr_elm, "AllowDesktopSync") == 0) {
          ToBool(allowDesktopSync);
        } else if(strcmp((const char *)curr_elm, "AllowHTMLEmail") == 0) {
          ToBool(allowHTMLEmail);
        } else if(strcmp((const char *)curr_elm, "AllowInternetSharing") == 0) {
          ToBool(allowInternetSharing);
        } else if(strcmp((const char *)curr_elm, "AllowIrDA") == 0) {
          ToBool(allowIrDA);
        } else if(strcmp((const char *)curr_elm, "AllowPOPIMAPEmail") == 0) {
          ToBool(allowPOPIMAPEmail);
        } else if(strcmp((const char *)curr_elm, "AllowRemoteDesktop") == 0) {
          ToBool(allowRemoteDesktop);
        } else if(strcmp((const char *)curr_elm, "AllowSimpleDevicePassword") == 0) {
          ToBool(allowSimpleDevicePassword);
        } else if(strcmp((const char *)curr_elm, "AllowSMIMEEncryptionAlgorithmNegotiation") == 0) {
          ToI32(allowSMIMEEncryptionAlgorithmNegotiation);
        } else if(strcmp((const char *)curr_elm, "AllowSMIMESoftCerts") == 0) {
          ToBool(allowSMIMESoftCerts);
        } else if(strcmp((const char *)curr_elm, "AllowStorageCard") == 0) {
          ToBool(allowStorageCard);
        } else if(strcmp((const char *)curr_elm, "AllowTextMessaging") == 0) {
          ToBool(allowTextMessaging);
        } else if(strcmp((const char *)curr_elm, "AllowUnsignedApplications") == 0) {
          ToBool(allowUnsignedApplications);
        } else if(strcmp((const char *)curr_elm, "AllowUnsignedInstallationPackages") == 0) {
          ToBool(allowUnsignedInstallationPackages);
        } else if(strcmp((const char *)curr_elm, "AllowWiFi") == 0) {
          ToBool(allowWifi);
        } else if(strcmp((const char *)curr_elm, "AlphanumericDevicePasswordRequired") == 0) {
          ToBool(alphanumericDevicePasswordRequired);
        } else if(strcmp((const char *)curr_elm, "ApprovedApplicationList") == 0) {
          //TODO: no show
          //approvedApplicationList = ParseAppList(policySettingNode);
        } else if(strcmp((const char *)curr_elm, "AttachmentsEnabled") == 0) {
          ToBool(attachmentsEnabled);
        } else if(strcmp((const char *)curr_elm, "DevicePasswordEnabled") == 0) {
          ToBool(devicePasswordEnabled);
        } else if(strcmp((const char *)curr_elm, "DevicePasswordExpiration") == 0) {
          ToU32(devicePasswordExpiration);
        } else if(strcmp((const char *)curr_elm, "DevicePasswordHistory") == 0) {
          ToU32(devicePasswordHistory);
        } else if(strcmp((const char *)curr_elm, "MaxAttachmentSize") == 0) {
          ToU32(maxAttachmentSize);
        } else if(strcmp((const char *)curr_elm, "MaxCalendarAgeFilter") == 0) {
          ToU32(maxCalendarAgeFilter);
        } else if(strcmp((const char *)curr_elm, "MaxDevicePasswordFailedAttempts") == 0) {
          ToU32(maxDevicePasswordFailedAttempts);
        } else if (strcmp((const char *)curr_elm, "MaxEmailAgeFilter") == 0) {
          ToU32(maxEmailAgeFilter);
        } else if (strcmp((const char *)curr_elm, "MaxEmailBodyTruncationSize") == 0) {
          ToI32(maxEmailBodyTruncationSize);
        } else if (strcmp((const char *)curr_elm, "MaxEmailHTMLBodyTruncationSize") == 0) {
          ToI32(maxEmailHTMLBodyTruncationSize);
        } else if (strcmp((const char *)curr_elm, "MaxInactivityTimeDeviceLock") == 0) {
          ToU32(maxInactivityTimeDeviceLock);
        } else if (strcmp((const char *)curr_elm, "MinDevicePasswordComplexCharacters") == 0) {
          ToU8(minDevicePasswordComplexCharacters);
        } else if (strcmp((const char *)curr_elm, "MinDevicePasswordLength") == 0) {
          ToU8(minDevicePasswordLength);
        } else if (strcmp((const char *)curr_elm, "PasswordRecoveryEnabled") == 0) {
          ToBool(passwordRecoveryEnabled);
        } else if (strcmp((const char *)curr_elm, "RequireDeviceEncryption") == 0) {
          ToBool(requireDeviceEncryption);
        } else if (strcmp((const char *)curr_elm, "RequireEncryptedSMIMEMessages") == 0) {
          ToBool(requireEncryptedSMIMEMessages);
        } else if (strcmp((const char *)curr_elm, "RequireEncryptionSMIMEAlgorithm") == 0) {
          ToI32(requireEncryptionSMIMEAlgorithm);
        } else if (strcmp((const char *)curr_elm, "RequireManualSyncWhenRoaming") == 0) {
          ToBool(requireManualSyncWhenRoaming);
        } else if (strcmp((const char *)curr_elm, "RequireSignedSMIMEAlgorithm") == 0) {
          ToI32(requireSignedSMIMEAlgorithm);
        } else if (strcmp((const char *)curr_elm, "RequireSignedSMIMEMessages") == 0) {
          ToBool(requireSignedSMIMEMessages);
        } else if (strcmp((const char *)curr_elm, "RequireStorageCardEncryption") == 0) {
          ToBool(requireStorageCardEncryption);
        }  else if (strcmp((const char *)curr_elm, "RemoteWipe") == 0) {
          ToBool(remoteWipeRequested);
        }   else if (strcmp((const char *)curr_elm, "Status") == 0) {
          ToI32(status);
        } else if (strcmp((const char *)curr_elm, "UnapprovedInROMApplicationList") == 0) {
          //TODO: no show
          //unapprovedInROMApplicationList = ParseAppList(policySettingNode);
        } else {
          //node not in scheme
        }
      }else if  (n_type == XNT_EndElement) {
        curr_elm = NULL;
      } else {
        continue;
      }
    }
    xmlFreeTextReader(reader);
    return true;
  }
  return false;
}

SASPolicy::~SASPolicy() {
  
}


