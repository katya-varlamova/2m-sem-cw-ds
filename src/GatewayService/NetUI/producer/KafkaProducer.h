#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

#include "oatpp/encoding/Base64.hpp"

#include "exceptions/logic_exceptions.h"
#include "logger/LoggerFactory.h"
#include <boost/di.hpp>
#include "IProducer.h"
using json_t = nlohmann::json;

class KafkaProducer : public IProducer {
public:
    BOOST_DI_INJECT(KafkaProducer, const std::shared_ptr<BaseConfig>& config ) 
    {
        brokers = std::string(config->GetStringField( { BrokerAddressSection }));
        topic = std::string(config->GetStringField( { BrokerTopicSection }));
    } 
    void PostToBroker(const LogString& logData) override ;
    
    ~KafkaProducer() override {}
private:
    std::string errstr;
    std::string brokers;
    std::string topic;
};

