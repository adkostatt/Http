#pragma once
#include <Http.hpp>

namespace adkostatt {
namespace Http {
enum class InputType { Request, Response };
struct Request {
  const char *uri;
  int uriLength;
  MethodType method;
  HttpVersion version;
};
struct Response {
  int status;
  HttpVersion version;
};
struct Header {
  const char *name;
  const char *value;
  int nameLength;
  int valueLength;
};
class Parser {
private:
  static constexpr int32_t Delimiters = StatusCode::StatusCodeGen("\r \r:");
  char *start, *end;

public:
  Parser(char *start, char *end) : start(start), end(end) {}
  Parser(char *buffer, const int length)
      : Parser(buffer, buffer + length - 1) {}
  const InputType Recognise() {
    return start[1] == 'T' ? InputType::Response : InputType::Request;
  }
  const char *ParseRequest(Request &request) {
    start = const_cast<char *>(
        ParseRequestStart(start, end, &request.method, &request.uri,
                          &request.uriLength, &request.version));
    return start;
  }
  const char *ParseResponse(Response &response) {
    start = const_cast<char *>(
        ParseResponseStart(start, end, &response.version, &response.status));
    return start;
  }
  const char *ParseHeader(Header &header) {
    start = const_cast<char *>(
        Http::ParseHeader(start, end, &header.name, &header.nameLength,
                          &header.value, &header.valueLength));
    return start;
  }
  const int32_t SetDelimeters() {
    const int32_t data = *reinterpret_cast<int32_t *>(end - 4);
    *reinterpret_cast<int32_t *>(end - 4) = Delimiters;
    return data;
  }
  void RestoreData(const int32_t data) {
    *reinterpret_cast<int32_t *>(end - 4) = data;
  }
};

class Generator {
private:
  char *start, *end;

public:
  Generator(char *start, char *end) : start(start), end(end) {}
  Generator(char *buffer, const int length)
      : start(buffer), end(buffer + length - 1) {}
  // Return required length for request(or zero on succesful)
  int64_t GenerateRequest(const Request &request) {
    const int64_t requiredLength =
        21 + request.uriLength - static_cast<int64_t>(end - start);
    if (requiredLength > 0)
      return requiredLength;
    start = GenerateRequestStart(start, request.method, request.uri,
                                 request.uriLength, request.version);
    return 0;
  }
  int64_t GenerateResponse(const Response &response) {
    const int64_t requiredLength = 17 - static_cast<int64_t>(end - start);
    if (requiredLength > 0)
      return requiredLength;
    start = GenerateResponseStart(start, response.version, response.status);
    return 0;
  }
  int64_t GenerateHeader(const Header &header) {
    const int64_t requiredLength = 5 + header.nameLength + header.valueLength -
                                   static_cast<int64_t>(end - start);
    if (requiredLength > 0)
      return requiredLength;
    start = Http::GenerateHeader(start, header.name, header.nameLength,
                                 header.value, header.valueLength);
    return 0;
  }
  char *Finalise() {
    if (end - start < 2)
      return nullptr;
    return GenerateEnd(start);
  }
};
} // namespace Http
} // namespace adkostatt
