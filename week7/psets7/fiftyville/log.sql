-- Keep a log of any SQL queries you execute as you solve the mystery.
-- View schema
.schema
-- Check the description of the crime
SELECT description
FROM crime_scene_reports
WHERE month = 7
AND day = 28
AND street = 'Humphrey Street';
-- Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery. Littering took place at 16:36. No known witnesses

-- Search interviews for bakery
SELECT *
FROM interviews
WHERE transcript LIKE '%bakery%';

-- interview 161 Ruth Within 10 mins of the theft. the suspect drove into and out of the bakery carpark
-- check security logs of the day, since the crime took place at 10:15, we can narrow the search to 10am
SELECT *
FROM bakery_security_logs
WHERE month = 7
AND day = 28
AND hour = 10;
-- known suspect vehicles are R3G7486, 13FNH73, 5P2BI95, 94KL13X, 6P58WS2, 4328GD8, G412CB7, L93JTIZ, 322W7JE, 0NTHK55
SELECT *
FROM people
WHERE license_plate IN ('R3G7486', '13FNH73', '5P2BI95', '94KL13X', '6P58WS2', '4328GD8', 'G412CB7', 'L93JTIZ', '322W7JE', '0NTHK55');
-- acquired suspect list from security logs

-- interview 162 Eugene The thief withdrew money from the ATM on Leggett Street in the morning
SELECT *
FROM atm_transactions
WHERE month = 7
AND day = 28
AND transaction_type = 'withdraw'
AND atm_location = 'Leggett Street';
-- the list of suspect account numbers are 28500762, 28296815, 76054385, 49610011, 16153065, 25506511, 81061156, 26013199
SELECT *
FROM bank_accounts
WHERE account_number IN ('28500762', '28296815', '76054385', '49610011', '16153065', '25506511', '81061156', '26013199');
-- acquired suspect list from atm withdrawls

-- interview 163 Raymond The thief plans to take the earlist flight out the next day(29/7/21) the accomplice likely booked the tickets. Made a phone call around the time of the thief for less than 1 minute.
SELECT caller AS phone_number
FROM phone_calls
WHERE month = 7
AND day = 28
AND duration < 80
ORDER BY duration ASC;

SELECT receiver AS phone_number
FROM phone_calls
WHERE month = 7
AND day = 28
AND duration < 80
ORDER BY duration ASC;
-- got list of suspected caller and receivers from calls lasting less than 1 minute
-- acquired suspect list number from calls

-- flight out lead
SELECT id
FROM airports
WHERE city = 'Fiftyville';
-- id of Fiftyville airport is 8
SELECT *
FROM flights
WHERE origin_airport_id = 8
AND month = 7
AND day = 29
ORDER BY hour ASC;
-- the id of the 1st flight our is 36
SELECT *
FROM passengers
WHERE flight_id = 36;
-- the id of the destination airport is 4
SELECT * FROM airports WHERE id = 4;
-- the suspect escaoed to LGA New York City

--combining all searches
SELECT *
FROM people
WHERE passport_number IN (
    SELECT passport_number
    FROM passengers
    WHERE flight_id = 36
) AND id IN (
    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN ('28500762', '28296815', '76054385', '49610011', '16153065', '25506511', '81061156', '26013199')
) AND license_plate IN ('R3G7486', '13FNH73', '5P2BI95', '94KL13X', '6P58WS2', '4328GD8', 'G412CB7', 'L93JTIZ', '322W7JE', '0NTHK55')
AND phone_number IN (
    SELECT caller AS phone_number
    FROM phone_calls
    WHERE month = 7
    AND day = 28
    AND duration < 80
);
-- Bruce is the prime suspect

--search for teh accoplice
SELECT *
FROM phone_calls
WHERE month = 7
AND day = 28
AND duration < 80
AND caller = '(367) 555-5533';
-- (375) 555-8161 received the call from Bruce
SELECT *
FROM people
WHERE phone_number = '(375) 555-8161';
-- the accomplice is Robin