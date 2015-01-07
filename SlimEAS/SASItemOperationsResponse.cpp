//
//  SASItemOperationsResponse.cpp
//  SlimEAS
//
//  Created by Focus Lan on 1/6/15.
//  Copyright (c) 2015 Slim. All rights reserved.
//

#include "SASItemOperationsResponse.h"
#include "SASMail.h"

#include <libxml/xmlreader.h>

using namespace SlimEAS;
using namespace std;

//#define EMAILMIMEITEM

SASItemOperationsResponse::SASItemOperationsResponse(SASHTTPRequest::SASHTTPResponseContext &ctx)
: SASCommandResponse(ctx)
{
  xmlTextReaderPtr reader;
  
  /* Mock Data */
#ifdef EMAILITEM
  _xmlResponse =
  "<?xml version=\"1.0\" encoding=\"utf-8\"?> \
  <ItemOperations xmlns:airsync=\"AirSync\" xmlns:airsyncbase=\"AirSyncBase\" xmlns:email=\"Email\" xmlns:email2=\"Email2\" xmlns=\"ItemOperations\"> \
  <Status>1</Status> \
  <Response> \
  <Fetch> \
  <Status>1</Status> \
  <airsync:CollectionId>7</airsync:CollectionId> \
  <airsync:ServerId>7:1</airsync:ServerId> \
  <airsync:Class>Email</airsync:Class> \
  <Properties> \
  <email:To>\"deviceuser\" &lt;anat@contoso.com&gt;</email:To> \
  <email:Cc>\"deviceuser3\" &lt;chris@contoso.com.com&gt;</email:Cc> \
  <email:From>\"deviceuser2\" &lt;dag@contoso.com&gt; \
  </email:From>\
  <email:Subject>Subject</email:Subject> \
  <email:DateReceived>2007-05-08T17:29:07.890Z </email:DateReceived> \
  <email:DisplayTo>DeviceUserDisplayName</email:DisplayTo> \
  <email:ThreadTopic>Subject</email:ThreadTopic> \
  <email:Importance>1</email:Importance> \
  <email:Read>0</email:Read> \
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
  <email2:ConversationId>2</email2:ConversationId> \
  <email2:ConversationIndex>32</email2:ConversationIndex> \
  </Properties> \
  </Fetch> \
  </Response> \
  </ItemOperations>";
#endif 
#ifdef EMAILMIMEITEM
  _xmlResponse =
  "<?xml version=\"1.0\" encoding=\"utf-8\"?>\
  <ItemOperations xmlns=\"ItemOperations\" xmlns:airsync=\"AirSync\" xmlns:email=\"Email\" xmlns:email2=\"Email2\" xmlns:airsyncbase=\"AirSyncBase\">\
  <Status>1</Status>\
  <Response>\
  <Fetch>\
  <Status>1</Status>\
  <airsync:CollectionId>17</airsync:CollectionId>\
  <airsync:ServerId>17:11</airsync:ServerId>\
  <airsync:Class>Email</airsync:Class>\
  <Properties>\
  <email:To>\"Anat Kerry\" &lt;anat@contoso.com&gt;</email:To>\
  <email:From>\"Chris Gray\" &lt;chris@contoso.com&gt;</email:From>\
  <email:Subject>opaque s + e</email:Subject>\
  <email:DateReceived>2007-02-01T06:42:46.015Z</email:DateReceived>\
  <email:DisplayTo>Anat Kerry</email:DisplayTo>\
  <email:ThreadTopic>opaque s + e</email:ThreadTopic>\
  <email:Importance>1</email:Importance>\
  <email:Read>1</email:Read> \
  <airsyncbase:Attachments>\
  <airsyncbase:Attachment>\
  <airsyncbase:DisplayName>smime.p7m</airsyncbase:DisplayName>\
  <airsyncbase:FileReference>RgAAAAA4u8%2fWvU8lQ7GtLlC7V9V3BwCdyWYIRkOHRp2ozB%2f0DXQsAHgM%2bwAF AAA6pk60fqkEQbWH4Wm%2bnjh7AHgNBA%2bgAAAJ%3a0</airsyncbase:FileReference>\
  <airsyncbase:Method>1</airsyncbase:Method>\
  <airsyncbase:EstimatedDataSize>9340</airsyncbase:EstimatedDataSize>\
  </airsyncbase:Attachment>\
  </airsyncbase:Attachments>\
  <airsyncbase:Body>\
  <airsyncbase:Type>4</airsyncbase:Type>\
  <airsyncbase:EstimatedDataSize>13813</airsyncbase:EstimatedDataSize>\
  <airsyncbase:Data>Received: from contoso.com ([157.55.97.121])\
  by contoso.com ([157.55.97.121]) with mapi;\
  Wed, 31 Jan 2007 22:42:45 -0800\
From: Chris Gray &lt;chris@contoso.com&gt; To: Anat Kerry &lt;anat@contoso.com&gt; Content-Class: urn:content-classes:message Date: Wed, 31 Jan 2007 22:42:41 -0800 Subject: opaque s + e\
  Thread-Topic: opaque s + e\
  Thread-Index: AcdFzCv5tyCXieBuTd2I5APpEvS+iQ== Message-ID:\
  &lt;3AA64EB47EA90441B587E169BE9E387B780D00C326@contoso.com&gt; Content-Language: en-US\
  X-MS-Exchange-Organization-AuthAs: Internal X-MS-Exchange-Organization-AuthMechanism: 04 X-MS-Exchange-Organization-AuthSource:\
  contoso.com\
  X-MS-Has-Attach: yes\
  X-MS-Exchange-Organization-SCL: -1\
  X-MS-TNEF-Correlator:\
acceptlanguage: en-US\
  Content-Type: application/x-pkcs7-mime; smime-type=enveloped-data; name=\"smime.p7m\"\
  Content-Disposition: attachment; filename=\"smime.p7m\" Content-Transfer-Encoding: base64\
  MIME-Version: 1.0\
  MIAGCSqGSIb3DQEHA6CAMIACAQAxggJEMIIBHgIBADCBhjB4MRMwEQYKCZImiZPyLGQBGRYDY29t MRkwFwYKCZImiZPyLGQBGRYJbWljcm9zb2Z0MRYwFAYKCZImiZPyLGQBGRYGZXh0ZXN0MR0wGwYK CZImiZPyLGQBGRYNamluZ2h1YWMwMURPTTEPMA0GA1UEAxMGVGVzdENBAgonJIo2AAAAAAAHMA0G (Large section of sample data removed)\
  </airsyncbase:Data>\
  </airsyncbase:Body>\
  <email:MessageClass>IPM.Note.SMIME</email:MessageClass>\
  <email:InternetCPID>20127</email:InternetCPID>\
  <email:Flag/>\
  <email:ContentClass>urn:content-classes:message</email:ContentClass>\
  <airsyncbase:NativeBodyType>1</airsyncbase:NativeBodyType>\
  <email2:ConversationId>1</email2:ConversationId>\
  <email2:ConversationIndex>3</email2:ConversationIndex>\
  </Properties>\
  </Fetch>\
  </Response>\
  </ItemOperations>";
#endif
  
  uint8_t *buf = (uint8_t *)_xmlResponse.c_str();
  int buf_len = (int)strlen((const char *)buf);
  
  reader = xmlReaderForMemory((const char *)buf, buf_len, NULL, "utf-8", 0);
  if (reader != NULL) {
    const xmlChar *curr_elm = nullptr;
    const xmlChar *curr_elm_prefix = nullptr;
    const xmlChar *curr_ele_full = nullptr;
    int depth = 0;
    while (xmlTextReaderRead(reader)) {
      int n_type = xmlTextReaderNodeType(reader);
      if (n_type == XNT_Element) {
        depth = xmlTextReaderDepth(reader);
        curr_ele_full = xmlTextReaderConstName(reader);
        curr_elm = xmlTextReaderConstLocalName(reader);
        curr_elm_prefix = xmlTextReaderConstPrefix(reader);
        
        if (strcmp((const char *)curr_elm, "Properties") == 0) {
          string properties = string((const char*)xmlTextReaderReadOuterXml(reader));
          _mail.decode(properties);
          xmlTextReaderNext(reader);
        }
        printf("current element %s \n", curr_ele_full);
        continue;
      } else if (n_type == XNT_Text && curr_elm != NULL) {
        
#define ToI32(v) sscanf((const char *)value, "%d", &v)
#define ToSTR(v) (string((const char*)v))
        
        const xmlChar *value = xmlTextReaderConstValue(reader);
        if (strcmp((const char *)curr_elm, "Status") == 0 && depth == 0) {
          sscanf((const char *)value, "%d", &_status);
        } else if (depth == 3) {
          if (strcmp((const char *)curr_elm, "Status") == 0) {
            ToI32(_fetchResponse.status);
          } else if (strcmp((const char *)curr_elm, "CollectionId") == 0) {
            _fetchResponse.collectionId = ToSTR(value);
          } else if (strcmp((const char *)curr_elm, "ServerId") == 0) {
            _fetchResponse.serverId = ToSTR(value);
          }else if (strcmp((const char *)curr_elm, "Class") == 0) {
            _fetchResponse.className = ToSTR(value);
          }
        }
      } else if (n_type == XNT_EndElement) {
        curr_elm = NULL;
      } else {
        continue;
      }
    }
    
    xmlFreeTextReader(reader);
  } else {
    throw std::invalid_argument("create xml reader failed!");
  }
}

SASItemOperationsResponse::~SASItemOperationsResponse() {
  
}