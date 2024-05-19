#include "KafkaProducer.h"

void KafkaProducer::PostToBroker(const LogString& logData) {
    json_t j;
    j["path"] = logData.path;
    j["method"] = logData.method;
    j["userName"] = logData.userName;
    j["responseCode"] = logData.responseCode;
    j["startedAt"] = logData.startedAt;
    j["finishedAt"] = logData.finishedAt;
    j["duration"] = logData.duration;

    std::string payload = j.dump();


    std::string escaped_string = oatpp::encoding::Base64::encode(payload.c_str(), strlen(payload.c_str()));

    LoggerFactory::GetLogger()->LogInfo((std::string("python3 /app/NetUI/producer/producer.py ") + brokers + " " + topic + " " + escaped_string).c_str());
    system((std::string("python3 /app/NetUI/producer/producer.py ") + brokers + " " + topic + " " + escaped_string).c_str());

}