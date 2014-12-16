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
}

int main(int argc, const char * argv[]) {
//  cppTest();
  
  //make a test Call;

  SlimEAS::SASOptionsRequest req;
  
  std::future<void> res = std::async([&req]{
    req.testCurl();
  });
  
  std::cout << "start sleep";
  sleep(60);
  std::cout << "sleep over";
  
  return 0;
}
