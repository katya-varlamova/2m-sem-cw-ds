\c tickets

CREATE TABLE ticket
(
    id            SERIAL PRIMARY KEY,
    ticket_uid    uuid UNIQUE NOT NULL,
    username      VARCHAR(80) NOT NULL,
    flight_number VARCHAR(20) NOT NULL,
    price         INT         NOT NULL,
    status        VARCHAR(20) NOT NULL
        CHECK (status IN ('PAID', 'CANCELED'))
);

\c flights

CREATE TABLE airport
(
    id      SERIAL PRIMARY KEY,
    name    VARCHAR(255),
    city    VARCHAR(255),
    country VARCHAR(255)
);

CREATE TABLE flight
(
    id              SERIAL PRIMARY KEY,
    flight_number   VARCHAR(20)              NOT NULL,
    datetime        TIMESTAMP WITH TIME ZONE NOT NULL,
    from_airport_id INT REFERENCES airport (id),
    to_airport_id   INT REFERENCES airport (id),
    price           INT                      NOT NULL
);


\c privileges
CREATE TABLE privilege
(
    id       SERIAL PRIMARY KEY,
    username VARCHAR(80) NOT NULL UNIQUE,
    status   VARCHAR(80) NOT NULL DEFAULT 'BRONZE'
        CHECK (status IN ('BRONZE', 'SILVER', 'GOLD')),
    balance  INT
);

CREATE TABLE privilege_history
(
    id             SERIAL PRIMARY KEY,
    privilege_id   INT REFERENCES privilege (id),
    ticket_uid     uuid        NOT NULL,
    datetime       TIMESTAMP   NOT NULL,
    balance_diff   INT         NOT NULL,
    operation_type VARCHAR(20) NOT NULL
        CHECK (operation_type IN ('FILL_IN_BALANCE', 'DEBIT_THE_ACCOUNT'))
);

\c persons

create table if not exists Person (
    login varchar(80) primary key,
    name varchar(50),
    email varchar(50),
    mobilePhone varchar(50),
    lastName varchar(50),
    password varchar(50),
    role varchar(50)
    );

create table if not exists Client (
    client_id serial primary key,
    client_secret varchar(50)
);

create table if not exists AuthCode (
    code serial primary key,
    client_id   INT REFERENCES Client (client_id),
    login varchar(50),
    scope_string varchar(200)
);