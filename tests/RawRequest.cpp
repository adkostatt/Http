#include <Http.hpp>
#include <doctest/doctest.h>
#include <string_view>
using namespace adkostatt::Http;

TEST_CASE("Usual request") {
  const char http[] = "GET /search HTTP/1.1\r\nHost: google.com\r\n\r\n";
  const char *start = http, *end = http + sizeof(http) - 2;
  MethodType method = MethodType::Post;
  const char *uri = nullptr;
  int uriLength = 0;
  HttpVersion version = HttpVersion::v0;
  start = ParseRequestStart(start, end, &method, &uri, &uriLength, &version);

  CHECK(method == MethodType::Get);
  CHECK(std::string_view(uri, uriLength) == "/search");
  CHECK(uriLength == 7);
  CHECK(version == HttpVersion::v1);

  const char *name = nullptr, *value = nullptr;
  int nameLength = 0, valueLength = 0;
  start = ParseHeader(start, end, &name, &nameLength, &value, &valueLength);

  CHECK(std::string_view(name, nameLength) == "Host");
  CHECK(nameLength == 4);
  CHECK(std::string_view(value, valueLength) == "google.com");
  CHECK(valueLength == 10);

  start = ParseHeader(start, end, &name, &nameLength, &value, &valueLength);

  CHECK(start == end);
}

TEST_CASE("Unusual request") {
  const char http[] =
      "CONNECT google.com:443 HTTP/0.9\r\nHost:proxy.com\r\nUser-Id:     "
      "123\r\n\r\nSome data";
  const char *start = http, *end = http + sizeof(http) - 2;
  MethodType method = MethodType::Post;
  const char *uri = nullptr;
  int uriLength = 0;
  HttpVersion version = HttpVersion::v2;
  start = ParseRequestStart(start, end, &method, &uri, &uriLength, &version);

  CHECK(method == MethodType::Connect);
  CHECK(std::string_view(uri, uriLength) == "google.com:443");
  CHECK(uriLength == 14);
  CHECK(version == HttpVersion::v0);

  const char *name = nullptr, *value = nullptr;
  int nameLength = 0, valueLength = 0;
  start = ParseHeader(start, end, &name, &nameLength, &value, &valueLength);

  CHECK(std::string_view(name, nameLength) == "Host");
  CHECK(nameLength == 4);
  CHECK(std::string_view(value, valueLength) == "proxy.com");
  CHECK(valueLength == 9);
  start = ParseHeader(start, end, &name, &nameLength, &value, &valueLength);

  CHECK(std::string_view(name, nameLength) == "User-Id");
  CHECK(nameLength == 7);
  CHECK(std::string_view(value, valueLength) == "123");
  CHECK(valueLength == 3);
  start = ParseHeader(start, end, &name, &nameLength, &value, &valueLength);

  CHECK(*start == '\n');
}

TEST_CASE("Generate request") {
  char http[128];
  char *gstart =
      GenerateRequestStart(http, MethodType::Get, "/", 1, HttpVersion::v1);
  gstart = GenerateHeader(gstart, "Host", 4, "example.com", 11);
  gstart = GenerateEnd(gstart);

  const char *end = gstart - 1, *uri = nullptr;
  int uriLength = 0;
  const char *start = http;
  MethodType method = MethodType::Post;
  HttpVersion version = HttpVersion::v3;
  start = ParseRequestStart(start, end, &method, &uri, &uriLength, &version);

  CHECK(method == MethodType::Get);
  CHECK(std::string_view(uri, uriLength) == "/");
  CHECK(uriLength == 1);
  CHECK(version == HttpVersion::v1);

  const char *name = nullptr, *value = nullptr;
  int nameLength = 0, valueLength = 0;
  start = ParseHeader(start, end, &name, &nameLength, &value, &valueLength);

  CHECK(std::string_view(name, nameLength) == "Host");
  CHECK(nameLength == 4);
  CHECK(std::string_view(value, valueLength) == "example.com");
  CHECK(valueLength == 11);
  start = ParseHeader(start, end, &name, &nameLength, &value, &valueLength);

  CHECK(start == end);
}
