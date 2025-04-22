-- DROP database IF EXISTS gtfs_mont;
-- CREATE database IF NOT EXISTS gtfs_mont;

-- USE gtfs_mont;

DROP TABLE IF EXISTS agency;
CREATE TABLE agency (
  agency_id VARCHAR(50) PRIMARY KEY NOT NULL,
  agency_name VARCHAR(255),
  agency_url VARCHAR(255),
  agency_timezone VARCHAR(50),
  agency_lang VARCHAR(50),
  agency_phone VARCHAR(50),
  agency_fare_url VARCHAR(50),
  agency_email VARCHAR(50)
);

DROP TABLE IF EXISTS routes;
CREATE TABLE routes (
  route_id VARCHAR(50) PRIMARY KEY NOT NULL,
  agency_id VARCHAR(50),
  route_short_name VARCHAR(50),
  route_long_name VARCHAR(255),
  route_type TINYINT,
  route_color VARCHAR(50),
  route_text_color VARCHAR(50),
  route_url VARCHAR(50),
  FOREIGN KEY (agency_id) REFERENCES agency(agency_id)
);

DROP TABLE IF EXISTS calendar;
CREATE TABLE calendar (
    service_id VARCHAR(50) NOT NULL,
    monday TINYINT,
    tuesday TINYINT,
    wednesday TINYINT,
    thursday TINYINT,
    friday TINYINT,
    saturday TINYINT,
    sunday TINYINT,
    start_date INT,
    end_date INT
);

DROP TABLE IF EXISTS calendar_dates;
CREATE TABLE calendar_dates (
    service_id VARCHAR(50) NOT NULL,
    `date` DATE NOT NULL,
    exception_type TINYINT,
    CONSTRAINT Pk_cal PRIMARY KEY (service_id,`date`)
    FOREIGN KEY (service_id) REFERENCES calendar(service_id)
);


DROP TABLE IF EXISTS trips;
CREATE TABLE trips (
  route_id VARCHAR(50) NOT NULL,
  service_id VARCHAR(50) NOT NULL,
  trip_id VARCHAR(50) PRIMARY KEY,
  trip_headsign VARCHAR(255),
  direction_id TINYINT,
  block_id VARCHAR(50),
  wheelchair_accessible TINYINT,
  bikes_allowed TINYINT,
  FOREIGN KEY (route_id) REFERENCES routes(route_id),
  FOREIGN KEY (service_id) REFERENCES calendar_dates(service_id)

);


DROP TABLE IF EXISTS stops;
CREATE TABLE stops (
  stop_id VARCHAR(50) PRIMARY KEY,
  stop_code VARCHAR(255),
  stop_name VARCHAR(255) NOT NULL,
  stop_lat DECIMAL(9,6),
  stop_lon DECIMAL(9,6),
  location_type INT(2),
  parent_station VARCHAR(50) REFERENCES stops(stop_id),
  wheelchair_boarding tinyint
);

DROP TABLE IF EXISTS stop_times;
CREATE TABLE stop_times (
  trip_id VARCHAR(50),
  arrival_time TIME,
  departure_time TIME,
  stop_id VARCHAR(50),
  stop_sequence INT(11),
  pickup_type tinyint,
  drop_off_type tinyint,
  stop_headsign VARCHAR(255),
  CONSTRAINT Pk_st PRIMARY KEY (trip_id,stop_sequence),
  FOREIGN KEY (trip_id) REFERENCES trips(trip_id),
  FOREIGN KEY (stop_id) REFERENCES stops(stop_id)
);

DROP TABLE IF EXISTS alltrips;
CREATE TABLE alltrips (
    trip_id VARCHAR(50),
    stop_id VARCHAR(50),
    stop_sequence INT(11),
    FOREIGN KEY (trip_id) REFERENCES trips(trip_id),
    FOREIGN KEY (stop_id) REFERENCES stops(stop_id)
);

DROP TABLE IF EXISTS feed_info;
CREATE TABLE feed_info (
    feed_publisher_name TEXT,
    feed_publisher_url TEXT,
    feed_lang TEXT,
    feed_start_date TEXT,
    feed_end_date TEXT,
    feed_version TEXT,
    feed_contact_email TEXT,
    feed_contact_url TEXT
);

-- DROP TABLE IF EXISTS transfer;

-- CREATE TABLE transfer (
--   from_stop_id VARCHAR(50) NOT NULL,
--   to_stop_id VARCHAR(50) NOT NULL,
--   transfer_type TINYINT,  -- 0: No transfer, 1: Minimum transfer time, 2: Other transfer types
--   PRIMARY KEY (from_stop_id, to_stop_id),
--   FOREIGN KEY (from_stop_id) REFERENCES stops(stop_id),
--   FOREIGN KEY (to_stop_id) REFERENCES stops(stop_id)
-- );

DROP TABLE IF EXISTS cost;

-- CREATE TABLE cost (
--   from_stop_id VARCHAR(50) NOT NULL,
--   to_stop_id VARCHAR(50) NOT NULL,
--   travel_cost int,
--   PRIMARY KEY (from_stop_id, to_stop_id),
--   FOREIGN KEY (from_stop_id) REFERENCES stops(stop_id),
--   FOREIGN KEY (to_stop_id) REFERENCES stops(stop_id)
-- );


-- LOAD DATA LOCAL INFILE 'agency.txt' INTO TABLE agency FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' IGNORE 1 LINES;
-- LOAD DATA LOCAL INFILE 'routes.txt' INTO TABLE routes FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' IGNORE 1 LINES;
-- LOAD DATA LOCAL INFILE 'calendar.txt' INTO TABLE calendar FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' IGNORE 1 LINES;
-- LOAD DATA LOCAL INFILE 'calendar_dates.txt' INTO TABLE calendar_dates FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' IGNORE 1 LINES;
-- LOAD DATA LOCAL INFILE 'trips.txt' INTO TABLE trips FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' IGNORE 1 LINES;
-- LOAD DATA LOCAL INFILE 'stops.txt' INTO TABLE stops FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' IGNORE 1 LINES;
-- LOAD DATA LOCAL INFILE 'stop_times.txt' INTO TABLE stop_times FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' IGNORE 1 LINES;
-- LOAD DATA LOCAL INFILE 'transfers.txt' INTO TABLE transfer FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' IGNORE 1 LINES;
-- LOAD DATA LOCAL INFILE 'cost.txt' INTO TABLE cost FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"' IGNORE 1 LINES;
