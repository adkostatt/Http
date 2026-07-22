#pragma once
#include <array>
#include <bit>
#include <cstdint>
#include <cstring>
namespace adkostatt {
namespace Http {
namespace StatusCode {
consteval int32_t StatusCodeGen(const char array[4]) {
  return std::bit_cast<int32_t>(
      std::array<char, 4>{array[0], array[1], array[2], array[3]});
}
constexpr int32_t Continue = StatusCodeGen(" 100");
constexpr int32_t SwitchingProtocols = StatusCodeGen(" 101");
constexpr int32_t Ok = StatusCodeGen(" 200");
constexpr int32_t Created = StatusCodeGen(" 201");
constexpr int32_t Accepted = StatusCodeGen(" 202");
constexpr int32_t NonAuthoritativeInformation = StatusCodeGen(" 203");
constexpr int32_t NoContent = StatusCodeGen(" 204");
constexpr int32_t ResetContent = StatusCodeGen(" 205");
constexpr int32_t PartialContent = StatusCodeGen(" 206");
constexpr int32_t MultipleChoices = StatusCodeGen(" 300");
constexpr int32_t MovedPermanently = StatusCodeGen(" 301");
constexpr int32_t Found = StatusCodeGen(" 302");
constexpr int32_t SeeOther = StatusCodeGen(" 303");
constexpr int32_t NotModified = StatusCodeGen(" 304");
constexpr int32_t UseProxy = StatusCodeGen(" 305");
constexpr int32_t TemporaryRedirect = StatusCodeGen(" 307");
constexpr int32_t BadRequest = StatusCodeGen(" 400");
constexpr int32_t Unauthorized = StatusCodeGen(" 401");
constexpr int32_t PaymentRequired = StatusCodeGen(" 402");
constexpr int32_t Forbidden = StatusCodeGen(" 403");
constexpr int32_t NotFound = StatusCodeGen(" 404");
constexpr int32_t MethodNotAllowed = StatusCodeGen(" 405");
constexpr int32_t NotAcceptable = StatusCodeGen(" 406");
constexpr int32_t ProxyAuthenticationRequired = StatusCodeGen(" 407");
constexpr int32_t RequestTimeout = StatusCodeGen(" 408");
constexpr int32_t Conflict = StatusCodeGen(" 409");
constexpr int32_t Gone = StatusCodeGen(" 410");
constexpr int32_t LengthRequired = StatusCodeGen(" 411");
constexpr int32_t PreconditionFailed = StatusCodeGen(" 412");
constexpr int32_t RequestEntityTooLarge = StatusCodeGen(" 413");
constexpr int32_t RequestUriTooLarge = StatusCodeGen(" 414");
constexpr int32_t UnsupportedMediaType = StatusCodeGen(" 415");
constexpr int32_t RequestedRangeNotSatisfiable = StatusCodeGen(" 416");
constexpr int32_t ExpectationFailed = StatusCodeGen(" 417");
constexpr int32_t InternalServerError = StatusCodeGen(" 500");
constexpr int32_t NotImplemented = StatusCodeGen(" 501");
constexpr int32_t BadGateway = StatusCodeGen(" 502");
constexpr int32_t ServiceUnavailable = StatusCodeGen(" 503");
constexpr int32_t GatewayTimeout = StatusCodeGen(" 504");
constexpr int32_t HttpVersionNotSupported = StatusCodeGen(" 505");
} // namespace StatusCode
enum class MethodType { Options, Get, Head, Post, Put, Delete, Trace, Connect };
enum class HttpVersion {
  v0 = '0',
  v1, // Often Http/1.1
  v2,
  v3
};

inline const char *SkipHeaders(const char *start, const char *end) {
  return static_cast<const char *>(memmem(start, start - end, "\r\n\r\n", 4));
}
// Array at the start must contain ':', ' ' and '\r' Or UB is possible
const char *ParseHeader(const char *start, const char *end, const char **name,
                        int *const nameLength, const char **value,
                        int *const valueLength);
// Array at the start must contain ' '. Or UB is possible
const char *ParseRequestStart(const char *start, const char *end,
                              MethodType *const method, const char **uri,
                              int *const uriLength, HttpVersion *const version);
// Array at the start must contain '\r'. Or UB is possinle
const char *ParseResponseStart(const char *start, const char *end,
                               HttpVersion *const version,
                               int32_t *const statusCode);
char *GenerateRequestStart(char *start, const MethodType method,
                           const char *uri, const int uriLength,
                           const HttpVersion version);
// Unable to write HttpVersion:v0 fully(HTTP/0.1 will be generated)
char *GenerateResponseStart(char *start, const HttpVersion version,
                            const int32_t status);
char *GenerateHeader(char *start, const char *name, const int nameLength,
                     const char *value, const int valueLength);

// Just writes CRLF
inline char *GenerateEnd(char *start) {
  constexpr int16_t CRLF = std::bit_cast<int16_t>(std::array{'\r', '\n'});
  *reinterpret_cast<int16_t *>(start) = CRLF;
  return start + 2;
}
} // namespace Http
} // namespace adkostatt
