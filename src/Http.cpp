#include <Http.hpp>
#include <cstring>

constexpr int16_t CRLF = std::bit_cast<int16_t>(std::array{'\r', '\n'});
namespace adkostatt {
namespace Http {
const char *ParseHeader(const char *start, const char *end, const char **name,
                        int *const nameLength, const char **value,
                        int *const valueLength) {
  if (end - start < 3)
    return nullptr;
  start += 2;
  if (*start == '\r')
    return start + 1;
  *name = start;
  const char *nameStart = start;
  ++start;
  while (*start != ':')
    ++start;
  if (start >= end)
    return nullptr;
  *nameLength = start - nameStart;
  ++start;
  while (*start == ' ')
    ++start;
  if (start >= end)
    return nullptr;
  *value = start;
  const char *valueStart = start;
  ++start;
  while (*start != '\r')
    ++start;
  *valueLength = start - valueStart;
  return (start < end) ? start : nullptr;
}
const char *ParseRequestStart(const char *start, const char *end,
                              MethodType *const method, const char **uri,
                              int *const uriLength,
                              HttpVersion *const version) {
  if (end - start < 16)
    return nullptr;
  switch (*start) {
  case 'G':
    *method = MethodType::Get;
    start += 4;
    break;
  case 'O':
    *method = MethodType::Options;
    start += 8;
    break;
  case 'H':
    *method = MethodType::Head;
    start += 5;
    break;
  case 'P':
    switch (start[1]) {
    case 'O':
      *method = MethodType::Post;
      start += 5;
      break;
    case 'U':
      *method = MethodType::Put;
      start += 4;
      break;
    default:
      return nullptr;
    }
    break;
  case 'D':
    *method = MethodType::Delete;
    start += 7;
    break;
  case 'T':
    *method = MethodType::Trace;
    start += 6;
    break;
  case 'C':
    *method = MethodType::Connect;
    start += 8;
    break;
  default:
    return nullptr;
  }
  *uri = start;
  const char *uriStart = start;
  while (*start != ' ')
    ++start;
  if (start >= end)
    return nullptr;
  *uriLength = start - uriStart;
  if (end - start < 10)
    return nullptr;
  start += 6;
  HttpVersion hVersion = static_cast<HttpVersion>(*start);
  *version = hVersion;
  return start + 3 - (static_cast<int>(hVersion) & 2);
}
const char *ParseResponseStart(const char *start, const char *end,
                               HttpVersion *const version,
                               int32_t *const statusCode) {
  if (end - start < 14)
    return nullptr;
  start += 5;
  HttpVersion hVersion = static_cast<HttpVersion>(*start);
  *version = hVersion;
  start += 3 - (static_cast<int>(hVersion) & 2);
  *statusCode = *reinterpret_cast<const int32_t *>(start);
  start += 5;
  while (*start != '\r')
    ++start;
  return (start < end) ? start : nullptr;
}

char *GenerateRequestStart(char *start, const MethodType method,
                           const char *uri, const int uriLength,
                           const HttpVersion version) {
  switch (method) {
  case MethodType::Get:
    std::memcpy(start, "GET ", 4);
    start += 4;
    break;
  case MethodType::Post:
    std::memcpy(start, "POST ", 5);
    start += 5;
    break;
  case MethodType::Head:
    std::memcpy(start, "HEAD ", 5);
    start += 5;
    break;
  case MethodType::Options:
    std::memcpy(start, "OPTIONS ", 8);
    start += 8;
    break;
  case MethodType::Connect:
    std::memcpy(start, "CONNECT ", 8);
    start += 8;
    break;
  case MethodType::Delete:
    std::memcpy(start, "DELETE ", 7);
    start += 7;
    break;
  case MethodType::Put:
    std::memcpy(start, "PUT ", 4);
    start += 4;
    break;
  case MethodType::Trace:
    std::memcpy(start, "TRACE ", 6);
    start += 6;
    break;
  default:
    return nullptr;
  }
  std::memcpy(start, uri, uriLength);
  start += uriLength;
  *start = ' ';
  std::memcpy(start + 1, "HTTP/1.1", 8);
  start += 6;
  *start = static_cast<char>(version);
  start += 3 - (static_cast<int>(version) & 2);
  *reinterpret_cast<int16_t *>(start) = CRLF;
  return start + 2;
}

char *GenerateResponseStart(char *start, const HttpVersion version,
                            const int32_t status) {
  std::memcpy(start, "HTTP/1.1", 8);
  start += 5;
  *start = static_cast<char>(version);
  start += 3 - (static_cast<int>(version) & 2);
  *reinterpret_cast<int32_t *>(start) = status;
  *reinterpret_cast<int16_t *>(start + 4) = CRLF;
  return start + 6;
}

char *GenerateHeader(char *start, const char *name, const int nameLength,
                     const char *value, const int valueLength) {
  std::memcpy(start, name, nameLength);
  start += nameLength;
  *start = ':';
  std::memcpy(start + 1, value, valueLength);
  start += valueLength + 1;
  *reinterpret_cast<int16_t *>(start) = CRLF;
  return start + 2;
}

char *GenerateEnd(char *start) {
  *reinterpret_cast<int16_t *>(start) = CRLF;
  return start + 2;
}
} // namespace Http
} // namespace adkostatt
