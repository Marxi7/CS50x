-- Keep a log of any SQL queries you execute as you solve the mystery.

--Starting by finding the crime report description that matches with with the date (28.07.2021) and the place( Humphrey Street) of the crime..
SELECT description
FROM crime_scene_reports
WHERE month = "7"
AND day = "28"
AND year = "2021"
AND street = "Humphrey Street";

-- Looking through the transcripts from the interviews conducted on the date of the theft mentioning the word Bakery as we know that 3 witnessed have been there and all mentionned the bakery!
SELECT transcript
FROM interviews
WHERE month = "7"
AND day = "28"
AND year = "2021"
AND transcript LIKE '%Bakery%';

-- the #1 Witness tells us that we might be able to find the theft by looking as which cars left the bakering within 10' after the crim - between 10:15am and 10.25am.
-- the #2 witness tells us that he recognized the theft and that he saw him/her at the ATM by Leggett Street before the crime withdrawing money (so before 10:15am)
-- the #3 witness tells us that
    -- 1) The thief called someone for less than 1' and that he was telling the person on the phone that he was planning to flight out of fiftyville the next day with the earliest flight
    -- 2) The thief asked the person (his accomplice) on the phone to purchase the flight ticket.

-- Based on these informations, we can proceed steps by steps:


-- Looking for the plate from a car leaving the bakery between 10:15am and 10:25am. Then we find to find the proprietary of that car.
SELECT people.name FROM people JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
WHERE bakery_security_logs.month = "7"
AND bakery_security_logs.day = "28"
AND bakery_security_logs.year = "2021"
AND bakery_security_logs.hour = 10
AND bakery_security_logs.minute >= 15
AND bakery_security_logs.minute <= 25;
-- We got these names out of our query; Vannessa, Bruce, Barry, Luca, Sofia, Iman, Diana, Kelsey - all these peoples' plates match with the cars' plate that left the bakery between 10:15 and 10:25

-- As we know that the thief withdraw some money before 10:15.
-- We we'll see if we can match a name from the people who withdraw in that timeframe in the location mentioned and the people who left the bakery between 10:15 and 10:25
SELECT people.name FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE atm_transactions.transaction_type = "withdraw"
AND atm_transactions.month = "7"
AND atm_transactions.day = "28"
AND atm_transactions.year = "2021"
AND atm_transactions.atm_location = "Leggett Street"
AND people.name IN
(SELECT people.name
FROM people JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
WHERE bakery_security_logs.month = "7"
AND bakery_security_logs.day = "28"
AND bakery_security_logs.year = "2021"
AND bakery_security_logs.hour = 10
AND bakery_security_logs.minute >= 15
AND bakery_security_logs.minute <= 25);
-- These are the name of the people who went to withdraw that day at the ATM by Legget street and left, with their car, the bakery between 10:15am and 10:25am ; Bruce, Diana, Iman, Luca


-- Now we need to find the caller and the receiver of a phone call that lasted less than 1'  on 28.07.2021 when the theft was leaving the bakery, so from 10:15am and presumably before 10:25am.
-- we will start with the caller in order to then compare that list of people to the list of people who went to withdraw earlier that day that also have their plate matching the cars leaving the bakery between 10:15 and 10:25am
-- This will propably get us the  thief. We'll might also compare that, if we get more than one result, to the flight ticket leaving the earliest the next day.
SELECT people.name FROM people JOIN phone_calls ON people.phone_number = phone_calls.caller
WHERE phone_calls.month = "7"
AND phone_calls.day = "28"
AND phone_calls.year = "2021"
AND phone_calls.duration <=60
AND people.name IN
(SELECT people.name FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE atm_transactions.transaction_type = "withdraw"
AND atm_transactions.month = "7"
AND atm_transactions.day = "28"
AND atm_transactions.year = "2021"
AND atm_transactions.atm_location = "Leggett Street"
AND people.name IN
(SELECT people.name
FROM people JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
WHERE bakery_security_logs.month = "7"
AND bakery_security_logs.day = "28"
AND bakery_security_logs.year = "2021"
AND bakery_security_logs.hour = 10
AND bakery_security_logs.minute >= 15
AND bakery_security_logs.minute <= 25));
-- This query gets us back to Bruce and Diana. These are out last 2 suspects of being the thief.

-- We will now match this last query with a query regarding the earliest flight on 29.07.2021, this will get us the thief and the location he escaped to!
SELECT people.name, airports.city FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
JOIN flights ON passengers.flight_id = flights.id
JOIN airports ON flights.destination_airport_id = airports.id
WHERE flights.year = "2021"
AND flights.month = "7"
AND flights.day = "29"
AND people.name IN
(SELECT people.name FROM people JOIN phone_calls ON people.phone_number = phone_calls.caller
WHERE phone_calls.month = "7"
AND phone_calls.day = "28"
AND phone_calls.year = "2021"
AND phone_calls.duration <=60
AND people.name IN
(SELECT people.name FROM people JOIN bank_accounts ON people.id = bank_accounts.person_id JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE atm_transactions.transaction_type = "withdraw"
AND atm_transactions.month = "7"
AND atm_transactions.day = "28"
AND atm_transactions.year = "2021"
AND atm_transactions.atm_location = "Leggett Street"
AND people.name IN
(SELECT people.name
FROM people JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
WHERE bakery_security_logs.month = "7"
AND bakery_security_logs.day = "28"
AND bakery_security_logs.year = "2021"
AND bakery_security_logs.hour = 10
AND bakery_security_logs.minute >= 15
AND bakery_security_logs.minute <= 25)));
-- This query gets us back Diana who go in Boston and Bruce who's going to New York on the 29th July 2021. But we still don't know which flight leaved the earliest.

-- We should now check what the first flight' destination is in order to determine who the thief is. The flight should be on the 29.07.2021.
SELECT airports.city, flights.hour, flights.minute, flights.destination_airport_id FROM flights
JOIN airports ON flights.origin_airport_id = airports.id
WHERE airports.city = "Fiftyville"
AND flights.year = "2021"
AND flights.month = "7"
AND flights.day = "29" ORDER BY flights.hour, flights.minute;
-- This query told us that the first flight from Fftyville is at 8:20am and hie destination_id is 4. Now we should look where this flight is going with that id.

-- Finding the name of the city where the first flight  with the id 4 is landing.
SELECT airports.city FROM airports
JOIN flights ON airports.id = flights.destination_airport_id
WHERE flights.hour = "8"
AND flights.minute = "20"
AND flights.year = "2021"
AND flights.month = "7"
AND flights.day = "29"
AND flights.destination_airport_id = "4";
-- This flight is going to New York City! We know that Bruce is going to New York city and not Diana, so Bruce is the Thief.

--Now we need to find the receiver of the call from Bruce. The date was the 28.07.2021 and the duration of the call was less than 1' (60sec)
SELECT phone_number FROM people
WHERE name = "Bruce";
-- this returns us the number of Bruce : (367) 555-5533

-- Now we check what the name of the phone call receiver is by checking who was called by the Caller (Bruce) on the 28.07.2021 for less than 1 minute.
SELECT people.name FROM people
JOIN phone_calls ON people.phone_number = phone_calls.receiver
WHERE phone_calls.caller = "(367) 555-5533" --using the number of Bruce
AND phone_calls.month = "7"
AND phone_calls.day = "28"
AND phone_calls.year = "2021"
AND phone_calls.duration <=60;
-- This returns Robin! The accomplice is Robin.

-- To recap:
-- The thief is Bruce and he escaped to New York City the next day of the theft.
-- His accomplice is Robin.







