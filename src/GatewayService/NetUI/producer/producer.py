import time
import base64
from confluent_kafka import Producer, KafkaError
import sys
import re
kafka_broker = sys.argv[1]
topic_name = sys.argv[2]
message = sys.argv[3]

producer = Producer({
    'bootstrap.servers': kafka_broker
})

message = base64.b64decode(message).decode("utf-8") 

print(message)


try:
    producer.produce(topic_name, message.encode('utf-8'))
    producer.flush()
except KafkaError as e:
    if e.code() == KafkaError._PARTITION_NOT_FOUND:
        producer.create_topic(topic_name)
        print('Создан топик:', topic_name)
        producer.produce(topic_name, message.encode('utf-8'))
        producer.flush()
    else:
        print('Ошибка при отправке сообщения:', e)

