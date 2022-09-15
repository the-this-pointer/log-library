#include <logger.h>
#include <fstream>

using namespace std;
using namespace thisptr;

int main() {
  Logger logger("main", std::cout);

  logger(Logger::TRACE) << "trace test.." << std::endl;
  logger(Logger::DEBUG) << "debug test.." << std::endl;
  logger(Logger::INFO) << "info test.." << std::endl;
  logger(Logger::WARN) << "warn test.." << std::endl;
  logger(Logger::ERR) << "error test.." << std::endl;
  logger(Logger::CRIT) << "crit test.." << std::endl;

  std::ofstream outfile;
  outfile.open("test.log", ios::out | ios::app );
  Logger logFile("main", outfile);

  logFile(Logger::TRACE) << "trace test.." << std::endl;
  logFile(Logger::DEBUG) << "debug test.." << std::endl;
  logFile(Logger::INFO) << "info test.." << std::endl;
  logFile(Logger::WARN) << "warn test.." << std::endl;
  logFile(Logger::ERR) << "error test.." << std::endl;
  logFile(Logger::CRIT) << "crit test.." << std::endl;
  outfile.close();

  return 0;
}
