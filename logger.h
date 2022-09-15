#ifndef LOG_LIBRARY_LOGGER_H
#define LOG_LIBRARY_LOGGER_H

#include <iostream>
#include <fstream>
#include <string>

#ifdef WINNT
#include <windows.h>
#endif

namespace thisptr {

  class FileSink: public std::ofstream {
  public:
    explicit FileSink(const std::string& path, std::ios_base::openmode openMode = std::ios::out | std::ios::app) {
      m_path = path;
      this->open(path, std::ios::out | std::ios::app );
    }

    ~FileSink() override {
      if (this->is_open())
        this->close();
    }

  private:
    std::string m_path;
  };

  class RotatedFileSink: private std::streambuf , public std::ostream {
  public:
    explicit RotatedFileSink(const std::string& path,
                    unsigned int maxBackups = 10,
                    unsigned long long maxFileSize = 1024):
                    std::ostream(this), m_path(path), m_maxFileCount(maxBackups), m_maxFileSize(maxFileSize) {
      m_path = path;
      checkRollOver();
      open();
    }

    ~RotatedFileSink() override {
      close();
    }

    void open() {
      close();
      m_file.open(m_path, std::ios::out | std::ios::app);
    }

    void close() {
      if (m_file.is_open())
        m_file.close();
    }

  protected:
    bool checkRollOver() {
#ifdef WINNT
      WIN32_FIND_DATAA data;
      HANDLE h = FindFirstFileA(m_path.c_str(), &data);
      if (h != INVALID_HANDLE_VALUE)
        m_currentFileSize = data.nFileSizeLow | (__int64)data.nFileSizeHigh << 32;
      FindClose(h);
#else
      FILE * pFile = NULL;
      pFile = fopen (m_path.c_str(), "rb");

      if (pFile != NULL) {
        fseek (pFile, 0, SEEK_END);
        m_currentFileSize = ftell(pFile);
        fclose (pFile);
      }
#endif
      if (m_currentFileSize > m_maxFileSize) {
        return true;
      }
      return false;
    }

    void rollOverIfNeeded() {
      if (m_currentFileSize > m_maxFileSize) {
        close();
        renameFiles();
        m_currentFileSize = 0;
        m_file.open(m_path, std::ios::out | std::ios::trunc);
      }
    }

    void renameFiles() {
      for (unsigned int i = m_maxFileCount; i > 0; i--) {
        if (FILE *file = fopen((m_path + "." + std::to_string(i)).c_str(), "r")) {
          fclose(file);

          rename((m_path + "." + std::to_string(i)).c_str(), (m_path + "." + std::to_string(i+1)).c_str());
        }
      }

      if (FILE *file = fopen((m_path + "." + std::to_string(m_maxFileCount+1)).c_str(), "r")) {
        fclose(file);

        remove((m_path + "." + std::to_string(m_maxFileCount+1)).c_str());
      }
      rename(m_path.c_str(), (m_path + "." + std::to_string(1)).c_str());
    }

  private:
    int overflow(int c) override
    {
      m_file.put(c);
      m_currentFileSize++;
      if (c == '\n') {
        rollOverIfNeeded();
      }
      return 0;
    }

    std::ofstream m_file;
    std::string m_path;
    unsigned long long m_currentFileSize;
    unsigned long long m_maxFileSize;
    const unsigned int m_maxFileCount;
  };

  class Logger {
  public:
    enum LogLevel {
      SILENT = 0,
      CRIT,
      ERR,
      WARN,
      INFO,
      DEBUG,
      TRACE
    };

    Logger(const std::string& name, std::ostream& sink):
        m_sink(sink) {
      m_name = name;
    }

    Logger(Logger& logger)=delete;
    Logger(Logger&& logger)=delete;
    Logger& operator= (Logger& logger)=delete;
    Logger& operator= (Logger&& logger)=delete;

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
        case ERR:     return "[E]";
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
