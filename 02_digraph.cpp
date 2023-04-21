/* 
 * Tests pour la classe générique Digraph<T> du TP3.
 * TP3 -- Graphes orientés
 * UQAM / Département d'informatique
 * INF3105 - Structures de données et algorithmes (Hiver 2023)
 */
 
#include "digraph.h"
#include <iostream>

int main() {
  int erreur = 0, som = 15, arr = 34;
  Digraph<std::string> graphe;
	std::string vertices [] = {"BIC","BMO","BNP","ATB","BEA","AXA","NBG", 
													"BOC","RBC","BOA","LCL","BNS","CIC","BNC", 
													"ING"};
	std::string arcs [] = {"NBG","LCL","NBG","AXA","LCL","RBC","LCL","BOA", 
												"LCL","BNS","BNS","BOA","BNS","BIC","BNS","CIC", 
												"CIC","LCL","CIC","ING","ATB","CIC","ATB","BIC", 
												"ATB","BNP","ING","ATB","ING","BNC","BNC","NBG", 
												"BNC","BEA","BOC","NBG","BOC","AXA","BNP","BMO", 
												"BNP","BEA","BIC","BMO","BIC","BNP","BMO","BOA", 
												"BMO","NBG","BMO","AXA","BOA","RBC","BOA","NBG",
												"RBC","NBG","BEA","AXA","BEA","BOC","BEA","ING", 
												"BEA","ATB","AXA","BNP"};
	double emprunts [] = {5320,1500,4420,3900,8010,6370,5320,2313,5500,4200,
												7622,1700,3050,1700,1302,4710,4500,1740,2200,4000,
												5000,3012,3220,2410,3240,3440,5100,7400,3800,1100,
												3900,9390,2100,9200};
	for(int i = 0, j = 0; i < arr && j < 2 * arr; i++, j += 2)
		graphe.inserer(arcs[j], arcs[j + 1], emprunts[i]);
  if(graphe.sommets() != som) {
    std::cerr << "ECHEC - I" << std::endl;
    erreur++;
  }
  if(graphe.arcs() != arr) {
    std::cerr << "ECHEC - II" << std::endl;
    erreur++;
  }
	std::string pas_sommets [] = {"NBGX","LCLX","NBGX","AXAX","LCLX","RBCX"};
	for(int i = 0; i < 6; i++)
		graphe.supprimer(pas_sommets[i]);
	if(graphe.sommets() != som) {
    std::cerr << "ECHEC - III" << std::endl;
    erreur++;
  }
	for(int i = 0; i < 2 * arr; i += 2)
		graphe.supprimer(arcs[i + 1], arcs[i]);
	if(graphe.arcs() != arr) {
    std::cerr << "ECHEC - IV" << std::endl;
    erreur++;
  }
	std::string pas_arcs [] = {"BOC","BNC","AXA","BNC","RBC","BMO","BOA","BNP", 
														"BOA","BIC","NBG","BNP","BNP","CIC"};
	for(int i = 0; i < 14; i += 2)
		graphe.supprimer(pas_arcs[i], pas_arcs[i + 1]);
	if(graphe.arcs() != arr) {
		std::cerr << "ECHEC - V" << std::endl;
		erreur++;
	}
	Digraph<std::string> copy(graphe);
	graphe.supprimer("BNP");
	if(graphe.sommets() != som - 1) {
    std::cerr << "ECHEC - VI" << std::endl;
    erreur++;
  }
  if(graphe.arcs() != arr - 5) {
    std::cerr << "ECHEC - VII" << std::endl;
    erreur++;
  }
	graphe.supprimer("BOA");
	if(graphe.sommets() != som - 2) {
    std::cerr << "ECHEC - VIII" << std::endl;
    erreur++;
  }
  if(graphe.arcs() != arr - 10) {
    std::cerr << "ECHEC - IX" << std::endl;
    erreur++;
	} 
	graphe.supprimer("BNP","BMO");
	graphe.supprimer("BNP","BEA");
	graphe.supprimer("ATB","BNP");
	graphe.supprimer("BIC","BNP");
	graphe.supprimer("AXA","BNP");
	graphe.supprimer("BMO","BOA");
	graphe.supprimer("BOA","NBG");
	graphe.supprimer("BOA","RBC");
	if(graphe.sommets() != som - 2 || graphe.arcs() != arr - 10) {
    std::cerr << "ECHEC - X" << std::endl;
    erreur++;
	} 
	if(copy.sommets() != som) {
    std::cerr << "ECHEC - XI" << std::endl;
    erreur++;
  }
  if(copy.arcs() != arr) {
    std::cerr << "ECHEC - XII" << std::endl;
    erreur++;
  }
	for(int i = 0; i < arr; i += 2)
    copy.supprimer(arcs[i], arcs[i + 1]);	
	if(copy.sommets() != som) {
    std::cerr << "ECHEC - XIII" << std::endl;
    erreur++;
  }
  if(copy.arcs() != arr - 17) {
    std::cerr << "ECHEC - XIV" << std::endl;
    erreur++;
	}
	int old_erreur = erreur;
	for(int i = 0; i < arr; i += 2)
    if(copy.arc(arcs[i], arcs[i + 1]) || !copy.arc(arcs[arr + i], arcs[arr + i + 1]))
			erreur++;
	if(old_erreur != erreur) {
		std::cerr << "ECHEC - XV" << std::endl;
		erreur++;
	}
	Digraph<int> digraph;
	for(int i = 10; i < 1000; i += 10)
		for(int j = 1; j < 10; j++) {
			digraph.inserer(i, i + j, 3105.3173);
			digraph.inserer(i, i - j, 3173.3105);
		}
	Digraph<int> degraph = digraph;
	if(digraph.sommets() != 999 || degraph.sommets() != 999) {
		std::cerr << "ECHEC - XVI" << std::endl;
		erreur++;
	}
	if(digraph.arcs() != 1782 || degraph.arcs() != 1782) {
		std::cerr << "ECHEC - XVII" << std::endl;
		erreur++;
	}
	for(int i = 10; i < 1000; i += 20)
		digraph.supprimer(i);
	if(digraph.sommets() != 949) {
		std::cerr << "ECHEC - XVIII" << std::endl;
		erreur++;
	}
	if(digraph.arcs() != 882) {
		std::cerr << "ECHEC - XIX" << std::endl;
		erreur++;
	}
	if(degraph.arcs() != 1782 || degraph.sommets() != 999) {
		std::cerr << "ECHEC - XX" << std::endl;
		erreur++;
	}
	for(int i = 10; i < 1000; i += 10)
		for(int j = 1; j < 10; j++)
			degraph.supprimer(i, i + j);
	if(degraph.arcs() != 891 || degraph.sommets() != 999) {
		std::cerr << "ECHEC - XXI" << std::endl;
		erreur++;
	}
	for(int i = 1; i < 1000; i++)
		if(i % 10 != 0)
			degraph.supprimer(i);
	if(degraph.arcs() != 0 || degraph.sommets() != 99) {
		std::cerr << "ECHEC - XXII" << std::endl;
		erreur++;
	}
  if(erreur == 0)
    std::cout << "\t==> OK" << std::endl;
  return erreur;
}
