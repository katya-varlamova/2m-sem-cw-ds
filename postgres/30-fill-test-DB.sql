\c tickets
INSERT INTO ticket (ticket_uid ,username, flight_number, price, status) VALUES
('049161bb-badd-4fa8-9d90-87c9a82b0668', 'Kate', 'AFL031', 1500, 'PAID');

\c flights
INSERT INTO airport (name, city, country) VALUES
('Шереметьево', 'Москва', 'Россия');
INSERT INTO airport (name, city, country) VALUES
('Пулково', 'Санкт-Петербург', 'Россия');
INSERT INTO flight (flight_number, datetime, from_airport_id, to_airport_id, price) VALUES
('AFL031', '2021-10-08 20:00', 2, 1, 1500);


\c privileges
INSERT INTO privilege (username, status, balance) VALUES
('Kate', 'GOLD', 1500);
INSERT INTO privilege_history (privilege_id, ticket_uid, datetime, balance_diff, operation_type) VALUES
(1,
 '049161bb-badd-4fa8-9d90-87c9a82b0668',
 '2021-10-08T19:59:19Z',
 1500,
 'FILL_IN_BALANCE');

 \c persons
INSERT INTO Person (login, name, email, mobilePhone, lastName, password, role) VALUES
('Kate', 'katya', 'katy@mail.ru', '+7 (990) 100-73-02', 'varlamova', 'pass', 'user'),
('admin', 'katya', 'katy_admin@mail.ru', '+7 (990) 100-73-02', 'varlamova', 'admin', 'admin');

INSERT INTO Client (client_secret) VALUES 
('secret');


