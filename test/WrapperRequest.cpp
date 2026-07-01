#include <Http.hpp>
#include <HttpWrapper.hpp>
#include <doctest/doctest.h>
#include <string_view>
using namespace adkostatt::Http;

TEST_CASE("Usual request with wrapper") {
  char buffer[128];
  Generator generator(buffer, sizeof(buffer));
  const char *names[]{"Host"};
  const char *values[]{"google.com"};
  int nameLengths[]{4};
  int valueLengths[]{10};
  generator.GenerateRequest(MethodType::Get, "/", 1, HttpVersion::v1, names,
                            nameLengths, values, valueLengths, 1);
  char *end = generator.GetStart();
  MethodType method = MethodType::Post;
  const char *uri = nullptr;
  int uriLength = 0;
  HttpVersion version = HttpVersion::v0;
  Parser parser(buffer, end);

  CHECK(parser.Recognise() == InputType::Request);
  CHECK(parser.ParseRequest(&method, &uri, &uriLength, &version, names,
                            nameLengths, values, valueLengths, 1) == 1);
  CHECK(method == MethodType::Get);
  CHECK(std::string_view(uri, uriLength) == "/");
  CHECK(uriLength == 1);
  CHECK(version == HttpVersion::v1);
  CHECK(std::string_view(names[0], nameLengths[0]) == "Host");
  CHECK(nameLengths[0] == 4);
  CHECK(std::string_view(values[0], valueLengths[0]) == "google.com");
  CHECK(valueLengths[0] == 10);
}
