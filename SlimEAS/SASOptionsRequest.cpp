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
#include <map>
#include <iterator>

#include "http_parser.h"

using namespace SlimEAS;
using namespace std;
using namespace curl;


//use for http parsing
class HttpHeaders {
public:
  HttpHeaders();
  ~HttpHeaders();
  
  http_parser *parser;
  ostringstream *o;
  map<string, string> *headers;
  
  string rawHeaders;
  
  string version;
  string statusCode;
  string status;
  
  void addHeader(const string& s);
  
 static int debug_callback(CURL *handle,
                     curl_infotype type,
                     char *data,
                     size_t size,
                     void *userptr);
  
  static size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
  
  static size_t HeaderCallback(void *buffer, size_t size, size_t nmemb, void *userp);
  
  //initial http_parser
  static int sockopt_callback(void *clientp, curl_socket_t curlfd, curlsocktype purpose);
};

HttpHeaders::HttpHeaders() {
  parser = (http_parser *)malloc(sizeof(http_parser));
  
  o = new ostringstream(std::ios_base::binary);
  
  headers = new map<string, string>();
}

HttpHeaders::~HttpHeaders() {
  if (parser) {
    free(parser);
  }
  
  delete o;
}

int HttpHeaders::debug_callback(CURL *handle,
                                curl_infotype type,
                                char *data,
                                size_t size,
                                void *userptr) {
  if (type  == CURLINFO_HEADER_OUT) {
    printf("output header: %s", data);
  }
  return 0;
}

void HttpHeaders::addHeader(const string &str) {
  string h(str);
  rawHeaders.append(str);
  
  if (str.compare("\r\n") == 0) {
    return;
  }
  
  size_t r = h.find_first_of("\r",0);
  h.erase(r, h.length()-1);
  
  size_t idx = h.find_first_of(":", 0);
  if (idx == std::string::npos) {
    std::istringstream ss(h);
    vector<string> v;
    string str;
    while (ss) {
      ss >> h;
      v.push_back(h);
    }
    version = v.front();
    statusCode = v.at(1);
    status = v.back();
  } else {
    string key = h.substr(0,idx);
    string value = h.substr(idx + 2, h.size()-1);
    headers->insert(pair<string, string>(key, value));
  }
}

int HttpHeaders::sockopt_callback(void *clientp, curl_socket_t curlfd, curlsocktype purpose) {
  HttpHeaders *pHeaders = (HttpHeaders *)clientp;
    
  http_parser_init(pHeaders->parser, HTTP_RESPONSE);
  
  return CURL_SOCKOPT_OK;
}

size_t HttpHeaders::write_callback(char *ptr, size_t size, size_t nmemb, void *userdata){
  HttpHeaders *pHttp = (HttpHeaders *)userdata;
  
  size_t result = 0;
  if (pHttp) {
    pHttp->o->write(ptr, size * nmemb);
    result = size * nmemb;
  }
  return result;
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
  
//  ostringstream o;
  
//  curl_writer writer(o);
  curl_easy easy;
  
  HttpHeaders headers;
  
  easy.add(curl_pair<CURLoption, string>(CURLOPT_URL, "http://www.baidu.com"));
  CURL *curl = easy.get_curl();

  curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
  curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, HttpHeaders::debug_callback);
  curl_easy_setopt(curl, CURLOPT_DEBUGDATA, &headers);
  
  curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)this->useSSL());
  
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpHeaders::write_callback);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &headers);
  
  curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, HttpHeaders::HeaderCallback);
  curl_easy_setopt(curl, CURLOPT_HEADERDATA, &headers);
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "OPTIONS");
  
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
  
//  curl_easy_setopt(curl, CURLOPT_SOCKOPTFUNCTION, HttpHeaders::sockopt_callback);
//  curl_easy_setopt(curl, CURLOPT_SOCKOPTDATA, &headers);
  
  easy.add(curl_pair<CURLoption, long>(CURLOPT_FOLLOWLOCATION, 1L));
  try {
    easy.perform();
    
    std::cout << headers.rawHeaders;
    
//    string str = headers.o->str();
//    std::cout << str;

    
    map<string, string>::iterator i = headers.headers->begin();
    
    for (; i != headers.headers->end(); i++) {
      std::cout << "header - " << (*i).first << ": " << (*i).second << "\n";
    }
    
  } catch (curl_easy_exception error) {
    vector<pair<string, string>> errors = error.what();
    error.print_traceback();
  }
  
  std::cout << "call over!";
}