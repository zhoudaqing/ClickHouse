#pragma once

#include "IServer.h"

#include <common/logger_useful.h>
#include <daemon/BaseDaemon.h>

/** Server provides three interfaces:
  * 1. HTTP - simple interface for any applications.
  * 2. TCP - interface for native clickhouse-client and for server to server internal communications.
  *    More rich and efficient, but less compatible
  *     - data is transferred by columns;
  *     - data is transferred compressed;
  *    Allows to get more information in response.
  * 3. Interserver HTTP - for replication.
  */


namespace DB
{

class Server : public BaseDaemon, public IServer
{
public:
    Poco::Util::LayeredConfiguration & config() const override
    {
        return BaseDaemon::config();
    }

    Poco::Logger & logger() const override
    {
        return BaseDaemon::logger();
    }

    Context & context() const override
    {
        return *global_context;
    }

    bool isCancelled() const override
    {
        return BaseDaemon::isCancelled();
    }

protected:
    void initialize(Application & self) override
    {
        BaseDaemon::initialize(self);
        logger().information("starting up");
    }

    void uninitialize() override
    {
        logger().information("shutting down");
        BaseDaemon::uninitialize();
    }

    int main(const std::vector<std::string> & args) override;

    std::string getDefaultCorePath() const override;

private:
    std::unique_ptr<Context> global_context;
};

}
