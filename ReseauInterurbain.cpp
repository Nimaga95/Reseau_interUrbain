/**
 * \file ReseauInterurbain.cpp
 * \brief Implémentattion de la classe ReseauInterurbain.
 * \author El hadji Djimé Nimaga
 * \version 0.1
 * \date juin-juillet 2023
 *
 *
 */
#include <sstream>
#include <fstream>
#include <algorithm>
#include <stack>
#include <limits>
#include <queue>

#include "ReseauInterurbain.h"
//vous pouvez inclure d'autres librairies si c'est nécessaire

namespace TP2
{

    /**
     * @brief Constructeur de la classe ReseauInterurbain.
     * @param nomReseau Le nom du réseau interurbain.
     * @param nbVilles Le nombre de villes dans le réseau.
     */
    ReseauInterurbain::ReseauInterurbain(std::string nomReseau, size_t nbVilles) :
            nomReseau(nomReseau)
    {
        // Le corps du constructeur est vide car aucune initialisation supplémentaire n'est nécessaire.
    }


    /**
     * @brief Destructeur de la classe ReseauInterurbain.
     */
    ReseauInterurbain::~ReseauInterurbain() {
        // Le corps du destructeur est vide car aucune opération supplémentaire n'est nécessaire.
    }


    /**
     * @brief Redimensionne le réseau interurbain.
     * @param nouvelleTaille La nouvelle taille du réseau interurbain.
     */
    void ReseauInterurbain::resize(size_t nouvelleTaille) {
        unReseau.resize(nouvelleTaille);
    }


    // Méthode fournie
    /**
     * @brief Charge le réseau interurbain à partir d'un fichier.
     * @param fichierEntree Le fichier d'entrée contenant les données du réseau.
     * @throws std::logic_error Si le fichier d'entrée n'est pas ouvert.
     */
    void ReseauInterurbain::chargerReseau(std::ifstream & fichierEntree)
    {
        if (!fichierEntree.is_open())
            throw std::logic_error("ReseauInterurbain::chargerReseau: Le fichier n'est pas ouvert !");

        std::string buff;

        getline(fichierEntree, nomReseau);
        nomReseau.erase(0, 20); // Enlève: Reseau Interurbain:

        int nbVilles;

        fichierEntree >> nbVilles;
        getline(fichierEntree, buff); //villes

        unReseau.resize(nbVilles);

        getline(fichierEntree, buff); //Liste des villes

        size_t i = 0;

        getline(fichierEntree, buff); //Premiere ville

        while(buff != "Liste des trajets:")
        {
            unReseau.nommer(i, buff);
            getline(fichierEntree, buff);
            i++;
        }

        while(!fichierEntree.eof())
        {
            getline(fichierEntree, buff);
            std::string source = buff;
            getline(fichierEntree, buff);
            std::string destination = buff;

            getline(fichierEntree, buff);
            std::istringstream iss(buff);

            float duree;
            iss >> duree;

            float cout;
            iss >> cout;

            unReseau.ajouterArc(unReseau.getNumeroSommet(source), unReseau.getNumeroSommet(destination), duree, cout);
        }
    }

    /**
     * @brief Algorithme Djikstra pour la recherche du chemin le plus court ou le moins coûteux entre deux villes du réseau en utilisant l'algorithme de Dijkstra.
     * @param source La ville de départ.
     * @param destination La ville d'arrivée.
     * @param dureeCout Indicateur spécifiant si le critère de recherche est la durée (true) ou le coût (false).
     * @return Le chemin trouvé.
     * @throws std::logic_error Si la ville source est hors du réseau, si la ville destination est hors du réseau, ou si tous les arcs ne sont pas non négatifs.
     */
    Chemin ReseauInterurbain::rechercheCheminDijkstra(const std::string& source, const std::string& destination, bool dureeCout) const
    {
        // Vérifier si la ville source est hors du réseau
        if (static_cast<int>(unReseau.getNumeroSommet(source)) > unReseau.getNombreSommets())
        {
            throw std::logic_error("rechercheCheminDijkstra : source hors du réseau");
        }
        // Vérifier si la ville destination est hors du réseau
        if (static_cast<int>(unReseau.getNumeroSommet(destination)) > unReseau.getNombreSommets())
        {
            throw std::logic_error("rechercheCheminDijkstra : destination hors réseau");
        }
        // Vérifier si tous les arcs sont non négatifs
        if (!unReseau.arcsSontNonNegatifs())
        {
            throw std::logic_error("rechercheCheminDijkstra : les arcs ne sont pas tous non négatifs");
        }
        // Récupérer les numéros de sommet correspondant à la source et à la destination
        auto numero_source = unReseau.getNumeroSommet(source);
        auto numero_destination = unReseau.getNumeroSommet(destination);

        // Tableaux pour stocker les distances minimales et les prédécesseurs des sommets
        std::vector<float> distances(unReseau.getNombreSommets(), MAX);
        std::vector<unsigned int> predecesseurs(unReseau.getNombreSommets(), MAX);

        // Tableau pour indiquer si les sommets ont été solutionnés ou non
        std::vector<bool> solutionnes(unReseau.getNombreSommets(), false);

        // Initialisation de la distance de la source à 0
        distances[numero_source] = 0.0;

        // Boucle principale de l'algorithme de Dijkstra
        for (auto i = 0; i < unReseau.getNombreSommets(); i++) {

            float minimum = MAX;
            unsigned int voisin = 0;

            // Recherche du sommet non solutionné avec la distance minimale
            for (auto j = 0; j < unReseau.getNombreSommets(); j++) {
                if (!solutionnes[j] && distances[j] < minimum) {
                    minimum = distances[j];
                    voisin = j;
                }
            }

            // Marquer le sommet comme solutionné
            solutionnes[voisin] = true;

            // Parcourir les sommets adjacents au sommet solutionné
            for (auto v : unReseau.listerSommetsAdjacents(voisin)) {
                if (!solutionnes[v]) {
                    float tmp;
                    if (dureeCout) {
                        // Calculer la distance temporaire en ajoutant la durée de l'arc au sommet solutionné
                        tmp = distances[voisin] + unReseau.getPonderationsArc(voisin, v).duree;
                    }
                    else {
                        // Calculer la distance temporaire en ajoutant le coût de l'arc au sommet solutionné
                        tmp = distances[voisin] + unReseau.getPonderationsArc(voisin, v).cout;
                    }
                    // Mettre à jour la distance minimale et le prédécesseur si la distance temporaire est plus petite
                    if (tmp < distances[v]) {
                        distances[v] = tmp;
                        predecesseurs[v] = voisin;
                    }
                }
            }
        }

        // Construction du chemin trouvé
        Chemin cheminTrouve;

        // Vérifier si un chemin a été trouvé jusqu'à la destination
        if (distances[numero_destination] == MAX)
        {
            cheminTrouve.reussi = false;
        }
        else
        {
            cheminTrouve.reussi = true;
            if (dureeCout)
            {
                cheminTrouve.dureeTotale = distances[numero_destination];
            }
            else
            {
                cheminTrouve.coutTotal = distances[numero_destination];
            }

            // Reconstruction du chemin en remontant les prédécesseurs depuis la destination jusqu'à la source
            auto sentinelle = numero_destination;
            std::stack<std::string> villes;

            while (sentinelle != numero_source) {
                villes.push(unReseau.getNomSommet(sentinelle));
                sentinelle = predecesseurs[sentinelle];
            }

            // Ajouter la source et les villes du chemin dans l'objet Chemin
            cheminTrouve.listeVilles.push_back(source);
            while (!villes.empty())
            {
                cheminTrouve.listeVilles.push_back(villes.top());
                villes.pop();
            }
        }

        //Renvoie le chemin trouvé
        return cheminTrouve;
    }

    /**
     * @brief Algorithme de Kosaraju pour trouver les composantes fortement connexes du réseau interurbain.
     * @return Vecteur de vecteurs de chaînes de caractères représentant les composantes fortement connexes.
     */
    std::vector<std::vector<std::string>> ReseauInterurbain::algorithmeKosaraju()
    {
        // Vecteur de composantes fortement connexes
        std::vector<std::vector<std::string>> composantes;

        // Création du graphe inverse
        Graphe inverse = unReseau.inverseGraphe();

        // Vecteur pour suivre les sommets visités lors du parcours en profondeur
        std::vector<bool> sommetsVisites(inverse.getNombreSommets(), false);

        // Parcours en profondeur du graphe inverse
        auto pile = Graphe::parcoursProfondeur(inverse);

        while (!pile.empty())
        {
            // Récupération du sommet en haut de la pile
            auto sommet = pile.top();
            pile.pop();

            // Vérification si le sommet a déjà été visité
            if (!sommetsVisites[sommet])
            {
                // Exploration du sommet et récupération des sommets de la composante fortement connexe
                std::stack<unsigned int> resultat;
                Graphe::explore(sommet, inverse, sommetsVisites, resultat);

                // Création d'une nouvelle composante fortement connexe
                std::vector<std::string> composante;
                while (!resultat.empty())
                {
                    // Ajout du nom du sommet à la composante
                    composante.push_back(unReseau.getNomSommet(resultat.top()));
                    resultat.pop();
                }

                // Ajout de la composante au vecteur de composantes
                composantes.push_back(composante);
            }
        }

        return composantes;
    }

/**
 * @brief Renvoie le nom du réseau interurbain.
 * @return Le nom du réseau interurbain.
 */
    std::string ReseauInterurbain::getNomReseau() const {
        return nomReseau;
    }

/**
 * @brief Renvoie le nombre de villes dans le réseau interurbain.
 * @return Le nombre de villes dans le réseau interurbain.
 */
    size_t ReseauInterurbain::getNombreVilles() const {
        return unReseau.getNombreSommets();
    }

}//Fin du namespace TP2
