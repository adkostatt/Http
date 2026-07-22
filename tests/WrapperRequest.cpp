#include <Http.hpp>
#include <HttpWrapper.hpp>
#include <doctest/doctest.h>
#include <string_view>
using namespace adkostatt::Http;

TEST_CASE("Usual request with wrapper") {
  char buffer[128];
  Generator generator(buffer, sizeof(buffer));
  CHECK(generator.GenerateRequest({"/", 1, MethodType::Get, HttpVersion::v1}) ==
        0);
  CHECK(generator.GenerateHeader({"Host", "google.com", 4, 10}) == 0);
  char *end = generator.Finalise();
  CHECK(end != nullptr);
  Parser parser(buffer, end);
  int32_t data = parser.SetDelimeters();
  Request request;

  CHECK(parser.Recognise() == InputType::Request);
  CHECK(parser.ParseRequest(request) != nullptr);
  CHECK(request.method == MethodType::Get);
  CHECK(std::string_view(request.uri, request.uriLength) == "/");
  CHECK(request.uriLength == 1);
  CHECK(request.version == HttpVersion::v1);

  Header header;
  const char *ptr = parser.ParseHeader(header);
  CHECK(ptr != nullptr);
  CHECK(std::string_view(header.name, header.nameLength) == "Host");
  CHECK(header.nameLength == 4);
  CHECK(std::string_view(header.value, header.valueLength) == "google.com");
  CHECK(header.valueLength == 10);

  CHECK(parser.ParseHeader(header) - ptr == 3);
  parser.RestoreData(data);
}
