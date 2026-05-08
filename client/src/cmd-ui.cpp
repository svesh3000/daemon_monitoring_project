#include "cmd-ui.hpp"
#include <iostream>

void CMDUI::registerCommand(const std::string & name, command_handler handler)
{
  commands_[name] = handler;
}

void CMDUI::run()
{
  std::cout << "== commands ==\n";
  for (auto item : commands_)
  {
    std::cout << item.first << '\n';
  }

  std::cout << "> ";
  for (std::string command; !(std::cin >> command).eof();)
  {
    try
    {
      commands_.at(command)();
    }
    catch (const std::out_of_range &)
    {
      std::cout << "error: invalid command\n";
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    catch (const std::exception & e)
    {
      std::cout << "error: " << e.what() << '\n';
      if (std::cin.fail())
      {
        std::cin.clear(std::cin.rdstate() ^ std::ios::failbit);
      }
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    std::cout << "> ";
  }
}

void CMDUI::updateServers(std::map< std::string, ServerInfo > servers)
{
  for (auto it = servers.begin(); it != servers.end(); ++it)
  {
    std::cout << "server: " << it->first << " with " << it->second.url << "\n";
  }
}

void CMDUI::updateMetricGraph(const std::string & name, const std::string & pc_part,
    std::vector< std::pair< std::chrono::system_clock::time_point, metric_value > > values)
{
  std::cout << "metrics from " << name << ":\n";
  std::cout << "== " << pc_part << " ==\n";
  for (const auto & metric : values)
  {
    auto time_t = std::chrono::system_clock::to_time_t(metric.first);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    std::cout << ss.str() << " | " << metric.second << '\n';
  }
}
