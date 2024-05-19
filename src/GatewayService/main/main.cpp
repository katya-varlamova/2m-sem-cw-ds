
#include "OatppServer.hpp"
#include <boost/di.hpp>
#include <config/yamlcpp_config.h>
#include "producer/KafkaProducer.h"
int main(int argc, const char* argv[]) {
  if (argc < 2)
      return 1;
  std::string CONFIG_PATH = argv[1]; 
  auto injector = di::make_injector(
          di::bind<BaseConfig>().to<YamlCppConfig>(),
          di::bind<IProducer>().to<KafkaProducer>(),
          di::bind<std::string>.named( configFileName ).to( CONFIG_PATH ),
          di::bind<IServer>().to<OatppServer>());

  auto server = injector.create<IServerPtr>();
  //LoggerFactory::InitLogger( injector.create<BaseConfigPtr>());

  server->run();
  
  return 0;
}

