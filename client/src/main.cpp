#include <cstring>
#include <iostream>
#include <memory>
#include "client.hpp"
#include "cmd-ui.hpp"
#include "commands.hpp"

int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cerr << "Error: incorrect args count, need config path\n";
    return 1;
  }

  try
  {
    ClientConfig config(argv[1]);
    Client client(config);

    std::unique_ptr< UI > ui = std::make_unique< CMDUI >();
    ui->registerCommand("load-config", std::bind(client_commands::loadClientConfig, std::ref(client)));
    ui->registerCommand("print-time-metric", std::bind(client_commands::printTimeMetric, std::ref(client)));
    ui->registerCommand("print-interval-metrics", std::bind(client_commands::printIntervalMetrics, std::ref(client)));

    ui->run();
  }
  catch (const std::exception & e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return 2;
  }
}
