//
//  SASOptionsRequest.cpp
//  SlimEAS
//
//  Created by envy.chen on 12/15/14.
//  Copyright (c) 2014 Slim. All rights reserved.
//

#include "SASOptionsRequest.h"

#include "curl_easy.h"
#include "curl_receiver.h"
#include <thread>
#include <sstream>
#include <ios>

using namespace SlimEAS;
using namespace std;
using namespace curl;

class HttpHeaders {
public:
  string headers;
  
  void addHeader(const string& s);
  
  static size_t HeaderCallback(void *buffer, size_t size, size_t nmemb, void *userp);

};

void HttpHeaders::addHeader(const string &s) {
  this->headers.append(s);
}

size_t HttpHeaders::HeaderCallback(void *buffer, size_t size, size_t nmemb, void *userp) {
  char *d = (char *)buffer;
  HttpHeaders *pHeaders = (HttpHeaders *)userp;
  
  size_t result = 0;
  if (pHeaders != NULL) {
    string s = "";
    s.append(d, size * nmemb);
    pHeaders->addHeader(s);
    result = size * nmemb;
  }
  
  return result;
}

//get httpResponse headers

SASOptionsRequest::SASOptionsRequest():
_useSSL(true){}

SASOptionsRequest::~SASOptionsRequest(){

}

void SASOptionsRequest::testCurl() {
  std::cout << "start test call \n";
  
  ostringstream o;
  
  curl_writer writer(o);
  curl_easy easy(writer);
  
  HttpHeaders headers;
  
  easy.add(curl_pair<CURLoption, string>(CURLOPT_URL, "http://www.baidu.com"));
  CURL *curl = easy.get_curl();
  
  curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HttpHeaders::HeaderCallback);
  easy.add(curl_pair<CURLoption, HttpHeaders *>(CURLOPT_HEADERDATA, &headers));
  easy.add(curl_pair<CURLoption, long>(CURLOPT_FOLLOWLOCATION, 1L));
  try {
    easy.perform();
    
    std::cout << headers.headers;
    
    string str = o.str();
    std::cout << str;
    
  } catch (curl_easy_exception error) {
    vector<pair<string, string>> errors = error.what();
    error.print_traceback();
  }
  
  std::cout << "call over!";
}