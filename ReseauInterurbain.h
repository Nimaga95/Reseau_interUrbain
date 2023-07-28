/**
 * \file ReseauInterurbain.h
 * \brief Gestion d'un réseau de transport interurbain.
 * \author El hadji Djimé Nimaga
 * \version 0.1
 * \date juin-juillet 2023
 *
 *
 */

#include "Graphe.h"
#include <iostream>
#include <stack>
#include <string>
#include <stdexcept>
#include <climits>
#include <vector>
#include <list>
#include <cfloat>
//vous pouvez inclure d'autres librairies de la STL si vous pensez les utiliser


#ifndef _RESEAUINTERURBAIN__H
#define _RESEAUINTERURBAIN__H

namespace TP2
{

    struct Chemin
    {
        //listeVilles doit contenir la liste des villes formant le chemin
        //La ville de départ du chemin correspond à l'élément d'indice 0 du vecteur
        //la ville d'arrivée correspond au dernier élément du vecteur.
        std::vector<std::string> listeVilles;

        float dureeTotale;		// Durée totale du trajet en heures
        float coutTotal;			// Cout total en dollars du chemin entre la ville de départ et la ville de destination
        bool reussi;				// Un booléen qui vaut true si le chemin a été trouvé, false sinon
    };

    class ReseauInterurbain{

    public:

        std::string getNomReseau() const;

        size_t getNombreVilles() const;
        // Constructeur
        ReseauInterurbain(std::string nomReseau, size_t nbVilles = 10);

        // Destructeur
        ~ReseauInterurbain();

        // Change la taille du réseau en utilisant un nombre de villes = nouvelleTaille
        void resize(size_t nouvelleTaille);

        //Ne touchez pas à cette fonction !
        friend std::ostream& operator<<(std::ostream& out, const ReseauInterurbain& r)
        {
            out << r.unReseau << std::endl;
            return out;
        }

        // Charger un réseau à partir d'un fichier texte en entrée (voir format du fichier dans l'énoncé du Tp).
        // Construit un réseau routier à partir d'une liste de villes ainsi que leurs liens.
        // fichierEntree est ouvert corectement.
        // fichierEntree n'est pas fermé par la fonction.
        // Exception logic_error si fichierEntree n'est pas ouvert correctement.
        void chargerReseau(std::ifstream & fichierEntree);

        // Trouve le plus court chemin entre une ville source et une ville destination en utilsiant l'algorithme de Dijkstra
        // si dureeCout = true, on utilise la durée comme pondération au niveau des trajets
        // si dureeCout = false, on utilise le coût (en $) comme pondération au niveau des trajets
        // Exception std::logic_error si origine et/ou destination absent du réseau
        Chemin rechercheCheminDijkstra(const std::string& origine, const std::string& destination, bool dureeCout) const;

        // Trouve l’ensemble des composantes fortement connexes en utilisant l'algorithme de Kosaraju
        // Retourne un vecteur de vecteurs de chaînes caractères. Chaque sous-vecteur représente une composante.
        std::vector<std::vector<std::string> > algorithmeKosaraju();

        //Vous pouvez ajoutez d'autres méthodes publiques si vous sentez leur nécessité

    private:

        Graphe unReseau;			//Le type ReseauRoutier est composé d'un graphe
        std::string nomReseau;		// Le nom du reseau (exemple: Orleans Express)

        std::vector<unsigned int> debut;
        std::vector<unsigned int> fin;
        // Vous pouvez définir des constantes ici. À vous de voir!

        const float MAX = FLT_MAX;

        //Vous pouvez ajoutez d'autres méthodes privées si vous sentez leur nécessité


    };

}//Fin du namespace

#endif
