#ifndef LOG_LIBRARY_LOGGER_H
#define LOG_LIBRARY_LOGGER_H

#include <iostream>
#include <string>

namespace thisptr {

  class Logger {
  public:
    enum LogLevel {
      SILENT = 0,
      CRIT,
      ERROR,
      WARN,
      INFO,
      DEBUG,
      TRACE
    };

    Logger(const std::string& name, std::ostream& sink):
        m_sink(sink) {
      m_name = name;
    }

    void setLogLevel (const LogLevel& level) {
      m_level = level;
    }

    const LogLevel& logLevel() {
      return m_level;
    }

    Logger& operator() (const LogLevel& level) {
      if (level <= m_level && level > SILENT) {
        time_t now;
        time(&now);
        struct tm* t = localtime(&now);
        m_current_level = level;
        m_sink << ToString(t) << ToString(level) << " ";
      }
      return *this;
    }

    template<typename T>
    friend Logger& operator << (Logger& logger, const T& t);

    // This part is taken from: https://stackoverflow.com/questions/1134388/stdendl-is-of-unknown-type-when-overloading-operator
    // this is the type of std::cout
    typedef std::basic_ostream<char, std::char_traits<char> > CoutType;

    // this is the function signature of std::endl
    typedef CoutType& (*StandardEndLine)(CoutType&);

    // define an operator<< to take in std::endl
    Logger& operator<<(StandardEndLine manip)
    {
      // call the function, but we cannot return it's value
      manip(m_sink);
      return *this;
    }

  private:
    static inline std::string ToString(const LogLevel& lvl)
    {
      switch (lvl)
      {
        case CRIT:    return "[C]";
        case ERROR:   return "[E]";
        case WARN:    return "[W]";
        case INFO:    return "[I]";
        case DEBUG:   return "[D]";
        case TRACE:   return "[T]";
        default:      return "[Unknown]";
      }
    }

    static inline std::string ToString(struct tm* t)
    {
      std::string Y = std::to_string(t->tm_year + 1900);
      std::string M = std::to_string(t->tm_mon + 1);
      std::string D = std::to_string(t->tm_mday);
      std::string h = std::to_string(t->tm_hour);
      std::string m = std::to_string(t->tm_min);
      std::string s = std::to_string(t->tm_sec);

      if (t->tm_sec < 10) s = "0" + s;
      if (t->tm_min < 10) m = "0" + m;
      if (t->tm_hour < 10) h = "0" + h;
      if (t->tm_mday < 10) D = "0" + D;
      if (t->tm_mon + 1 < 10) M = "0" + M;

      return "[" + Y + "-" + M + "-" + D + " " + h + ":" + m + ":" + s + "]";
    }

    std::string m_name;
    LogLevel m_level {TRACE};
    LogLevel m_current_level {TRACE};
    std::ostream& m_sink;
  };

  template<typename T>
  Logger &operator<<(Logger &logger, const T &t) {
    //logger(logger.m_current_level) << t;
    if (logger.m_current_level <= logger.logLevel() && logger.logLevel() > Logger::SILENT) {
      logger.m_sink << t;
    }
    return logger;
  }

}

#endif //LOG_LIBRARY_LOGGER_H
