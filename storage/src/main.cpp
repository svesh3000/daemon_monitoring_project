#include <iostream>
#include <filesystem>
#include <variant>
#include <httplib.h>
#include <json.hpp>

#include "shared-storage.hpp"
#include "models.hpp"

namespace
{
  models::SharedStorageConfig parseConfig(const std::string& path)
  {
    std::ifstream fin(path);
    if (!fin.is_open()) {
      throw std::logic_error("<Error>: not found config file");
    }
    nlohmann::json json_config = nlohmann::json::parse(fin);
    std::cout << json_config.dump(2) << '\n';
    return json_config.get< models::SharedStorageConfig >();
  }
}

int main(int argc, char** argv)
{
  namespace fs = std::filesystem;
  using json = nlohmann::json;
  using ordered_json = nlohmann::ordered_json;

  if (argc != 2) {
    std::cerr << "<Error>: incorrect args count, need config path\n";
    return 1;
  }
  models::SharedStorageConfig config = parseConfig(argv[1]);

  json json1 = json::parse(R"(
    {
      "cpu_usage": 52.0,
      "disk_usage": 23.0,
      "memory_usage": 63.0,
      "temperature": -1.0,
      "time": "2026-05-06T17:00:31Z"
    }
  )");
  json json2 = json::parse(R"(
    {
      "cpu_usage": 53.0,
      "disk_usage": 24.0,
      "memory_usage": 64.0,
      "temperature": -1.0,
      "time": "2026-05-06T17:00:36Z"
    }
  )");
  json json3 = json::parse(R"(
    {
      "cpu_usage": 54.0,
      "disk_usage": 25.0,
      "memory_usage": 65.0,
      "temperature": -1.0,
      "time": "2026-05-06T17:00:41Z"
    }
  )");
  json batch1 = json::parse(R"(
    [
      {
        "cpu_usage": 54.0,
        "disk_usage": 25.0,
        "memory_usage": 65.0,
        "temperature": -1.0,
        "time": "2026-05-06T17:00:46Z"
      },
      {
        "cpu_usage": 64.0,
        "disk_usage": 35.0,
        "memory_usage": 75.0,
        "temperature": -1.0,
        "time": "2026-05-06T17:00:51Z"
      }
    ]
  )");

  SharedStorage sh(config);
  std::cout << sh.hasMetric("server-a", "2026-05-06T17:00:51Z") << '\n';
  sh.addMetric("server-c", json1);
  sh.addMetric("server-b", json2);
  std::cout << sh.getTimedMetric("server-c", "2026-05-06T17:00:31Z") << '\n';
  std::cout << sh.getTimedMetric("server-b", "2026-05-06T17:00:36Z") << '\n';
  std::cout << sh.getIntervalBatch("server-b", "2026-05-06T17:00:31Z", "2026-05-06T17:00:41Z") << '\n';

  /*httplib::Server svr;

  fs::path metrics_dir = "./storage/metrics";
  svr.Get("/get_metric",
      [&metrics_dir](const httplib::Request & req, httplib::Response & res)
      {
        if (!req.has_param("name") || !req.has_param("time"))
        {
          json err = {{"error", "parameters 'name' and 'time' are required"}};
          res.status = 400;
          res.set_content(err.dump(2), "application/json");

          return;
        }

        std::string query_server_name = req.get_param_value("name");
        std::string query_metric_timestamp = req.get_param_value("time");

        fs::path dir_path = metrics_dir / query_server_name;
        for (const auto & entry : fs::directory_iterator(dir_path))
        {
          std::ifstream fin(entry.path().string());
          if (!fin)
          {
            json err = {{"error", "invalid metrics directory set for server"}};
            res.status = 500;
            res.set_content(err.dump(2), "application/json");

            return;
          }
          ordered_json jin = json::parse(fin);
          ordered_json jres;
          jres["version"] = jin["version"];
          jres["id"] = jin["id"];
          jres["name"] = jin["name"];
          jres["interval_value"] = jin["interval_value"];
          jres["interval_units"] = jin["interval_units"];
          for (auto & metric_item : jin["metrics"])
          {
            std::string timestamp = metric_item["time"];
            if (timestamp == query_metric_timestamp)
            {
              jres["time"] = metric_item["time"];
              jres["data"] = metric_item["data"];

              break;
            }
          }

          res.set_content(jres.dump(2), "application/json");

          return;
        }

        json err = {{"error", "no matching file found"}};
        res.status = 404;
        res.set_content(err.dump(2), "application/json");
      });

  std::cout << "Listening on port 8080..." << std::endl;
  svr.listen("0.0.0.0", 8080);*/
}
