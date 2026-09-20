CXX = clang++
CXXFLAGS = -Wall -std=c++17

# Liste des exécutables à produire
all: prog

# L'édition de liens : assemble les .o en binaire
prog: Point.o main.o
	$(CXX) Point.o main.o -o prog

# Règle générique : fabrique n'importe quel .o à partir de son .cpp
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Nettoyage des binaires
clean:
	rm -f *.o prog