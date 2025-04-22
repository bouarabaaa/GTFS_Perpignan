#include "../include/graph.h"
#include <algorithm>

std::vector<std::pair<std::string, std::string>> GraphProcessor::findPath(
    const std::unordered_map<std::string, Station>& graph,
    const std::string& start,
    const std::string& end
) {
    std::unordered_map<std::string, std::string> prev;
    std::queue<std::string> q;
    std::set<std::string> visited;

    q.push(start);
    visited.insert(start);

    while (!q.empty()) {
        std::string current = q.front(); q.pop();
        if (current == end) break;

        if (graph.count(current)) {
            for (const auto& neighborObj : graph.at(current).neighbors) {
                const std::string& neighbor = neighborObj.neighbor;
                if (!visited.count(neighbor) && graph.count(neighbor)) {
                    visited.insert(neighbor);
                    prev[neighbor] = current;
                    q.push(neighbor);
                }
            }
        }
    }

    std::vector<std::pair<std::string, std::string>> path;
    std::string at = end;

    while (!at.empty() && prev.count(at)) {
        std::string from = prev[at];
        std::string line_used;

        // Chercher la ligne entre `from` et `at`
        for (const auto& n : graph.at(from).neighbors) {
            if (n.neighbor == at) {
                line_used = n.ligne;
                break;
            }
        }

        path.insert(path.begin(), {at, line_used});
        at = from;
    }

    // Ajouter la station de départ (sans ligne)
    if (at == start) {
        path.insert(path.begin(), {start, ""});
    }

    return path;
}

std::unordered_map<std::string, Station> GraphProcessor::createModifiedGraph(
    const std::unordered_map<std::string, Station>& originalGraph,
    const std::set<std::string>& stationsToAvoid
) {
    auto modifiedGraph = originalGraph;
    
    // Supprimer les stations à éviter
    for (const auto& station : stationsToAvoid) {
        modifiedGraph.erase(station);
    }
    
    // Supprimer les références aux stations évitées dans les listes de voisins
    for (auto& [id, station] : modifiedGraph) {
        auto& neighbors = station.neighbors;
        neighbors.erase(
            std::remove_if(neighbors.begin(), neighbors.end(),
                        [&stationsToAvoid](const neighb& n) { 
                            return stationsToAvoid.count(n.neighbor) > 0; 
                        }),
            neighbors.end()
        );
    }
    
    return modifiedGraph;
}

std::vector<std::pair<std::string, std::string>> GraphProcessor::findPathWithIntermediates(
    const std::unordered_map<std::string, Station>& graph,
    const std::string& start,
    const std::string& end,
    const std::vector<std::string>& intermediates
) {
    std::vector<std::pair<std::string, std::string>> fullPath;
    std::string current = start;
    
    // Créer une copie du vecteur avec la destination finale
    std::vector<std::string> waypoints = intermediates;
    waypoints.push_back(end);
    
    for (const auto& target : waypoints) {
        auto partialPath = findPath(graph, current, target);
        
        if (partialPath.empty()) {
            // Aucun chemin trouvé pour cette section
            return {};
        }
        
        // Éviter de dupliquer la station de départ si ce n'est pas la première section
        if (!fullPath.empty()) {
            partialPath.erase(partialPath.begin());
        }
        
        // Ajouter cette section au chemin complet
        fullPath.insert(fullPath.end(), partialPath.begin(), partialPath.end());
        
        // La station cible devient le nouveau point de départ pour la prochaine section
        current = target;
    }
    
    return fullPath;
}
