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
  generator.GenerateResponse(HttpVersion::v1, StatusCode::BadGateway, names,
                             nameLengths, values, valueLengths, 1);
  char *end = generator.GetStart();
  HttpVersion version = HttpVersion::v0;
  int32_t status = StatusCode::Accepted;
  Parser parser(buffer, end);

  CHECK(parser.Recognise() == InputType::Response);
  CHECK(parser.ParseResponse(&version, &status, names, nameLengths, values,
                             valueLengths, 1) == 1);
  CHECK(version == HttpVersion::v1);
  CHECK(status == StatusCode::BadGateway);
  CHECK(std::string_view(names[0], nameLengths[0]) == "Host");
  CHECK(nameLengths[0] == 4);
  CHECK(std::string_view(values[0], valueLengths[0]) == "google.com");
  CHECK(valueLengths[0] == 10);
}
