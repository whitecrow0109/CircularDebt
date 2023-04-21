/* 
 * Tests pour la classe générique Digraph<T> du TP3.
 * TP3 -- Graphes orientés
 * UQAM / Département d'informatique
 * INF3105 - Structures de données et algorithmes (Hiver 2023)
 */
 
#include "digraph.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <vector>
#include <set>
#include <cmath>

class Chrono {
	public:
		Chrono() {}
		~Chrono() {}
		void debut() {
			start = std::chrono::steady_clock::now();
		}
		void fin() {
			end = std::chrono::steady_clock::now();
			duree = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		}
		float duree;
	private:
		std::chrono::steady_clock::time_point start, end;
};

int main(int argc, char * argv[]) {
	double EPSILON = 0.5;
  int erreur = 0;
  Digraph<int> complete;
  std::string line;
	std::ifstream input_graph(argv[1]);
  while(std::getline(input_graph, line) && !line.empty()) {
    std::stringstream ss(line);
 		int u, v;
    double emprunt;
    ss >> u >> v >> emprunt;
    complete.inserer(u, v, emprunt);
  }
	std::vector<double> dettes, prets;
	std::ifstream input_dp(argv[2]);
  while(std::getline(input_dp, line) && !line.empty()) {
    std::stringstream ss(line);
 		double d, p;
    		ss >> d >> p;
		dettes.push_back(d);
		prets.push_back(p);
  }
	Chrono timer;
	timer.debut();
	if(complete.sommets() != 100 || complete.arcs() != 9900)
		erreur++;
	for(int i = 0; i < 100; i += 20) {
		complete.reduire(i);	
	}
	for(int i = 0; i < 100; i++)
		if(std::fabs(complete.dette(i) - dettes[i]) > EPSILON)
			erreur++;
			
	for(int i = 0; i < 100; i++)
		if(std::fabs(complete.pret(i) - prets[i]) > EPSILON)
			erreur++;
	timer.fin();
	if(erreur != 0 || timer.duree > 25000000) {
		std::cerr << "ECHEC - PERFORMANCES" << std::endl;
		return erreur;
	}
	std::cout << "\t==> OK" << std::endl;
  return 0;
}
