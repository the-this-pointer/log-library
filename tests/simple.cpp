#include <logger.h>

using namespace std;
using namespace thisptr;

int main() {
  Logger logger("main", std::cout);

  logger(Logger::TRACE) << "trace test.." << std::endl;
  logger(Logger::DEBUG) << "debug test.." << std::endl;
  logger(Logger::INFO) << "info test.." << std::endl;
  logger(Logger::WARN) << "warn test.." << std::endl;
  logger(Logger::ERROR) << "error test.." << std::endl;
  logger(Logger::CRIT) << "crit test.." << std::endl;

  return 0;
}
