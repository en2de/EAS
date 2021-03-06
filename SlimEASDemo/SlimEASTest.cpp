//
//  SlimEASTest
//  SlimEAS
//
//  Created by envy.chen on 12/14/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#include "SlimEASTest.h"

#include <iostream>
#include <fstream>
#include <utility>
#include <string>

#include "SASWBXml.h"
#include "SASOptionsRequest.h"
#include "SASCommandRequest.h"
#include "SASProvisionRequest.h"
#include "SASProvisionResponse.h"
#include "SASFolderSyncRequest.h"
#include "SASFolderSyncResponse.h"
#include "SASSyncRequest.h"
#include "SASSyncResponse.h"
#include "SASSendMailRequest.h"
#include "SASSendMailResponse.h"
#include "SASItemOperationsResponse.h"
#include "SASFetchAttachmentRequest.h"
#include "SASFetchEmailRequest.h"
#include "SASFetchMIMEEmailRequest.h"
#include "SASPingRequest.h"
#include "SASPingResponse.h"
#include "SASSmartReplyRequest.h"
#include "SASSmartForwardRequest.h"

#include "SASMail.h"
#include "SASFolder.h"
#include "SASCollection.h"

#include <unistd.h>

#include <thread>
#include <future>

#include <time.h>
#include <stdio.h>

#include <libxml/xmlwriter.h>

#include "SASDevice.h"

#include <mimetic/mimetic.h>
#include <mimetic/mimeentity.h>

#define MY_ENCODING "ISO-8859-1"

#define  BUFFER_SIZE 4096

using namespace SlimEAS;
using namespace mimetic;
using namespace std;

static const char *prefix = "settings";

static const char *server = "https://ex.qq.com";
static const char *user = "chenxu@nationsky.com";
static const char *password = "123456abcA";
static const bool useSSL = true;

#define InitialRequest(v) do { \
v.setServer("https://ex.qq.com"); \
v.setUser("136025803@qq.com"); \
v.setPassword("focus@917729"); \
v.setUseSSL(true); \
v.setDeviceId("6F24CAD599A5BF1A690246B8C68FAE8D"); \
v.setDeviceType("iPad"); \
v.setProtocolVersion("14.0"); \
v.setUseEncodeRequestLine(false); \
} while(0)



void mailTest() {
  
  SlimEAS::SASMail mail;
  mail.setTo("13491729@qq.com");
  mail.setFrom("chenxu@nationsky.com");
  mail.setSubject("New mail message");
  
  time_t now;
  mail.setDateReceived(now);
  
  mail.setDisplayTo("qintyo");
  mail.setThreadTopic("New mail message");
  mail.setImportance(1);
  mail.setRead(0);
  
  SlimEAS::SASBody body;
  body.setType(2);
  body.setEstimatedDataSize(116575);
  body.setTruncated(true);
  body.setMimeData("Hello I am the content.");
  
  mail.setBody(body);
  
  string xml = "<Properties xmlns:email=\"Email\" xmlns:email2=\"Email2\" xmlns:airsyncbase=\"AirSyncBase\"> \
  <email:To>\"deviceuser\" &lt;anat@contoso.com&gt;</email:To> \
  <email:Cc>\"deviceuser3\" &lt;chris@contoso.com.com&gt;</email:Cc> \
  <email:From>\"deviceuser2\" &lt;dag@contoso.com&gt;</email:From> \
  <email:Subject>Subject</email:Subject> \
  <email:DateReceived>2007-05-08T17:29:07.890Z </email:DateReceived> \
  <email:DisplayTo>DeviceUserDisplayName</email:DisplayTo> \
  <email:ThreadTopic>Subject</email:ThreadTopic> \
  <email:Importance>1</email:Importance> \
  <email:Read>0</email:Read> \
  <airsyncbase:Attachments> \
  <airsyncbase:Attachment> \
  <airsyncbase:DisplayName>ActiveSyncClient_ AcceptingMeetingRequest.JPG</airsyncbase:DisplayName> \
  <airsyncbase:FileReference>7%3a1%3a0</airsyncbase:FileReference> \
  <airsyncbase:Method>1</airsyncbase:Method> \
  <airsyncbase:EstimatedDataSize>18790 </airsyncbase:EstimatedDataSize> \
  </airsyncbase:Attachment> \
  </airsyncbase:Attachments> \
  <airsyncbase:Body> \
  <airsyncbase:Type>1</airsyncbase:Type> \
  <airsyncbase:EstimatedDataSize>20</airsyncbase:EstimatedDataSize> \
  <airsyncbase:Data>Body as plain text</airsyncbase:Data> \
  </airsyncbase:Body> \
  <email:MessageClass>IPM.Note</email:MessageClass> \
  <email:InternetCPID>28591</email:InternetCPID> \
  <email:Flag /> \
  <email:ContentClass>urn:content-classes:message</email:ContentClass> \
  <airsyncbase:NativeBodyType>1</airsyncbase:NativeBodyType> \
  <email2:ConversationId>1</email2:ConversationId> \
  <email2:ConversationIndex>12</email2:ConversationIndex> \
  </Properties>";
  
  cout << xml << endl;
  
  SASMail mail2;
  mail2.decode(xml);
  
  struct tm tm;
  strptime("2007-05-08T17:29:07.890Z", "%Y-%b-%dT%T", &tm);
  
  now = mktime(&tm);
  
  cout << "************* mail payload test start ************" << endl;
  // cout << mail.encode() << endl;
  cout << mail2.encode() << endl;
  cout << "************* mail payload test end ************" << endl;
  
  std::time_t now1= std::time(0);
  std::tm* now_tm= std::gmtime(&now1);
  char buf[42];
  std::strftime(buf, 42, "%Y-%m-%dT%TZ", now_tm);
  cout << buf;
  
}

void folderTest() {
  
  // (const std::string &folderName, const std::string &folderId, FolderType folderType, const SASFolder &parent);
  SASFolder parent("/Users/focuslan/Documents/workspace/Slim/Slim-EAS/testdata/");
  SASFolder subFolder("folderPath");
  SASFolder folder("./", "folderId", UserGeneric, parent);
  FolderSyncOptions options;
  options.filterType = OneDayBack;
  options.className = "className";
  options.maxItems = 1;
  options.mimeSupportLevel = SendMimeForAll;
  folder.setFolderSyncOptions(options);
  folder.addSubFolder(&subFolder);
  folder.generateOptionsXml();
  
  string folderXmls =
  "<Folder>\n \
  <Name>./testdata1/</Name>\n \
  <Id>folderId1</Id>\n \
  <Type>1</Type>\n \
  <SyncKey>0</SyncKey>\n \
  <LastSyncTime>0</LastSyncTime>\n \
  <FilterType>1</FilterType>\n \
  <ClassToSync>className1</ClassToSync>\n \
  <MaxItems>1</MaxItems>\n \
  <MimeSupport>2</MimeSupport>\n \
  <Folders/>\n \
  </Folder>\n";
  
  parent.addSubFolderFromXml(folderXmls);
  
  folder.saveFolderInfo();
}

void xmlTest() {
  
  xmlBufferPtr buf;
  buf = xmlBufferCreate();
  if (buf == NULL) {
    throw std::invalid_argument("Error creating the xml buffer");
  }
  
  xmlTextWriterPtr writer;
  writer = xmlNewTextWriterMemory(buf, 0);
  xmlTextWriterSetIndent(writer, 1);
  if (writer == NULL) {
    throw std::invalid_argument("Error creating the xml writer");
  }
  
  SlimEAS::SASDevice device;
  device.setModel("testModel");
  device.setIMEI("testIMEI");
  device.setFriendlyName("testDevice");
  device.setOS("iphone os 8.0");
  device.setOS_Lang("english");
  device.setPhoneNumber("18603008614");
  device.setMobileOperator("noOperator");
  device.setUserAgent("SlimEAS");
  
  string deviceXml = device.payload();
  //  xml.append("<!DOCTYPE ActiveSync PUBLIC \"-//MICROSOFT//DTD ActiveSync//EN\" \"http://www.microsoft.com/\">\n");
  
  
  xmlTextWriterStartDocument(writer, "1.0", "utf-8", NULL);
  xmlTextWriterWriteDTD(writer, BAD_CAST "ActiveSync", BAD_CAST "-/MICROSOFT/DTD ActiveSync/EN", BAD_CAST "http://www.microsoft.com/", NULL);
  xmlTextWriterStartElement(writer, BAD_CAST "Provision");
  xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns", BAD_CAST "Provision:");
  xmlTextWriterWriteAttributeNS(writer, BAD_CAST "xmlns", BAD_CAST "settings", NULL, BAD_CAST "Settings:");
  //insert device xml
  xmlTextWriterWriteRaw(writer, BAD_CAST deviceXml.c_str());
  
  xmlTextWriterStartElement(writer, BAD_CAST "Policies");
  xmlTextWriterStartElement(writer, BAD_CAST "Policy");
  xmlTextWriterWriteElement(writer, BAD_CAST "PolicyType", BAD_CAST "MS-EAS-Provisioning-WBXML");
  xmlTextWriterEndElement(writer);
  xmlTextWriterEndElement(writer);
  xmlTextWriterEndElement(writer);
  xmlTextWriterEndDocument(writer);
  
  //  xmlTextWriterStartDocument(writer, "1.0", "utf-8", NULL);
  //  xmlTextWriterStartElementNS(writer, BAD_CAST "settings", BAD_CAST "DeviceInformation", NULL);
  //  xmlTextWriterStartElementNS(writer, BAD_CAST prefix, BAD_CAST "Set", NULL);
  //  xmlTextWriterWriteElementNS(writer, BAD_CAST prefix, BAD_CAST "Model", NULL, BAD_CAST "TestMOdel");
  //  xmlTextWriterWriteElementNS(writer, BAD_CAST prefix, BAD_CAST "IMEI", NULL, BAD_CAST "TestMOdel");
  //  xmlTextWriterWriteElementNS(writer, BAD_CAST prefix, BAD_CAST "FriendlyName", NULL, BAD_CAST "TestMOdel");
  //  xmlTextWriterWriteElementNS(writer, BAD_CAST prefix, BAD_CAST "OS", NULL, BAD_CAST "TestMOdel");
  //  xmlTextWriterWriteElementNS(writer, BAD_CAST prefix, BAD_CAST "OSLanguage", NULL, BAD_CAST "TestMOdel");
  //  xmlTextWriterWriteElementNS(writer, BAD_CAST prefix, BAD_CAST "PhoneNumber", NULL, BAD_CAST "TestMOdel");
  //  xmlTextWriterWriteElementNS(writer, BAD_CAST prefix, BAD_CAST "MobileOperator", NULL, BAD_CAST "TestMOdel");
  //  xmlTextWriterWriteElementNS(writer, BAD_CAST prefix, BAD_CAST "UserAgent", NULL, BAD_CAST "TestMOdel");
  //  xmlTextWriterEndElement(writer);
  //  xmlTextWriterEndElement(writer);
  //  xmlTextWriterEndDocument(writer);
  
  string xml((char *)buf->content);
  
  std::cout << xml;
}

void cppTest() {
  std::string str;
  
  str = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" \
  "<!DOCTYPE ActiveSync PUBLIC \"-/MICROSOFT//DTD ActiveSync//EN\" \"http://www.microsoft.com/\">\n" \
  "<Provision xmlns=\"Provision:\">\n" \
  "  <Policies>\n" \
  "    <Policy>\n" \
  "      <PolicyType>MS-EAS-Provisioning-WBXML</PolicyType>\n" \
  "    </Policy>\n" \
  "  </Policies>\n"\
  "</Provision>\n";
  
  std::cout << "Test file content\n";
  std::cout << str << "\n";
  std::cout << "Test file Lenght:" << str.size() << "\n";
  
  std::cout << "Start xml - wbxml\n";
  SlimEAS::SASWBXml wb;
  
  uint32_t outlen = 0;
  uint8_t *output = wb.toWBXml(str, &outlen);
  std::cout << "load to wbxml, output length:" << outlen << "\n";
  
  std::cout << "Start xml - wbxml\n";
  std::string result = wb.toXml(output, outlen);
  std::cout << "load to xml , result: \n";
  std::cout << result << "\n";
  std::cout << "result file lenght:" << result.size() << "\n";
  
  free(output);
}

void devicePayloadTest() {
  SlimEAS::SASDevice device;
  device.setModel("testModel");
  device.setIMEI("testIMEI");
  device.setFriendlyName("testDevice");
  device.setOS("iphone os 8.0");
  device.setOS_Lang("english");
  device.setPhoneNumber("18603008614");
  device.setMobileOperator("noOperator");
  device.setUserAgent("SlimEAS");
  
  string deviceXml = device.payload();
  
  std::cout << deviceXml;
}

void endianTest() {
  uint32_t i=0x12345678;
  cout<<hex<<i<<endl;
  uint8_t *p = (uint8_t *)&i;
  if((*p==0x78)&(*(p+1)==0x56))
    cout<<"little-endian"<<endl;
  else if((*p==0x12)&(*(p+1)==0x34))
    cout<<"big-endian"<<endl;
  else
    cout<<"unknow endianess!";
}

void optionRequestTest() {
  SlimEAS::SASOptionsRequest optReq;
  optReq.setServer("https://ex.qq.com");
  optReq.setUseSSL(true);
  optReq.setUser("chenxu@nationsky.com");
  optReq.setPassword("123456abcA");
  
  SlimEAS::SASOptionsResponse resS = optReq.getReponse();
  
  std::cout << "Supported Versions: " << resS.supportedVersions << "\n";
  std::cout << "Supported Commands: " << resS.supportedCommand  << "\n";
  
  /*
   std::future<SlimEAS::SASOptionsResponse> res = std::async([&req]{
   return req.getReponse();
   });
   
   std::cout << "start sleep";
   sleep(60);
   std::cout << "sleep over";
   */
}

void commandRequestTest() {
  SlimEAS::SASCommandRequest req("https://ex.qq.com", "chenxu@nationsky.com", "123456abcA");
  req.setCommand("Provision");
  req.setDeviceId("6F24CAD599A5BF1A690246B8C68FAE8D");
  req.setDeviceType("SmartPhone");
  req.setProtocolVersion("14.0");
  req.setUseEncodeRequestLine(false);
  
  std::string xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  xml.append("<!DOCTYPE ActiveSync PUBLIC \"-//MICROSOFT//DTD ActiveSync//EN\" \"http://www.microsoft.com/\">\n");
  xml.append("<Provision xmlns=\"Provision:\"=\"Settings:\">\n");
  xml.append("    <settings:DeviceInformation>\n");
  xml.append("        <settings:Set>\n");
  xml.append("            <settings:Model>Test 1.0</settings:Model>\n");
  xml.append("            <settings:IMEI>012345678901234</settings:IMEI>\n");
  xml.append("            <settings:FriendlyName>My Test App</settings:FriendlyName>\n");
  xml.append("            <settings:OS>iOS 8.1</settings:OS>\n");
  xml.append("            <settings:OSLanguage>English</settings:OSLanguage>\n");
  xml.append("            <settings:PhoneNumber>555-123-4567</settings:PhoneNumber>\n");
  xml.append("            <settings:MobileOperator>Nationsky</settings:MobileOperator>\n");
  xml.append("            <settings:UserAgent>Slim-EAS</settings:UserAgent>\n");
  xml.append("        </settings:Set>\n");
  xml.append("    </settings:DeviceInformation>\n");
  xml.append("     <Policies>\n");
  xml.append("          <Policy>\n");
  xml.append("               <PolicyType>MS-EAS-Provisioning-WBXML</PolicyType> \n");
  xml.append("          </Policy>\n");
  xml.append("     </Policies>\n");
  xml.append("</Provision>");
  req.setXMLPayload(xml);
  
  SlimEAS::SASHTTPResponse *res = req.getResponse();
}

void provisionTest() {
  
  uint32_t policyKey = 0;
  
  //provisioning test
  SlimEAS::SASDevice provDevice;
  provDevice.setModel("testModel");
  provDevice.setIMEI("testIMEI");
  provDevice.setFriendlyName("testDevice");
  provDevice.setOS("iphone os 8.0");
  provDevice.setOS_Lang("english");
  provDevice.setPhoneNumber("18603008614");
  provDevice.setMobileOperator("noOperator");
  provDevice.setUserAgent("SlimEAS");
  
  /* Step 1: client requests security policy from the server */
  SlimEAS::SASProvisionRequest provRequest;
  InitialRequest(provRequest);
  provRequest.setProvisionDevice(provDevice);
  provRequest.setPolicyKey(0);
  
  /* Step 2: Server responds to client request with security policy and temorary policyKey */
  SlimEAS::SASProvisionResponse *provRes = dynamic_cast<SlimEAS::SASProvisionResponse *>(provRequest.getResponse());
  printf("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  printf("Step 1 headers: \n\n%s\n", provRes->headerString().c_str());
  printf("----------------------------------------------------\n");
  printf("Step 1 payload: \n\n%s\n", provRequest.XMLPayload().c_str());
  printf("----------------------------------------------------\n");
  printf("Step 1 response: \n\n%s\n", provRes->xmlResponse().c_str());
  printf("++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
  
  /* Step 3: Client acknowledges receipt and application of security policy */
  SlimEAS::SASPolicy policy = provRes->policy();
  
  printf("\nResponse Status: %d\n", provRes->status());
  if (provRes->status() == SlimEAS::SASProvisionResponse::Provision_Success) {
    if (provRes->isPolicyLoad()) {
      if (provRes->policy().status == SlimEAS::SASProvisionResponse::Provision_Success) {
        if (provRes->policy().remoteWipeRequested) {
          // The server requested a remote wipe.
          // The client must acknowledge it.
          printf("\n+++ The server has requested a remote wipe. +++\n");
          SASProvisionRequest remoteWipeAcknowledgment;
          InitialRequest(remoteWipeAcknowledgment);
          remoteWipeAcknowledgment.setRemoteWipe(true);
          remoteWipeAcknowledgment.setStatus(SlimEAS::SASProvisionResponse::Provision_Success);
          
          printf("\nSending remote wipe acknowledgment...\n");
          // send the acknowledgment
          SlimEAS::SASProvisionResponse *remoteWipeAckResponse =
          dynamic_cast<SlimEAS::SASProvisionResponse *>(remoteWipeAcknowledgment.getResponse());
          
          printf("\nRemote Wipe Acknowledgment Request: \n%s\n", remoteWipeAcknowledgment.XMLPayload().c_str());
          printf("\nRemote wipe acknowledgment response status: \n%d\n", remoteWipeAckResponse->status());
        } else {
          // The server has provided a policy
          // and a temprary policy key.
          // The client must acknowledge this policy
          // in order to get a permanet policy
          printf("\nPolicy retrieved from the server.\n");
          printf("\nTemporary policy key: %d\n", policy.policyKey);
          
          SlimEAS::SASProvisionRequest policyAcknowledgment;
          InitialRequest(policyAcknowledgment);
          policyAcknowledgment.setPolicyKey(policy.policyKey);
          policyAcknowledgment.setAcknowledgement(true);
          policyAcknowledgment.setStatus(SlimEAS::SASProvisionResponse::Provision_Success);
          
          printf("\nSending policy acknowledgment...\n");
          
          /* Step 4: Server responds with final policyKey */
          SlimEAS::SASProvisionResponse *policyAckResponse = dynamic_cast<SlimEAS::SASProvisionResponse *>(policyAcknowledgment.getResponse());
          
          printf("\nPolicy Acknowledgment Request:\n%s\n", policyAcknowledgment.XMLPayload().c_str());
          printf("\nResponse Status: %d\n", policyAckResponse->status());
          printf("\nResponse:\n%s\n", policyAckResponse->xmlResponse().c_str());
          
          if (policyAckResponse->status() == SlimEAS::SASProvisionResponse::Provision_Success &&
              policyAckResponse->isPolicyLoad()) {
            printf("\nPolicy acknowledgment successful.\n");
            printf("\nPermanent Policy Key: %d\n", policyAckResponse->policy().policyKey);
            policyKey = policyAckResponse->policy().policyKey;
          } else {
            printf("\Error returned from policy acknowledgment request: %d.\n", policyAckResponse->status());
          }
        }
      }
    } else {
      printf("\nPolicy Error returned from initial provision request: %d.\n", policy.status);
    }
  } else {
    printf("\Error returned from initial provision request: %d.\n", provRes->status());
  }
}

void IterateRootFolder(SASFolder *rootFolder)
{
  if (rootFolder == nullptr) {
    std::cout << "NULL root folder!" << std::endl;
    return;
  }
  //std::cout << "++++Begin iterate++++" << std::endl;
  
  std::cout << "name:" << rootFolder->name() << ", id:" << rootFolder->id() << ", type:" << (int)(rootFolder->type()) << std::endl;
  for (auto &it: rootFolder->subFolders()) {
    std::cout << "--subfolder:" << std::endl;
    std::cout << "name:" << const_cast<SASFolder &>(it).name() << ", id:" << const_cast<SASFolder &>(it).id() << ", type:" << (int)const_cast<SASFolder &>(it).type() << std::endl;
    
    if (const_cast<SASFolder &>(it).subFolders().size() > 0 ) {
      
      
      IterateRootFolder(&const_cast<SASFolder &>(it));
    }
  }
  //std::cout << "----End iterate----" << std::endl;
}

void FolderSyncTest() {
  
  SlimEAS::SASFolderSyncRequest *request = new SlimEAS::SASFolderSyncRequest(server, user, password, useSSL);
  request->setDeviceId("6F24CAD599A5BF1A690246B8C68FAE8D");
  request->setDeviceType("SmartPhone");
  request->setProtocolVersion("14.0");
  
  SlimEAS::SASFolderSyncResponse *response = dynamic_cast<SlimEAS::SASFolderSyncResponse *>(request->getResponse());
  if (response->status() == SlimEAS::SASFolderSyncResponse::FolderSync_Success) {
    static const std::string mailboxCacheLocation = "/Users/gy/Workspace/Slim-EAS/tmp/";
    SASFolder *rootFolder = new SASFolder(mailboxCacheLocation);
    response->updateRootFolder(rootFolder);
    IterateRootFolder(rootFolder);
    
    const SASFolder * inboxFolder = rootFolder->findFolderById("");
    
    
    delete request;
    request = nullptr;
    delete response;
    response = nullptr;
    
    SlimEAS::SASSyncRequest *syncRequest = new SlimEAS::SASSyncRequest(server, user, password, useSSL);
    syncRequest->setDeviceId("6F24CAD599A5BF1A690246B8C68FAE8D");
    syncRequest->setDeviceType("SmartPhone");
    syncRequest->setProtocolVersion("14.0");
    std::list<SASFolder *> & fl = syncRequest->folderList();
    fl.push_back(const_cast<SASFolder *>(inboxFolder));
    
    SlimEAS::SASHTTPResponse *syncResponse = syncRequest->getResponse();
    
    
    
    delete syncRequest;
    syncRequest = nullptr;
    delete syncResponse;
    syncResponse = nullptr;
  }
  
}

void syncTest() {
  
  SASSyncRequest request;
  InitialRequest(request);
  
  request.setPolicyKey(1307199584);
  request.setAction(Synchronize);
  
  list<SASFolder *> folderList = request.folderList();
  
  // do not delete directly, request will take care of it.
  SASFolder *inbox = new SASFolder("/Users/focuslan/Documents/workspace/Slim/Slim-EAS/testdata/");
  
  // initial the syncKey from FoderSync command.
  inbox->setSyncKey("1287061996");
  inbox->setId("1");
  
  // FilterType.
  FolderSyncOptions options;
  options.filterType = ThreeDaysBack;
  inbox->setFolderSyncOptions(options);
  
  // the WindowSize param decides how many emails you want to sync one time.
  inbox->setWindowSize(10);
  
  request.folderList().push_back(inbox);
  
  SASSyncResponse *response = dynamic_cast<SASSyncResponse *>(request.getResponse());
  
  // 1. get the collections from the response.
  vector<SASCollection> collectionList = response->collectionList();
  for (auto &collection : collectionList) {
    // 2. if moreAvailable() if true, then get the responsed syncKey to sync another window
    if (collection.moreAvailable()) {
      // 3. get the syncKey
      string syncKey = collection.syncKey();
      // 4. request for next window of email
      
    }
  }
  
  printf("response: \nHeaders:\n %s\nContents:\n%s\n", response->headerString().c_str(), response->response().c_str());
  
  if (response) {
    delete response;
  }
  response = nullptr;
}

void syncAddContactTest() {
  
  SASSyncRequest request;
  InitialRequest(request);
  
  request.setPolicyKey(1307199584);
  request.setAction(AddContact);
  
  list<SASFolder *> folderList = request.folderList();
  
  SASFolder *inbox = new SASFolder("/Users/focuslan/Documents/workspace/Slim/Slim-EAS/testdata/");
  
  // initial the syncKey from FoderSync command.
  inbox->setSyncKey("6");
  inbox->setId("1");
  
  // the WindowSize param decides how many emails you want to sync one time.
  inbox->setWindowSize(5);
  
  request.folderList().push_back(inbox);
  
  SASSyncResponse *response = dynamic_cast<SASSyncResponse *>(request.getResponse());
  
  printf("response: \nHeaders:\n %s\nContents:\n%s\n", response->headerString().c_str(), response->response().c_str());
  
  if (response) {
    delete response;
  }
  response = nullptr;
}

void fetchAttachmentTest() {
  
  SlimEAS::SASFetchAttachmentRequest request;
  InitialRequest(request);
  
  FolderSyncOptions options;
  
  BodyPreferences pre;
  pre.Type = PlainText;
  pre.TruncationSize = 5120;
  pre.AllOrNone = false;
  
  options.setBodyPreferences(pre);
  
  request.setPolicyKey(1307199584);
  request.setStore("Inbox");
  
  /* The FileReference param comes from Sync Command */
  request.setFileReference("ZL2721N8C~0vJPRnuxlOLGY2uxvQ0b%3A%243d669607e3b4355e9a1ab0edae871c09"); // IMG_3693副本.jpg
  
  SASItemOperationsResponse *response = dynamic_cast<SASItemOperationsResponse *>(request.getResponse());
  
  SASMail mail = response->mail();
  
  SASBody body = mail.body();
  
  vector<SASAttachment*> attachments = mail.attachments();
  
  // Base64 Decoder
  mimetic::Base64::Decoder b64;
  
  for (auto &it : attachments) {
    
    string fileName = "/Users/focuslan/Documents/workspace/Slim/Slim-EAS/testdata/" + it->fileReference();
    
    filebuf fout;
    fout.open(fileName + ".jpg", ios::out);
    
    std::istringstream is(it->data());
    
    ostream os(&fout);
    istreambuf_iterator<char> ibeg(is), iend;
    ostreambuf_iterator<char> out(os);
    
    decode(ibeg, iend, b64, out);
  }
  
  printf("Response:\n%s\n", response->xmlResponse().c_str());
}

void fetchEmailTest() {
  
  SlimEAS::SASFetchEmailRequest request;
  InitialRequest(request);
  
  FolderSyncOptions options;
  
  BodyPreferences pre;
  pre.Type = PlainText;
  pre.TruncationSize = 5120;
  pre.AllOrNone = false;
  
  options.setBodyPreferences(pre);
  
  request.setPolicyKey(1307199584);
  request.setOptions(options);
  request.setStore("Inbox");
  request.setCollectionId("1");
  request.setServerId("ZL2721N8C~0vJPRnuxlOLGY2uxvQ0b"); // the serverId comes from response of Sync command
  
  SASItemOperationsResponse *response = dynamic_cast<SASItemOperationsResponse *>(request.getResponse());
  
  SASMail mail = response->mail();
  
  SASBody body = mail.body();
  
  vector<SASAttachment*> attachments = mail.attachments();
  
  // Base64 Decoder
  mimetic::Base64::Decoder b64;
  
  for (auto &it : attachments) {
    
    string fileName = "/Users/focuslan/Documents/workspace/Slim/Slim-EAS/testdata/" + it->fileReference();
    
    filebuf fout;
    fout.open(fileName + ".jpg", ios::out);
    
    std::istringstream is(it->data());
    
    ostream os(&fout);
    istreambuf_iterator<char> ibeg(is), iend;
    ostreambuf_iterator<char> out(os);
    
    decode(ibeg, iend, b64, out);
  }
  
  printf("Response:\n%s\n", response->xmlResponse().c_str());
}

void fetchMimeEmailTest() {
  
  SlimEAS::SASFetchMIMEEmailRequest request;
  InitialRequest(request);
  
  FolderSyncOptions options;
  
  BodyPreferences pre;
  pre.Type = PlainText;
  pre.TruncationSize = 5120;
  pre.AllOrNone = false;
  
  options.setBodyPreferences(pre);
  
  request.setPolicyKey(1307199584);
  request.setOptions(options);
  request.setStore("Inbox");
  request.setCollectionId("1");
  request.setServerId("ZL2721N8C~0vJPRnuxlOLGY2uxvQ0b"); // the serverId comes from response of Sync command
  
  SASItemOperationsResponse *response = dynamic_cast<SASItemOperationsResponse *>(request.getResponse());
  
  SASMail mail = response->mail();
  
  SASBody body = mail.body();
  
  vector<SASAttachment*> attachments = mail.attachments();
  
  // Base64 Decoder
  mimetic::Base64::Decoder b64;
  
  for (auto &it : attachments) {
    
    string fileName = "/Users/focuslan/Documents/workspace/Slim/Slim-EAS/testdata/" + it->fileReference();
    
    filebuf fout;
    fout.open(fileName + ".jpg", ios::out);
    
    std::istringstream is(it->data());
    
    ostream os(&fout);
    istreambuf_iterator<char> ibeg(is), iend;
    ostreambuf_iterator<char> out(os);
    
    decode(ibeg, iend, b64, out);
  }
  
  printf("Response:\n%s\n", response->xmlResponse().c_str());
}

void sendMailTest(const string &fileFullPath) {
  SASSendMailRequest request;
  InitialRequest(request);
  
  request.setPolicyKey(1307199584);
  
  /********************* Entity Start **************************/
  
  string fqn = fileFullPath;
  string fileName = mimetic::utils::extractFilename(fqn);
  
  // 如果想写一封带附件的邮件体，则需要一个 MultipartMixed 对象，该对象会添加一个 BoundString
  mimetic::MimeEntity *mimeEntity = new mimetic::MultipartMixed;
  
  mimeEntity->header().from("CEO <136025803@qq.com>");
  mimeEntity->header().to("Focus <13491729@qq.com>");
  mimeEntity->header().subject("my first mimetic msg with attachment");
  
  MimeVersion version("1.0");
  mimeEntity->header().mimeVersion(version);
  
  mimetic::TextEntity *html = new mimetic::TextEntity("<html><body><b>FOO BAR</b></body></html>", "utf-8");
  ContentType contentType("text/html");
  html->header().contentType(contentType);
  
  // 添加一个 Attachment，指定为 Base64 编码
  mimetic::Attachment *attachment = new mimetic::Attachment(fileName, Base64::Encoder());
  
  // 设置完头以后，加载具体内容，编码成 Base64
  FILE                      *pfile = fopen(fqn.c_str(), "rb");
  char                      buffer[BUFFER_SIZE];
  mimetic::Base64::Encoder  b64;
  
  // 考虑到大文件的问题，需要分片段读取文件
  while (!feof(pfile))
  {
    //读文件
    uint64_t readbytes = fread(buffer, 1, BUFFER_SIZE - 1, pfile);
    if (ferror(pfile) || readbytes == 0)
    {
      break;
    }
    
    std::stringstream temp;
    std::ostreambuf_iterator<char> out(temp);
    
    // 转为BASE64编码，目标存放至std::stringstream中
    mimetic::encode(buffer,  buffer + readbytes, b64, out);
    std::string str = temp.str();
    
    // 将转换后的内容放入块中
    attachment->load(str.begin(), str.end(), mimetic::imNone);
  }
  
  // 压栈。两部分：正文 + 附件
  mimeEntity->body().parts().push_back(html);
  mimeEntity->body().parts().push_back(attachment);
  
  /********************* Entity End **************************/
  
  request.mail().setDisplayFrom("CEO");
  request.mail().setFrom("136025803@qq.com");
  request.mail().setTo("13491729@qq.com");
  request.mail().setSubject("Simple Subject From Slim using SendMail Command");
  request.mailInfo().clientId = "633916348086136194";
  request.mail().assignEntity(mimeEntity);
  
  SASSendMailResponse *provRes = dynamic_cast<SASSendMailResponse *>(request.getResponse());
  
  mimeEntity->body().clear();
  
  delete html;
  delete attachment;
  delete provRes;
  
  pfile = nullptr;
  html = nullptr;
  attachment = nullptr;
  provRes = nullptr;
}

void smartReplyTest() {
  SASSmartReplyRequest request;
  InitialRequest(request);
  
  request.setPolicyKey(1307199584);
  
  request.mail().setDisplayFrom("CEO");
  request.mail().setFrom("136025803@qq.com");
  request.mail().setTo("13491729@qq.com");
  request.mail().setSubject("Simple Subject From Slim using SendMail Command");
  request.mailInfo().clientId = "E61068AD-10AE-4819-993B-C50B2FB0714E";
  request.mailInfo().source.folderId = "1";
  request.mailInfo().source.itemId = "1:5";
  
  MimeEntity *plainText = new MimeEntity;
  
  plainText->header();
  plainText->header().from("CEO <136025803@qq.com>");
  plainText->header().to("you <13491729@qq.com>");
  plainText->header().subject("Simple Subject From Slim using SmartReply Command");
  plainText->body().assign("hello there!");
  
  
  request.mail().assignEntity(plainText);
  
  request.getResponse();
  
  delete plainText;
  plainText = nullptr;
}

void smartForwardTest() {
  SASSmartForwardRequest request;
  InitialRequest(request);
  
  request.setPolicyKey(1307199584);
  
  request.mail().setDisplayFrom("CEO");
  request.mail().setFrom("136025803@qq.com");
  request.mail().setTo("13491729@qq.com");
  request.mail().setSubject("Simple Subject From Slim using SmartForward Command");
  request.mailInfo().clientId = "d7b99822-685a-4a40-8dfb-87a114926986";
  request.mailInfo().source.longId = "E61068AD-10AE-4819-993B-C50B2FB0714E";
  
  MimeEntity *plainText = new MimeEntity;
  
  plainText->header();
  plainText->header().from("CEO <136025803@qq.com>");
  plainText->header().to("you <13491729@qq.com>");
  plainText->header().subject("Simple Subject From Slim using SmartForward Command");
  plainText->body().assign("hello there!");
  
  
  request.mail().assignEntity(plainText);
  
  request.getResponse();
}

void pingTest() {
  
  SASPingRequest request;
  InitialRequest(request);
  
  request.setPolicyKey(1307199584);
  request.addFolder("1", SlimEAS::CLASS_EMAIL);
  request.setHeartbeatInterval(80);
  
  SASPingResponse *response = dynamic_cast<SASPingResponse *>(request.getResponse());
  
  if(response->isChangedFound()) {
    for (auto &it : response->changedFolderIds()) {
      printf("\nFolder '%s' needs to sync up!!!\n", it.c_str());
      // Now we can use the sync command to sync emails...
    }
  }
  
  delete response;
  response = nullptr;
}

void mimeTest() {
  
  MimeEntity me;
  
  me.header();
  me.header().from("CEO <>");
  me.header().to("you <you@domain.com>");
  me.header().subject("my first mimetic msg");
  me.body().assign("hello there!");
  
  cout << me << endl;
}

void mimeAttachmentTest() {
  
  string fqn = "/Users/focuslan/Documents/workspace/Slim/Slim-EAS/testdata/test.jpg";
  int32_t ignoreMask = 0;
  
  File in(fqn);
  if(!in)
  {
    cerr << "ERR: unable to open file " << fqn << endl;
    return;
  }
  
  mimetic::MultipartEntity mulEntity;
  
  mimetic::ApplicationOctStream os;
  os.load(in.begin(), in.end(), ignoreMask);
  cout << os << endl;
  
  
  ImageJpeg me(fqn);
  me.load(in.begin(), in.end(), ignoreMask);
  
  cout << me << endl;
  
}

void goTest() {
  
  smartReplyTest();
  smartForwardTest();
  sendMailTest("/Users/focuslan/Documents/workspace/Slim/Slim-EAS/testdata/test.jpg");
  
  fetchAttachmentTest();
  fetchEmailTest();
  fetchMimeEmailTest();

}
