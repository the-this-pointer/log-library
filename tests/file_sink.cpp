#include <logger.h>
#include <fstream>

using namespace std;
using namespace thisptr;

int main() {
  FileSink sink("filelog.log");
  Logger logger("main", sink);

  logger(Logger::TRACE) << "trace test.." << std::endl;
  logger(Logger::DEBUG) << "debug test.." << std::endl;
  logger(Logger::INFO) << "info test.." << std::endl;
  logger(Logger::WARN) << "warn test.." << std::endl;
  logger(Logger::ERR) << "error test.." << std::endl;
  logger(Logger::CRIT) << "crit test.." << std::endl;

  RotatedFileSink sink2 ("rotated.log", 3, 1000);
  Logger logger2("main2", sink2);
  logger2(Logger::TRACE) << "trace test.." << std::endl;
  logger2(Logger::DEBUG) << "debug test.." << std::endl;
  logger2(Logger::INFO) << "info test.." << std::endl;
  logger2(Logger::WARN) << "warn test.." << std::endl;
  logger2(Logger::ERR) << "error test.." << std::endl;
  logger2(Logger::CRIT) << "crit test.." << std::endl;

  return 0;
}
