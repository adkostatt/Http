#include <Http.hpp>
#include <HttpWrapper.hpp>

namespace adkostatt {
namespace Http {
const int Parser::ParseHeaders(const char **names, int *nameLengths,
                               const char **values, int *valueLengths,
                               const int headersLimit) {
  int headers = 0;
  for (; headers < headersLimit; ++headers) {
    start = const_cast<char *>(
        ParseHeader(start, end, names, nameLengths, values, valueLengths));
    if (start == nullptr)
      return -1;
    else if (*start == '\n') {
      ++start;
      return 0;
    }
    ++names;
    ++nameLengths;
    ++values;
    ++valueLengths;
  }
  return headers;
}
Parser::Parser(char *start, char *end)
    : start(start), end(end), data(*(reinterpret_cast<int32_t *>(end - 4))) {
  *reinterpret_cast<int32_t *>(end - 4) = Delimiters;
}
Parser::Parser(char *buffer, const int length)
    : Parser(buffer, buffer + length - 1) {}
Parser::~Parser() { *reinterpret_cast<int32_t *>(end - 4) = data; }
const InputType Parser::Recognise() {
  return start[1] == 't' ? InputType::Response : InputType::Request;
}
const int Parser::ParseRequest(MethodType *const method, const char **uri,
                               int *const uriLength, HttpVersion *const version,
                               const char **names, int *nameLengths,
                               const char **values, int *valueLengths,
                               const int headersLimit) {
  start = const_cast<char *>(
      ParseRequestStart(start, end, method, uri, uriLength, version));
  return ParseHeaders(names, nameLengths, values, valueLengths, headersLimit);
}
const int Parser::ParseResponse(HttpVersion *const version, int *const status,
                                const char **names, int *nameLengths,
                                const char **values, int *valueLengths,
                                const int headersLimit) {
  start = const_cast<char *>(ParseResponseStart(start, end, version, status));
  return ParseHeaders(names, nameLengths, values, valueLengths, headersLimit);
}
char *Parser::GetStart() { return start; }

const int Generator::GenerateHeaders(const char **names, const int *nameLengths,
                                     const char **values,
                                     const int *valueLengths, const int count) {
  int totalLength = count * 3;
  for (int i = 0; i < count; ++i)
    totalLength += nameLengths[i] + valueLengths[i];
  if (end - start < totalLength)
    return totalLength - static_cast<int>(end - start);
  for (int i = 0; i < count; ++i) {
    start = GenerateHeader(start, *names, *nameLengths, *values, *valueLengths);
    ++names;
    ++nameLengths;
    ++values;
    ++valueLengths;
  }
  return 0;
}
Generator::Generator(char *start, char *end) : start(start), end(end) {}
Generator::Generator(char *buffer, const int length)
    : start(buffer), end(buffer + length - 1) {}
const int Generator::GenerateRequest(
    const MethodType method, const char *uri, const int uriLength,
    const HttpVersion version, const char **names, const int *nameLengths,
    const char **values, const int *valueLengths, const int headers) {
  const int lineLength = 21 + uriLength;
  if (end - start < lineLength)
    return lineLength - static_cast<int>(end - start);
  start = GenerateRequestStart(start, method, uri, uriLength, version);
  const int headersLengthNeeded =
      GenerateHeaders(names, nameLengths, values, valueLengths, headers);
  if (headersLengthNeeded)
    return headersLengthNeeded;
  start = GenerateEnd(start);
  return 0;
}
const int Generator::GenerateResponse(const HttpVersion version,
                                      const int status, const char **names,
                                      const int *nameLengths,
                                      const char **values,
                                      const int *valueLengths,
                                      const int headers) {
  if (end - start < 17)
    return 17 - static_cast<int>(end - start);
  start = GenerateResponseStart(start, version, status);
  const int headersLengthNeeded =
      GenerateHeaders(names, nameLengths, values, valueLengths, headers);
  if (headersLengthNeeded)
    return headersLengthNeeded;
  start = GenerateEnd(start);
  return 0;
}
char *Generator::GetStart() { return start; }
} // namespace Http
} // namespace adkostatt
