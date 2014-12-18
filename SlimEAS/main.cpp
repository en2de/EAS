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

#include <unistd.h>

#include <thread>
#include <future>

void cppTest() {
  std::string str;
  
  str = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n" \
  "<!DOCTYPE ActiveSync PUBLIC \"-//MICROSOFT//DTD ActiveSync//EN\" \"http://www.microsoft.com/\">\n" \
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
  std::string *result = wb.toXML(output, outlen);
  std::cout << "load to xml , result: \n";
  std::cout << *result << "\n";
  std::cout << "result file lenght:" << result->size() << "\n";
  
  free(output);
  delete result;
}

int main(int argc, const char * argv[]) {
  uint32_t i=0x12345678;
  cout<<hex<<i<<endl;
  uint8_t *p = (uint8_t *)&i;        
  if((*p==0x78)&(*(p+1)==0x56))
    cout<<"little-endian"<<endl;
  else if((*p==0x12)&(*(p+1)==0x34))
    cout<<"big-endian"<<endl;
  else
    cout<<"unknow endianess!";
  
  cppTest();
  
  //option request test
  SlimEAS::SASOptionsRequest optReq("https://ex.qq.com", "", "");
  SlimEAS::SASOptionsResponse resS = optReq.getReponse();
  
  std::cout << "Supported Versions: " << resS.supportedVersions << "\n";
  std::cout << "Supported Commands: " << resS.supportedCommand  << "\n";
  
  //command request test
  SlimEAS::SASCommandRequest req("https://ex.qq.com", "chenxu@nationsky.com", "123456abcA");
  req.command("Provision");
  req.deviceID("6F24CAD599A5BF1A690246B8C68FAE8D");
  req.deviceType("SmartPhone");
  req.protocolVersion("14.0");
  req.useEncodeRequestLine(false);
  
  std::string xml = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
  xml.append("<!DOCTYPE ActiveSync PUBLIC \"-//MICROSOFT//DTD ActiveSync//EN\" \"http://www.microsoft.com/\">\n");
  xml.append("<Provision xmlns=\"Provision:\" xmlns:settings=\"Settings:\">\n");
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
  req.requestBody(xml);

  SlimEAS::SASHTTPResponse *res = req.getResponse();
 
//  std::future<SlimEAS::SASOptionsResponse> res = std::async([&req]{
//    return req.getReponse();
//  });
//  
//  std::cout << "start sleep";
//  sleep(60);
//  std::cout << "sleep over";
  
  return 0;
}
