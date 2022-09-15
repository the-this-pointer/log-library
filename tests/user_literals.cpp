#include <logger.h>
#include <iostream>

using namespace std;
using namespace thisptr;

#ifdef __cpp_user_defined_literals
using namespace thisptr::logger_literals;

int main() {
  // to initialize sink for static instance
  Logger::getInstance("main", std::cout);

  "this is a critical error"_c;
  "this is an error"_e;
  "this is a warning"_w;
  "this is a info"_i;
  "this is a debug"_d;
  "this is a trace"_t;

  return 0;
}
#endif
