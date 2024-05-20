FROM node:16.14.2

WORKDIR /app
COPY ./frontend/ .
RUN npm install --silent --force

RUN npm run build
EXPOSE 3000

CMD ["npm", "start"]
