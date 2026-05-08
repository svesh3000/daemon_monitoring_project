#include "client.hpp"
#include <httplib.h>

Client::Client(const ClientConfig & config):
  config_(config)
{}

const std::map< std::string, ServerInfo > & Client::loadConfig(const std::string & config_path)
{
  ClientConfig newConfig(config_path);
  config_ = newConfig;

  return config_.getServerInfoMap();
}

const std::string & Client::getTimeMetric(const std::string & server_name, const std::string & timestamp)
{
  if (config_.getServerInfoMap().find(server_name) == config_.getServerInfoMap().end())
  {
    throw std::invalid_argument("invalid server name");
  }

  return get("/api/get?name=" + server_name + "&time=" + timestamp);
}

const std::string & Client::getIntervalMetrics(
    const std::string & server_name, const std::string & begin_timestamp, const std::string & end_timestamp)
{
  if (config_.getServerInfoMap().find(server_name) == config_.getServerInfoMap().end())
  {
    throw std::invalid_argument("invalid server name");
  }

  return get("/api/get?name=" + server_name + "&begin=" + begin_timestamp + "&end=" + end_timestamp);
}

// void Client::refreshAllMetrics()
// {}

// void Client::refreshMetricsFor(const std::string & server_name)
// {
//   MetricsPackage metrics;
//   // todo: check for config existing
//   metrics.load(config_.get()->getMetricsFilePath() + server_name);
//   // todo: comparing with critical values
//   auto temp = metrics.getServerMetrics();
//   std::vector< std::pair< std::chrono::system_clock::time_point, metric_value > > result;
//   for (size_t i = 0; i < temp.size(); ++i)
//   {
//     for (size_t j = 0; j < temp[0].metrics.size(); ++j)
//     {
//       // todo safety and smarter interface because this so complicated..
//       result.push_back({temp[i].metrics[j].time, temp[i].metrics[j].data["gpu"]["usage"]});
//     }
//   }

//   ui_->updateMetricGraph(server_name, "GPU USAGE", result);
// }

const std::string & Client::get(const std::string & query) const
{
  httplib::Client client("http://localhost:8080");

  auto res = client.Get(query);
  if (!res)
  {
    throw std::runtime_error(httplib::to_string(res.error()));
  }
  if (res->status != 200)
  {
    throw std::runtime_error("HTTP error: " + res->status);
  }

  return res->body;
}
