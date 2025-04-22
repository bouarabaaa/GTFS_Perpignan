#include "../Crow/include/crow.h"
#include <fstream>
#include <sstream>
#include "../include/database-handler.h"
#include "../include/graph.h"

int main() {
    crow::SimpleApp app;
    
    // Charger les données depuis la base de données
    DatabaseHandler dbHandler("database/tras.db");
    auto stations = dbHandler.loadStations();

    // Route pour la page d'accueil
    CROW_ROUTE(app, "/")([](){
        crow::response res;
        std::ifstream file("./public/index.html");
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            res.body = buffer.str();
            res.code = 200;
        } else {
            res.code = 404;
            res.body = "Not Found";
        }
        return res;
    });

    // Route pour obtenir la liste des stations
    CROW_ROUTE(app, "/stations")
    ([&stations]() {
        crow::json::wvalue result;
        int i = 0;
        for (const auto& [id, station] : stations) {
            result["stations"][i]["id"] = id;
            result["stations"][i]["name"] = station.name;
            i++;
        }
        return result;
    });

    // Route pour servir les fichiers statiques
    CROW_ROUTE(app, "/public/<string>")([](const crow::request& req, std::string filename){
        crow::response res;
        std::string filepath = "./public/" + filename;
        std::ifstream file(filepath, std::ios::in | std::ios::binary);
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            res.body = buffer.str();
            res.code = 200;
            
            // Définir le type MIME approprié
            std::string extension = filename.substr(filename.find_last_of(".") + 1);
            if (extension == "css")
                res.set_header("Content-Type", "text/css");
            else if (extension == "js")
                res.set_header("Content-Type", "text/javascript");
            else if (extension == "png")
                res.set_header("Content-Type", "image/png");
            
        } else {
            res.code = 404;
            res.body = "Not Found";
        }
        return res;
    });

    // Route pour servir les images
    CROW_ROUTE(app, "/public/images/<string>")([](const crow::request& req, std::string filename){
        crow::response res;
        std::string filepath = "./public/images/" + filename;
        std::ifstream file(filepath, std::ios::in | std::ios::binary);
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            res.body = buffer.str();
            res.code = 200;
            
            // Définir le type MIME approprié
            std::string extension = filename.substr(filename.find_last_of(".") + 1);
            if (extension == "png")
                res.set_header("Content-Type", "image/png");
            else if (extension == "jpg" || extension == "jpeg")
                res.set_header("Content-Type", "image/jpeg");
            
        } else {
            res.code = 404;
            res.body = "Not Found";
        }
        return res;
    });  

    // Route principale pour trouver des itinéraires
    CROW_ROUTE(app, "/route")
    ([&stations](const crow::request& req) {
        auto params = crow::query_string(req.url_params);
        std::string start = params.get("start") ? params.get("start") : "";
        std::string end = params.get("end") ? params.get("end") : "";
        std::string eviter_param = params.get("eviter") ? params.get("eviter") : "";
        std::string inter_param = params.get("intermediaires") ? params.get("intermediaires") : "";

        crow::json::wvalue res;
        if (start.empty() || end.empty()) {
            res["error"] = "Start or end station is missing.";
            return crow::response(400, res.dump());
        }

        // Stations à éviter
        std::set<std::string> toAvoid;
        std::stringstream ss(eviter_param);
        std::string token;
        while (std::getline(ss, token, ',')) {
            if (!token.empty()) {
                toAvoid.insert(token);
            }
        }

        // Stations intermédiaires
        std::vector<std::string> intermediates;
        std::stringstream iss(inter_param);
        while (std::getline(iss, token, ',')) {
            if (!token.empty()) {
                intermediates.push_back(token);
            }
        }

        // Créer une version modifiée du graphe sans les stations à éviter
        auto modifiedGraph = GraphProcessor::createModifiedGraph(stations, toAvoid);
        
        // Trouver le chemin avec les stations intermédiaires
        auto fullPath = GraphProcessor::findPathWithIntermediates(
            modifiedGraph, start, end, intermediates
        );

        if (fullPath.empty()) {
            res["error"] = "Impossible de trouver un chemin respectant les contraintes.";
            return crow::response(404, res.dump());
        }

        // Convertir le chemin en JSON
        crow::json::wvalue result;
        int i = 0;
        for (const auto& [station, ligne] : fullPath) {
            if (stations.count(station)) {
                result["path"][i]["station"] = station;
                result["path"][i]["stationName"] = stations[station].name;
                result["path"][i]["ligne"] = ligne;
                ++i;
            }
        }
        
        return crow::response(200, result.dump());
    });

    // Démarrer le serveur
    app.port(8080).multithreaded().run();
    
    return 0;
}
