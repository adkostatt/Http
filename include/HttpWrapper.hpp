#pragma once
#include <Http.hpp>

namespace adkostatt {
namespace Http {
enum class InputType { Request, Response };
enum class ReturnType { Error, Continue, BodyReady };
class Parser {
private:
  static constexpr int32_t Delimiters = StatusCode::StatusCodeGen("\r \r:");
  char *start, *end;
  const int32_t data;
  const int ParseHeaders(const char **names, int *nameLengths,
                         const char **values, int *valueLengths,
                         const int headersLimit);

public:
  Parser(char *start, char *end);
  Parser(char *buffer, const int length);
  ~Parser();
  const InputType Recognise();
  const int ParseRequest(MethodType *const method, const char **uri,
                         int *const uriLength, HttpVersion *const version,
                         const char **names, int *nameLengths,
                         const char **values, int *valueLengths,
                         const int headersLimit);
  const int ParseResponse(HttpVersion *const version, int *const status,
                          const char **names, int *nameLengths,
                          const char **values, int *valueLengths,
                          const int headersLimit);
  char *GetStart();
};

class Generator {
private:
  char *start, *end;
  const int GenerateHeaders(const char **names, const int *nameLengths,
                            const char **values, const int *valueLengths,
                            const int count);

public:
  Generator(char *start, char *end);
  Generator(char *buffer, const int length);
  const int GenerateRequest(const MethodType method, const char *uri,
                            const int uriLength, const HttpVersion version,
                            const char **names, const int *nameLengths,
                            const char **values, const int *valueLengths,
                            const int headers);
  const int GenerateResponse(const HttpVersion version, const int status,
                             const char **names, const int *nameLengths,
                             const char **values, const int *valueLengths,
                             const int headers);
  char *GetStart();
};
} // namespace Http
} // namespace adkostatt
