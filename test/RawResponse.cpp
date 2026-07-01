#include <Http.hpp>
#include <doctest/doctest.h>
#include <string_view>
using namespace adkostatt::Http;

TEST_CASE("Usual response") {
  const char http[] =
      "HTTP/1.1 200 OK\r\nDate: Mon, 15 Jan 2024 12:00:00 GMT\r\n\r\n";
  const char *start = http, *end = http + sizeof(http) - 2;
  HttpVersion version = HttpVersion::v0;
  int32_t status = StatusCode::Accepted;
  start = ParseResponseStart(start, end, &version, &status);

  CHECK(version == HttpVersion::v1);
  CHECK(status == StatusCode::Ok);

  const char *name = nullptr, *value = nullptr;
  int nameLength = 0, valueLength = 0;
  start = ParseHeader(start, end, &name, &nameLength, &value, &valueLength);

  CHECK(std::string_view(name, nameLength) == "Date");
  CHECK(nameLength == 4);
  CHECK(std::string_view(value, valueLength) ==
        "Mon, 15 Jan 2024 12:00:00 GMT");
  CHECK(valueLength == 29);
}
