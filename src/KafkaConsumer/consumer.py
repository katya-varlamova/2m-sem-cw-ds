import time
import requests
from confluent_kafka import Consumer
import sys
import json
host = sys.argv[1]
topic_name = sys.argv[2]
kafka_broker = sys.argv[3]
group_id = sys.argv[4]
print(kafka_broker)
consumer = Consumer({
    'bootstrap.servers': kafka_broker,
    'group.id': group_id
})
try:
    consumer.create_topic(topic_name)
except:
    pass

consumer.subscribe([topic_name])


while True:
    message = consumer.poll(1)

    if message is None:
        continue
    if message.error():
        print('Ошибка при получении сообщения:', message.error())
        continue

    print('Получено сообщение:', message.value().decode('utf-8'))
    response = requests.post(host, data=message.value().decode('utf-8'))
