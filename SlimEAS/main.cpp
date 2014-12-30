//
//  main.cpp
//  SlimEAS
//
//  Created by envy.chen on 12/14/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <utility>
#include <string>

#include "SASWBXml.h"
#include "SASOptionsRequest.h"
#include "SASCommandRequest.h"
#include "SASProvisionRequest.h"
#include "SASProvisionResponse.h"

#include <unistd.h>

#include <thread>
#include <future>

#include <libxml/xmlwriter.h>

#include "SASDevice.h"

#define MY_ENCODING "ISO-8859-1"

static const char *prefix = "settings";

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
  std::string result = wb.toXML(output, outlen);
  std::cout << "load to xml , result: \n";
  std::cout << result << "\n";
  std::cout << "result file lenght:" << result.size() << "\n";
  
  free(output);
}

int main(int argc, const char * argv[]) {
//  uint32_t i=0x12345678;
//  cout<<hex<<i<<endl;
//  uint8_t *p = (uint8_t *)&i;        
//  if((*p==0x78)&(*(p+1)==0x56))
//    cout<<"little-endian"<<endl;
//  else if((*p==0x12)&(*(p+1)==0x34))
//    cout<<"big-endian"<<endl;
//  else
//    cout<<"unknow endianess!";
  
  xmlTest();
  
//  SlimEAS::SASDevice device;
//  device.setModel("testModel");
//  device.setIMEI("testIMEI");
//  device.setFriendlyName("testDevice");
//  device.setOS("iphone os 8.0");
//  device.setOS_Lang("english");
//  device.setPhoneNumber("18603008614");
//  device.setMobileOperator("noOperator");
//  device.setUserAgent("SlimEAS");
//  
//  string deviceXml = device.payload();
//  
//  std::cout << deviceXml;
//  
////  cppTest();
//  
//  //option request test
//  SlimEAS::SASOptionsRequest optReq("https://ex.qq.com", "", "");
//  SlimEAS::SASOptionsResponse resS = optReq.getReponse();
//  
//  std::cout << "Supported Versions: " << resS.supportedVersions << "\n";
//  std::cout << "Supported Commands: " << resS.supportedCommand  << "\n";
  
  //command request test
//  SlimEAS::SASCommandRequest req("https://ex.qq.com", "chenxu@nationsky.com", "123456abcA");
//  req.command("Provision");
//  req.deviceID("6F24CAD599A5BF1A690246B8C68FAE8D");
//  req.deviceType("SmartPhone");
//  req.protocolVersion("14.0");
//  req.useEncodeRequestLine(false);
//  
//  std::string xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
//  xml.append("<!DOCTYPE ActiveSync PUBLIC \"-//MICROSOFT//DTD ActiveSync//EN\" \"http://www.microsoft.com/\">\n");
//  xml.append("<Provision xmlns=\"Provision:\" xmlns:settings=\"Settings:\">\n");
//  xml.append("    <settings:DeviceInformation>\n");
//  xml.append("        <settings:Set>\n");
//  xml.append("            <settings:Model>Test 1.0</settings:Model>\n");
//  xml.append("            <settings:IMEI>012345678901234</settings:IMEI>\n");
//  xml.append("            <settings:FriendlyName>My Test App</settings:FriendlyName>\n");
//  xml.append("            <settings:OS>iOS 8.1</settings:OS>\n");
//  xml.append("            <settings:OSLanguage>English</settings:OSLanguage>\n");
//  xml.append("            <settings:PhoneNumber>555-123-4567</settings:PhoneNumber>\n");
//  xml.append("            <settings:MobileOperator>Nationsky</settings:MobileOperator>\n");
//  xml.append("            <settings:UserAgent>Slim-EAS</settings:UserAgent>\n");
//  xml.append("        </settings:Set>\n");
//  xml.append("    </settings:DeviceInformation>\n");
//  xml.append("     <Policies>\n");
//  xml.append("          <Policy>\n");
//  xml.append("               <PolicyType>MS-EAS-Provisioning-WBXML</PolicyType> \n");
//  xml.append("          </Policy>\n");
//  xml.append("     </Policies>\n");
//  xml.append("</Provision>");
//  req.setXMLPayload(xml);
//
//  SlimEAS::SASHTTPResponse *res = req.getResponse();
  
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
  
  SlimEAS::SASProvisionRequest provRequest;
  provRequest.setServer("https://ex.qq.com");
  provRequest.setUser("chenxu@nationsky.com");
  provRequest.setPassword("123456abcA");
  provRequest.setUseSSL(true);
  provRequest.setDeviceId("6F24CAD599A5BF1A690246B8C68FAE8D");
  provRequest.setDeviceType("SmartPhone");
  provRequest.setProtocolVersion("14.0");
  provRequest.setUseEncodeRequestLine(false);
  provRequest.setProvisionDevice(provDevice);
  
  SlimEAS::SASProvisionResponse *provRes = dynamic_cast<SlimEAS::SASProvisionResponse *>(provRequest.getResponse());
 
//  std::future<SlimEAS::SASOptionsResponse> res = std::async([&req]{
//    return req.getReponse();
//  });
//  
//  std::cout << "start sleep";
//  sleep(60);
//  std::cout << "sleep over";
  
  return 0;
}
