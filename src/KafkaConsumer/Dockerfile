FROM python:3.11

COPY ./ /app
WORKDIR /app
RUN pip3 install confluent-kafka requests
ENV PYTHONUNBUFFERED=1

ENTRYPOINT ["python", "/app/consumer.py", "http://statistics-service:8020/api/v1/statistics", "logs", "kafka:29092", "1"]
