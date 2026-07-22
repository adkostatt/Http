#include <Http.hpp>
#include <HttpWrapper.hpp>
#include <doctest/doctest.h>
#include <string_view>
using namespace adkostatt::Http;

TEST_CASE("Usual response with wrapper") {
  char buffer[128];
  Generator generator(buffer, sizeof(buffer));
  CHECK(generator.GenerateResponse({StatusCode::BadGateway, HttpVersion::v1}) ==
        0);
  CHECK(generator.GenerateHeader({"Host", "google.com", 4, 10}) == 0);
  char *end = generator.Finalise();
  CHECK(end != nullptr);
  Parser parser(buffer, end);
  int32_t data = parser.SetDelimeters();
  Response response;

  CHECK(parser.Recognise() == InputType::Response);
  CHECK(parser.ParseResponse(response) != nullptr);
  CHECK(response.version == HttpVersion::v1);
  CHECK(response.status == StatusCode::BadGateway);

  Header header{nullptr, nullptr, 0, 0};
  const char *ptr = parser.ParseHeader(header);
  CHECK(ptr != nullptr);
  CHECK(header.name != nullptr);
  CHECK(std::string_view(header.name, header.nameLength) == "Host");
  CHECK(header.nameLength == 4);
  CHECK(std::string_view(header.value, header.valueLength) == "google.com");
  CHECK(header.valueLength == 10);

  CHECK(parser.ParseHeader(header) - ptr == 3);
  parser.RestoreData(data);
}
