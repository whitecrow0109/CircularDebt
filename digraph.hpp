#include "digraph.h"

template <class T>
Digraph<T>::Digraph() {
	graphe.clear();
	inverseGraphe.clear();
}

template <class T>
Digraph<T>::Digraph(const Digraph& autre) {
	graphe = autre.graphe;
	inverseGraphe = autre.inverseGraphe;
}

template <class T>
Digraph<T>::~Digraph() {
	graphe.clear();
	inverseGraphe.clear();
}

template <class T>
Digraph<T> & Digraph<T>::operator =(const Digraph<T> & autre) {
	graphe = autre.graphe;
	inverseGraphe = autre.inverseGraphe;
	return *this;
}

template <class T>
int Digraph<T>::sommets() const {
	return graphe.size();
}

template <class T>
int Digraph<T>::arcs() const {
	int cnt = 0;
	for (auto it : graphe) {
		cnt += it.second.size();
	}
	return cnt;
}

template <class T>
bool Digraph<T>::sommet(T u) const {
	return graphe.count(u);
}

template <class T>
bool Digraph<T>::arc(T u, T v) const {
	if (!graphe.count(u)) return false;
	for (auto it : graphe.at(u)) {
		if (get<0>(it) == v) return true;
	}
	return false;
}

template <class T>
void Digraph<T>::inserer(T u) {
	if (sommet(u)) return;
	graphe[u] = std::set<std::tuple<T,double>>();
	inverseGraphe[u] = std::set<std::tuple<T,double>>();
}

template <class T>
void Digraph<T>::inserer(T u, T v, double emprunt) {
	if (arc(u, v)) return;
 	assert(u != v && emprunt > 0);
	inserer(u);	inserer(v);
	graphe[u].insert(make_tuple(v, emprunt));
	inverseGraphe[v].insert(make_tuple(u, emprunt));
}

template <class T>
void Digraph<T>::supprimer(T u) {
	if (!graphe.count(u)) return;
	
	for (auto e : graphe[u]) {
		T v = get<0>(e);
		inverseGraphe[v].erase(make_tuple(u, get<1>(e)));
	}
	inverseGraphe.erase(u);
	
	graphe.erase(u);
	vector<tuple<T, T, double>> vec;
	for (auto it : graphe) {
		T maintenant = it.first;
		for (auto e : graphe[maintenant]) {
			if (get<0>(e) == u) {
				vec.push_back(make_tuple(maintenant, get<0>(e), get<1>(e)));
			}
		}
	}
	for (auto e : vec) {
		graphe[get<0>(e)].erase(make_tuple(get<1>(e), get<2>(e)));
	}
}

template <class T>
void Digraph<T>::supprimer(T u, T v) {
	if (!graphe.count(u)) return;
	for (auto it : graphe[u]) {
		if (get<0>(it) == v) {
			graphe[u].erase(make_tuple(v, get<1>(it)));
			break;
		}
	}
	
	if (inverseGraphe.count(v)) {
		for (auto it : inverseGraphe[v]) {
			if (get<0>(it) == u) {
				inverseGraphe[v].erase(make_tuple(u, get<1>(it)));
				break;
			}
		}
	}
}

template <class T>
Digraph<T> Digraph<T>::cooperative(std::set<T> & S) const{
	Digraph<T> Cop;
	for (auto u : S) {
		if (!sommet(u)) {
			std::cout << "le sommet " << u << " n\'est pas le sommet du graphe courant" << std::endl;
			assert(0);
		}
		Cop.inserer(u);
		for (auto it : graphe.at(u)) {
			T v = get<0>(it);
			double emprunt = get<1>(it);
			if (S.count(v)) Cop.inserer(u, v, emprunt);
		}
	}
	return Cop;
}

template <class T>
double Digraph<T>::dette(T u) const {
	double somme = 0;
	if (inverseGraphe.count(u)) {
		for (auto it : inverseGraphe.at(u)) {
			somme += std::get<1>(it);
		}
	}
	
	return somme;
}

template <class T>
double Digraph<T>::dette(const Digraph<T> & SG) const {
	double somme = 0;
	for (auto it : SG.graphe) {
		T u = it.first;
		if (!inverseGraphe.count(u)) continue;
		for (auto e : inverseGraphe.at(u)) {
			T v = get<0>(e);
			double emprunt = get<1>(e);
			if (!SG.graphe.count(v)) somme += emprunt;
		}
	}
	return somme;
}

template <class T>
double Digraph<T>::pret(T u) const {
	double somme = 0;
	if (graphe.count(u)) {
		for (auto it : graphe.at(u)) {
			somme += std::get<1>(it);
		}
	}
	
	return somme;
}

template <class T>
double Digraph<T>::pret(const Digraph<T> & SG) const {
	double somme = 0;
	for (auto it : SG.graphe) {
		T u = it.first;
		if (!graphe.count(u)) continue;
		for (auto e : graphe.at(u)) {
			T v = get<0>(e);
			double emprunt = get<1>(e);
			if (!SG.graphe.count(v)) somme += emprunt;
		}
	}
	return somme;
}

const double eps = 1e-6;

template <class T>
double Digraph<T>::trouverChemin(const T& commencer, T maintenant, double empruntMin, std::vector<T>& chemin, std::set<T>& G) {
	if (commencer == maintenant) {
		return empruntMin;
	}
	for (auto it : graphe[maintenant]) {
		T v = get<0>(it);
		double emprunt = get<1>(it);
		if (G.count(v)) continue;
		G.insert(v);	chemin.push_back(v);
		double emp = trouverChemin(commencer, v, min(empruntMin, emprunt), chemin, G);
		if (fabs(emp + 1.0) > eps) return emp;
		chemin.pop_back();
	}
	return -1.0;
}

template <class T>
void Digraph<T>::reduire(T s) {
	if (!graphe.count(s)) return;
	while(true) {
		bool exister = false;
		for (auto it : graphe[s]) {
			std::vector<T> chemin;
			T u = get<0>(it);
			double emprunt = get<1>(it);
			if (fabs(emprunt) < eps) continue;
			
			chemin.push_back(s);
			chemin.push_back(u);
			
			set<T> G;
			G.insert(u);
			
			double empruntMin = trouverChemin(s, u, emprunt, chemin, G);
			if (fabs(empruntMin + 1.0) > eps) {
				int sz = chemin.size();
				for (int i = 0; i < sz - 1; i ++) {
					T A = chemin[i], B = chemin[i + 1];
					for (auto it : graphe[A]) {
						T v = get<0>(it);
						double emp = get<1>(it);
						if (v == B) {
							supprimer(A, B);
							double remp = emp - empruntMin;
							if (fabs(remp) > eps) {
								inserer(A, B, remp);
 							}
							break;
						}
					}
				}
				exister = true;
				break;
			}
		}
		if (!exister) break;
	}
}

template <class T>
double Digraph<T>::trouverChemin(const std::set<T>& SV, T maintenant, double empruntMin, std::vector<T>& chemin, std::set<T>& G) {
	if (SV.count(maintenant)) {
		return empruntMin;
	}
	for (auto it : graphe[maintenant]) {
		T v = get<0>(it);
		double emprunt = get<1>(it);
		if (G.count(v)) continue;
		G.insert(v);	chemin.push_back(v);
		double emp = trouverChemin(SV, v, min(empruntMin, emprunt), chemin, G);
		if (fabs(emp + 1.0) > eps) return emp;
		chemin.pop_back();
	}
	return -1.0;
}

template <class T>
void Digraph<T>::reduire(const Digraph<T> & SG) {
	std::set<T> SV;
	for (auto it : SG.graphe) {
		if (graphe.count(it.first)) SV.insert(it.first);
	}
	for (T s : SV) {
		while(true) {
			bool exister = false;
			for (auto it : graphe[s]) {
				T u = get<0>(it);
				if (SV.count(u)) continue;
				std::vector<T> chemin;
				double emprunt = get<1>(it);
				if (fabs(emprunt) < eps) continue;
				
				chemin.push_back(s);
				chemin.push_back(u);
				
				std::set<T> G;
				G.insert(u);
				
				double empruntMin = trouverChemin(SV, u, emprunt, chemin, G);
				if (fabs(empruntMin + 1.0) > eps) {
					int sz = chemin.size();
					for (int i = 0; i < sz - 1; i ++) {
						T A = chemin[i], B = chemin[i + 1];
						for (auto it : graphe[A]) {
							T v = get<0>(it);
							double emp = get<1>(it);
							if (v == B) {
								supprimer(A, B);
								double remp = emp - empruntMin;
								if (fabs(remp) > eps) {
									inserer(A, B, remp);
	 							}
								break;
							}
						}
					}
					T A = chemin[0], B = chemin[sz - 1];
					if (A != B) {
						bool insererArc = false;
						for (auto e : graphe[A]) {
							T v = get<0>(e);
							if (v == B) {
								supprimer(A, B);
								inserer(A, B, empruntMin + get<1>(e));
								insererArc = true;
								break;
							}
						}
						for (auto e : graphe[B]) {
							T v = get<0>(e);
							if (v == A) {
								supprimer(B, A);
								double emprunt = get<1>(e);
								if (empruntMin > emprunt) inserer(A, B, empruntMin - emprunt);
								else if (empruntMin < emprunt) inserer(B, A, emprunt - empruntMin);
								insererArc = true;
								break;
							}
						}
						if (!insererArc) inserer(A, B, empruntMin);
					}
					exister = true;
					break;
				}
			}
			
			if (!exister) break;
		}
	}
}

