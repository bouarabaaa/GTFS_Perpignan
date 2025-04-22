#include "../include/database-handler.h"
#include <iostream>

DatabaseHandler::DatabaseHandler(const std::string& dbPath) : dbPath(dbPath), db(nullptr) {
    // Constructeur
}

DatabaseHandler::~DatabaseHandler() {
    closeConnection();
}

bool DatabaseHandler::openConnection() {
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Erreur lors de l'ouverture de la base de données: " 
                  << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    return true;
}

void DatabaseHandler::closeConnection() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

std::unordered_map<std::string, Station> DatabaseHandler::loadStations() {
    std::unordered_map<std::string, Station> stations;
    
    if (!openConnection()) {
        return stations;
    }
    
    // Charger les stations de base
    loadStationsBasic(stations);
    
    // Charger les connexions entre stations
    loadConnections(stations);
    
    closeConnection();
    return stations;
}

void DatabaseHandler::loadStationsBasic(std::unordered_map<std::string, Station>& stations) {
    const char* sql = "SELECT stop_id, stop_name FROM stops;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            stations[id] = {id, name, {}};
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Erreur SQL lors du chargement des stations: " 
                  << sqlite3_errmsg(db) << std::endl;
    }
}

void DatabaseHandler::loadConnections(std::unordered_map<std::string, Station>& stations) {
    const char* sqlEdges = R"(
        SELECT DISTINCT 
            t1.stop_id AS from_stop, 
            t2.stop_id AS to_stop,
            trips.route_id
        FROM stop_times t1
        JOIN stop_times t2 ON t1.trip_id = t2.trip_id
        JOIN trips ON t1.trip_id = trips.trip_id
        WHERE t1.stop_sequence + 1 = t2.stop_sequence;
    )";
    
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sqlEdges, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string from = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::string to = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::string ligne = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    
            if (stations.count(from) && stations.count(to)) {
                neighb aa;
                aa.neighbor = to;
                aa.ligne = ligne;
                stations[from].neighbors.push_back(aa);
            }
        }
        sqlite3_finalize(stmt);
    } else {
        std::cerr << "Erreur SQL lors du chargement des connexions: " 
                  << sqlite3_errmsg(db) << std::endl;
    }
}
