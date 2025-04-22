#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>
#include <queue>
#include <set>

// Structure représentant une connexion vers une station voisine
struct neighb {
    std::string ligne;     // Ligne de transport utilisée
    std::string neighbor;  // ID de la station voisine
};

// Structure représentant une station
struct Station {
    std::string id;        // Identifiant unique de la station
    std::string name;      // Nom de la station
    std::vector<neighb> neighbors;  // Liste des connexions vers d'autres stations
};

class GraphProcessor {
public:
    // Trouver un chemin entre deux stations
    static std::vector<std::pair<std::string, std::string>> findPath(
        const std::unordered_map<std::string, Station>& graph,
        const std::string& start,
        const std::string& end
    );
    
    // Créer une copie du graphe en évitant certaines stations
    static std::unordered_map<std::string, Station> createModifiedGraph(
        const std::unordered_map<std::string, Station>& originalGraph,
        const std::set<std::string>& stationsToAvoid
    );
    
    // Trouver un chemin avec des stations intermédiaires obligatoires
    static std::vector<std::pair<std::string, std::string>> findPathWithIntermediates(
        const std::unordered_map<std::string, Station>& graph,
        const std::string& start,
        const std::string& end,
        const std::vector<std::string>& intermediates
    );
};

#endif // GRAPH_H
