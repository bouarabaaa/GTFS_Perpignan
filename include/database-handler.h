#ifndef DATABASE_HANDLER_H
#define DATABASE_HANDLER_H

#include <sqlite3.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "graph.h"

class DatabaseHandler {
public:
    DatabaseHandler(const std::string& dbPath);
    ~DatabaseHandler();
    
    // Charger toutes les stations depuis la base de données
    std::unordered_map<std::string, Station> loadStations();

private:
    std::string dbPath;
    sqlite3* db;
    
    // Ouvrir la connexion à la base de données
    bool openConnection();
    
    // Fermer la connexion à la base de données
    void closeConnection();
    
    // Charger les stations de base
    void loadStationsBasic(std::unordered_map<std::string, Station>& stations);
    
    // Charger les connexions entre stations
    void loadConnections(std::unordered_map<std::string, Station>& stations);
};

#endif // DATABASE_HANDLER_H
