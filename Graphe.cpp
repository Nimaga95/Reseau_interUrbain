/**
 * \file Graphe.cpp
 * \brief Implémentation d'un graphe orienté.
 * \author EL hadji Djimé Nimaga
 * \version 0.1
 * \date juin-juillet 2023
 *
 *  Travail pratique numéro 2
 *
 */

#include <istream>
#include <algorithm>
#include "Graphe.h"

namespace TP2
{

    /**
     * @brief Constructeur de la classe Graphe.
     * @param[in] nbSommets : le nombre de sommets du graphe.
     */
    Graphe::Graphe(size_t nbSommets) : noms(std::vector<std::string>(nbSommets)), listesAdj(std::vector<std::list<Arc>>(nbSommets)), nbSommets(nbSommets), nbArcs(0)
    {
        // Le corps du constructeur est vide car aucune initialisation supplémentaire n'est nécessaire.
    }


    /**
     * \brief Destructeur de la classe Graphe
     */
    Graphe::~Graphe()
    {
        // Le corps du destructeur est vide car aucune opération supplémentaire n'est nécessaire.
    }


    /**
     * @brief Cette méthode redimensionne le graphe avec une nouvelle taille.
     * @param nouvelleTaille La nouvelle taille du graphe.
     */
    void Graphe::resize(size_t nouvelleTaille)
    {
        // Redimensionner le vecteur des noms des sommets
        noms.resize(nouvelleTaille);

        // Redimensionner le vecteur des listes d'adjacence
        listesAdj.resize(nouvelleTaille);

        // Mettre à jour le nombre de sommets avec la nouvelle taille
        nbSommets = nouvelleTaille;
    }


    /**
     * @brief Cette méthode assigne un nom à un sommet du graphe.
     * @param sommet Le numéro du sommet à nommer.
     * @param nom Le nom à assigner au sommet.
     * @throws std::logic_error Si le sommet donné est invalide.
     */
    void Graphe::nommer(size_t sommet, const std::string& nom){

        // Vérifier si le numéro de sommet est valide
        if (sommet > nbSommets)
        {
            throw std::logic_error("nommer : sommet invalide");
        }
            // Assigner le nom au sommet s'il est valide
        else {
            noms[sommet].assign(nom);
        }
    }


    /**
     * @brief Cette méthode ajoute un arc entre deux sommets du graphe avec une durée et un coût donnés.
     * @param source Le numéro du sommet source de l'arc.
     * @param destination Le numéro du sommet destination de l'arc.
     * @param duree La durée de l'arc.
     * @param cout Le coût de l'arc.
     * @throws std::logic_error Si le numéro de sommet source ou de sommet destination est invalide ou si l'arc existe déjà.
     */
    void Graphe::ajouterArc(size_t source, size_t destination, float duree, float cout){

        // Vérifier si le numéro de sommet source est valide
        if (source > getNombreSommets())
        {
            throw std::logic_error("ajouterArc: numéro de sommet source invalide");
        }

        // Vérifier si le numéro de sommet destination est valide
        if (destination > getNombreSommets())
        {
            throw std::logic_error("ajouterArc: numéro de sommet destination invalide");
        }

        // Vérifier si l'arc existe déjà entre les sommets source et destination
        if (arcExiste(source, destination))
        {
            throw std::logic_error("ajouterArc: l'arc existe déjà");
        }

        // Créer un nouvel arc avec la destination et les pondérations spécifiées
        auto tmp = new Graphe::Arc(destination, Ponderations(duree, cout));

        // Ajouter l'arc à la liste d'adjacence du sommet source
        listesAdj[source].push_back(*tmp);

        // Augmenter le nombre d'arcs du graphe
        nbArcs++;
    }


    /**
     * @brief Cette méthode enlève un arc entre deux sommets du graphe.
     * @param source Le numéro du sommet source de l'arc à enlever.
     * @param destination Le numéro du sommet destination de l'arc à enlever.
     * @throws std::logic_error Si le numéro de sommet source ou de sommet destination est invalide ou si l'arc n'existe pas.
     */
    void Graphe::enleverArc(size_t source, size_t destination){

        // Vérifier si le numéro de sommet source est valide
        if (source > getNombreSommets())
        {
            throw std::logic_error("enleverArc: numéro de sommet source invalide");
        }

        // Vérifier si le numéro de sommet destination est valide
        if (destination > getNombreSommets())
        {
            throw std::logic_error("enleverArc: numéro de sommet destination invalide");
        }

        // Vérifier si l'arc existe entre les sommets source et destination
        if (!arcExiste(source, destination))
        {
            throw std::logic_error("enleverArc: l'arc n'existe pas");
        }

        // Parcourir la liste d'adjacence du sommet source pour trouver l'arc à enlever
        for (auto itr = listesAdj[source].begin(); itr != listesAdj[source].end(); ++itr )
        {
            // Si l'arc a la destination recherchée, le supprimer de la liste d'adjacence et diminuer le nombre d'arcs
            if ((*itr).destination == destination)
            {
                listesAdj[source].erase(itr);
                nbArcs--;
            }
        }
    }


    /**
     * @brief Cettte méthode vérifie si un arc existe entre deux sommets du graphe.
     * @param source Le numéro du sommet source.
     * @param destination Le numéro du sommet destination.
     * @return True si l'arc existe, False sinon.
     * @throws std::logic_error Si le numéro de sommet source ou de sommet destination est invalide.
     */
    bool Graphe::arcExiste(size_t source, size_t destination) const{

        // Vérifier si le numéro de sommet source est valide
        if (source > getNombreSommets()){
            throw std::logic_error("arcExiste: source invalide");
        }

        // Vérifier si le numéro de sommet destination est valide
        if (destination > getNombreSommets()){
            throw std::logic_error("arcExiste: destination invalide");
        }

        // Parcourir les arcs de la liste d'adjacence du sommet source
        for (auto arc : listesAdj[source]){
            if (arc.destination == destination){
                return true;  // L'arc existe
            }
        }

        return false;  // L'arc n'existe pas
    }


    /**
     * @brief Cette méthode liste les sommets adjacents à un sommet donné.
     * @param sommet Le numéro du sommet pour lequel on souhaite lister les sommets adjacents.
     * @return Un vecteur contenant les numéros des sommets adjacents.
     * @throws std::logic_error Si le numéro de sommet est invalide.
     */
    std::vector<size_t> Graphe::listerSommetsAdjacents(size_t sommet) const
    {
        // Vérifier si le numéro de sommet est valide
        if (sommet > getNombreSommets())
        {
            throw std::logic_error("listerSommetsAdjacents: numéro de sommet invalide");
        }

        // Créer un vecteur pour stocker les numéros des sommets adjacents
        std::vector<size_t> listeSommets;

        // Parcourir les arcs de la liste d'adjacence du sommet donné
        for (auto itr = listesAdj[sommet].begin(); itr != listesAdj[sommet].end(); ++itr)
        {
            // Ajouter le numéro du sommet destination à la liste
            listeSommets.push_back(itr->destination);
        }

        return listeSommets;
    }


    /**
     * @brief Cette méthode obtient le nom d'un sommet donné.
     * @param sommet Le numéro du sommet pour lequel on souhaite obtenir le nom.
     * @return Le nom du sommet.
     * @throws std::logic_error Si le numéro de sommet est invalide.
     */
    std::string Graphe::getNomSommet(size_t sommet) const
    {
        // Vérifier si le numéro de sommet est valide
        if (sommet > getNombreSommets())
        {
            throw std::logic_error("getNomSommet: numéro de sommet invalide");
        }

        // Retourner le nom du sommet correspondant
        return noms[sommet];
    }


    /**
     * @brief Cette méthode obtient le numéro d'un sommet à partir de son nom.
     * @param nom Le nom du sommet pour lequel on souhaite obtenir le numéro.
     * @return Le numéro du sommet.
     * @throws std::logic_error Si le nom du sommet n'existe pas dans le graphe.
     */
    size_t Graphe::getNumeroSommet(const std::string& nom) const
    {
        try
        {
            for(auto i = 0; i < getNombreSommets(); i++)
            {
                if (noms[i] == nom)
                {
                    return i;  // Retourner le numéro du sommet correspondant au nom
                }
            }
            throw std::logic_error("getNumeroSommet: le nom n'existe pas dans le graphe");
        }
        catch (const std::exception& e)
        {
            throw;  // Lancer à nouveau l'exception pour la propager
        }
    }


    /**
    * @brief Cette méthode retourne le nombre de sommets du graphe.
    * @return Le nombre de sommets du garphe.
    */
    int Graphe::getNombreSommets() const
    {
        return nbSommets;
    }

    /**
    * @brief Cette méthode retourne le nombre d'arcs du graphe.
    * @return Le nombre d'arcs du graphe.
    */
    int Graphe::getNombreArcs() const
    {
        return nbArcs;
    }


    /**
     * @brief Cette méthode obtient les pondérations d'un arc entre deux sommets.
     * @param source Le numéro du sommet source de l'arc.
     * @param destination Le numéro du sommet destination de l'arc.
     * @return Les pondérations de l'arc.
     * @throws std::logic_error Si le numéro de sommet source est invalide, le numéro de sommet destination est invalide
     * ou l'arc n'existe pas dans le graphe.
     */
    Ponderations Graphe::getPonderationsArc(size_t source, size_t destination) const
    {
        // Vérifier si le numéro de sommet source est valide
        if (source > getNombreSommets())
        {
            throw std::logic_error("getPonderationsArc: numéro de sommet source invalide");
        }

        // Vérifier si le numéro de sommet destination est valide
        if (destination > getNombreSommets())
        {
            throw std::logic_error("getPonderationsArc: numéro de sommet destination invalide");
        }

        // Vérifier si l'arc existe dans le graphe
        if (!arcExiste(source, destination))
        {
            throw std::logic_error("getPonderationsArc: l'arc n'existe pas dans le graphe");
        }

        // Parcourir les arcs de la liste d'adjacence du sommet source
        for (auto itr = listesAdj[source].begin(); itr != listesAdj[source].end(); ++itr) {
            if ((*itr).destination == destination)
            {
                return ((*itr).poids);  // Retourner les pondérations de l'arc
            }
        }

        // L'arc n'a pas été trouvé, lancer une exception appropriée ou retourner une valeur par défaut
        throw std::logic_error("getPonderationsArc: l'arc n'a pas été trouvé dans la liste d'adjacence");
    }


    /**
     * @brief Vérifie si les pondérations de tous les arcs du graphe sont non négatives.
     * @return true si les pondérations de tous les arcs sont non négatives, false sinon.
     * @throws std::logic_error Si le nombre de sommets est invalide.
     */
    bool Graphe::arcsSontNonNegatifs() const {
        // Vérifier si le nombre de sommets est valide
        if (this->getNombreSommets() < 0)
        {
            throw std::logic_error("arcsSontNonNegatifs: nombre de sommets invalides");
        }

        // Parcourir toutes les listes d'adjacence
        for (auto liste : listesAdj)
        {
            // Parcourir tous les arcs de la liste d'adjacence
            for(auto arc : liste)
            {
                // Vérifier si les pondérations du arc sont négatives
                if (arc.poids.cout < 0 || arc.poids.duree < 0)
                {
                    return false;  // Les pondérations d'un arc sont négatives, retourner false
                }
            }
        }

        return true;  // Toutes les pondérations d'arcs sont non négatives, retourner true
    }


    /**
     * @brief Cette méthode retourne l'inverse du graphe courant.
     * @return L'inverse du graphe courant.
     * @throws std::logic_error Si le nombre de sommets du graphe est invalide ou si le nombre de sommets ou d'arcs du graphe de base n'est pas égal à l'inverse.
     */
    Graphe Graphe ::inverseGraphe() const {

        // Vérifier si le nombre de sommets est valide
        if (this->getNombreSommets() < 0)
        {
            throw std::logic_error ("inverse : nombre de sommets inavalide");
        }
        // Si le graphe de base n'a pas d'arcs, retourner le graphe de base lui-même
        if(this->getNombreArcs() == 0)
        {
            return *this;
        }
        // Créer un nouveau graphe pour l'inverse
        auto inverse = new Graphe(nbSommets);
        // Parcourir tous les sommets du graphe de base
        for(int i = 0; i<this->getNombreSommets(); i++)
        {
            // Parcourir tous les sommets du graphe de base
            for (int j=0; j<this->getNombreSommets(); j++)
            {
                // Si l'arc existe entre les sommets (i,j) dans le graphe de base
                if (this ->arcExiste(i,j)){
                    // Ajouter un arc entre les sommets (j,i) dans l'inverse avec les mêmes pondérations
                    inverse->ajouterArc(j,i, this->getPonderationsArc(i,j).duree,
                                        this->getPonderationsArc(i,j).cout);
                }

            }
        }
        // Vérifier si le nombre de sommets du graphe de base est égal au nombre de sommets de l'inverse
        if (this->getNombreSommets() != inverse->getNombreSommets())
        {
            throw std::logic_error("inverse : nombreSommets graphe de base pas égal à l'inverse.");
        }
        // Vérifier si le nombre d'arcs du graphe de base est égal au nombre d'arcs de l'inverse
        if (this->getNombreArcs() != inverse->getNombreArcs())
        {
            throw std::logic_error("inverse : nombreArcs graphe de base pas égal à l'inverse.");
        }
        // Retourner l'inverse du graphe de base
        return *inverse;
    }


    /**
     * @brief Explore récursivement le graphe à partir d'un sommet donné.
     * @param sommet Le sommet à partir duquel l'exploration commence.
     * @param graphe Le graphe à explorer.
     * @param sommetsVisites Un vecteur booléen indiquant si un sommet a été visité ou non.
     * @param pile Une pile utilisée pour stocker les sommets visités.
     * @throws std::logic_error Si le sommet est invalide.
     */
    void Graphe::explore(unsigned int sommet, Graphe& graphe, std::vector<bool>& sommetsVisites, std::stack<unsigned int>& pile){
        // Vérifier si le sommet est valide
        if (static_cast<int>(sommet) > graphe.getNombreSommets())
        {
            throw std::logic_error("explore: sommet invalide");
        }

        // Marquer le sommet comme visité et l'ajouter à la pile
        sommetsVisites[sommet] = true;
        pile.push(sommet);

        // Parcourir tous les sommets adjacents au sommet actuel
        for (auto voisin : graphe.listerSommetsAdjacents(sommet)){
            // Si le voisin n'a pas encore été visité, appeler récursivement la fonction explore sur ce voisin
            if (!sommetsVisites[voisin]){
                explore(voisin, graphe, sommetsVisites, pile);
            }
        }
    }

    /**
     * @brief Effectue un parcours en profondeur du graphe.
     * @param graphe Le graphe à parcourir en profondeur.
     * @return Une pile contenant les sommets visités dans l'ordre du parcours en profondeur.
     * @throws std::logic_error Si le nombre de sommets du graphe est invalide.
     */
    std::stack<unsigned int> Graphe::parcoursProfondeur(Graphe& graphe)
    {
        // Vérifier si le nombre de sommets du graphe est valide
        if (graphe.getNombreSommets() <= 0)
        {
            throw std::logic_error("parcoursProfondeur: nombreSommets invalide");
        }

        std::stack<unsigned int> pile;
        std::vector<bool> sommetsVisites(graphe.getNombreSommets(), false);

        // Parcourir tous les sommets du graphe
        for (auto i = 0; i < graphe.getNombreSommets(); i++)
        {
            // Si le sommet n'a pas encore été visité, appeler la méthode explore pour effectuer le parcours en profondeur
            if (!sommetsVisites[i])
            {
                Graphe::explore(i, graphe, sommetsVisites, pile);
            }
        }

        // Retourner la pile contenant les sommets visités dans l'ordre du parcours en profondeur
        return pile;
    }

}//Fin du namespace TP2
