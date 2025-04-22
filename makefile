CXX = g++
CXXFLAGS = -std=gnu++17 -Wall -Wextra -I./Crow/include -Iinclude
LDFLAGS = -lsqlite3 -pthread

# Dossiers du projet
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj

# Fichiers source et objets
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SOURCES))
EXECUTABLE = transport_api

# Règle par défaut
all: $(EXECUTABLE)

# Création des dossiers nécessaires
# directories:
# 	mkdir -p $(OBJ_DIR)

# Règle de construction de l'exécutable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)

# Règle de compilation des fichiers source
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Règle pour compiler rapidement avec une seule commande (comme votre exemple)
quick:
	$(CXX) -std=gnu++17 -I./Crow/include $(SRC_DIR)/main.cpp -o $(EXECUTABLE) -pthread -lsqlite3

# Règle de nettoyage
clean:
	rm -rf $(OBJ_DIR)/*.o $(EXECUTABLE)
	clear

# Dépendances spécifiques
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp $(INCLUDE_DIR)/database-handler.h $(INCLUDE_DIR)/graph.h
$(OBJ_DIR)/database-handler.o: $(SRC_DIR)/database-handler.cpp $(INCLUDE_DIR)/database-handler.h $(INCLUDE_DIR)/graph.h
$(OBJ_DIR)/graph.o: $(SRC_DIR)/graph.cpp $(INCLUDE_DIR)/graph.h

.PHONY: all clean directories quick