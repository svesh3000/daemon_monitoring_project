#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <fstream>
#include <functional>
#include <httplib.h>
#include <memory>
#include "file-readers.hpp"
#include "ui.hpp"

class Client
{
public:
  Client(const ClientConfig & config);

  Client(const Client &) = delete;
  Client(Client &&) = delete;
  ~Client() = default;

  Client & operator=(const Client &) = delete;
  Client & operator=(Client &&) = delete;

  const std::string & getTimeMetric(const std::string & server_name, const std::string & timestamp);
  const std::string & getIntervalMetrics(
      const std::string & server_name, const std::string & begin_timestamp, const std::string & end_timestamp);
  void run();
  const std::map< std::string, ServerInfo > & loadConfig(const std::string & config_path);

  // void refreshAllMetrics();
  // void refreshMetricsFor(const std::string & server_name);

protected:
  ClientConfig config_;
  // std::unique_ptr< MetricsPackage > config_;

  const std::string & get(const std::string & query) const;
};

#endif
