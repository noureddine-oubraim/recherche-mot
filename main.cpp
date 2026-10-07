#include <iostream> // Entrées/Sorties (std::cout, std::cin)
#include <string>   // Gestion des chaînes de caractères std::string
#include <vector>   // Conteneur dynamique std::vector
#include <chrono>   // Mesure du temps d'exécution haute précision
#include <fstream>  // Lecture de fichiers (std::ifstream)
#include <sstream>  // Flux de traitement de chaînes (std::stringstream)
#include <filesystem> // Gestion des chemins de fichiers
#include <iomanip>  // Formatage des nombres (std::fixed, std::setprecision)

/**
 * 1. METHODE 1 : Recherche "From Scratch" (Algorithme naïf)
 * 
 * Complexité Temporelle (Pire cas) : O(N * M)
 * Complexité Spatiale   (Pire cas) : O(K)
 */
std::vector<size_t> rechercherFromScratch(const std::string& motPrincipal, const std::string& sousMot) {
    std::vector<size_t> positions;


    size_t n = motPrincipal.length();
    size_t m = sousMot.length();

    if (m == 0 || m > n) {
        return positions;
    }

    for (size_t i = 0; i <= n - m; ++i) {
        bool trouve = true;
        for (size_t j = 0; j < m; ++j) {
            if (motPrincipal[i + j] != sousMot[j]) {
                trouve = false;
                break;
            }
        }
        if (trouve) {
            positions.push_back(i);
        }
    }

    return positions;
}

/**
 * 2. METHODE 2 : Recherche avec std::string::find
 * 
 * Complexité Temporelle (Pire cas) : O(N * M)
 * Complexité Spatiale   (Pire cas) : O(K)
 */
std::vector<size_t> rechercherAvecFind(const std::string& motPrincipal, const std::string& sousMot) {
    std::vector<size_t> positions;

    if (sousMot.empty() || sousMot.length() > motPrincipal.length()) {
        return positions;
    }

    size_t pos = motPrincipal.find(sousMot, 0);
    while (pos != std::string::npos) {
        positions.push_back(pos);
        pos = motPrincipal.find(sousMot, pos + 1);
    }

    return positions;
}

/**
 * Remplace toutes les occurrences d'une sous-chaîne par une autre.
 * Exemple : remplacerSousChaine("bonjour monde", "monde", "amis") -> "bonjour amis"
 */
std::string remplacerSousChaine(const std::string& texte, const std::string& ancien, const std::string& nouveau) {
    if (ancien.empty()) {
        return texte;
    }

    std::string resultat;
    size_t debut = 0;

    while (debut <= texte.length()) {
        size_t pos = texte.find(ancien, debut);
        if (pos == std::string::npos) {
            resultat += texte.substr(debut);
            break;
        }

        resultat += texte.substr(debut, pos - debut);
        resultat += nouveau;
        debut = pos + ancien.length();
    }

    return resultat;
}

/**
 * Affiche les résultats de la recherche et le temps d'exécution
 */
void afficherResultats(const std::string& nomMethode, const std::vector<size_t>& positions, double tempsNanosecondes) {
    const double tempsSecondes = tempsNanosecondes / 1'000'000'000.0;

    std::cout << "[" << nomMethode << "] ";

    if (positions.empty()) {
        std::cout << "Le mot n'est PAS inclus.";
    } else {
        std::cout << "Le mot est INCLUS! (" << positions.size() << " occurrence(s))";
    }
    std::cout << " | Temps : " << std::fixed << std::setprecision(6)
              << tempsNanosecondes << " ns (" << tempsSecondes << " s)\n";
}

int main() {
    const std::string nomFichier = "1millions-mot.txt";
    std::filesystem::path cheminFichier = std::filesystem::current_path() / nomFichier;

    if (!std::filesystem::exists(cheminFichier)) {
        std::cerr << "Erreur: Le fichier '" << nomFichier << "' est introuvable dans le dossier courant.\n";
        return 1;
    }

    std::string nomFichierComplet = cheminFichier.string();

    // Ouverture du fichier texte
    std::ifstream fichier(nomFichierComplet);
    if (!fichier.is_open()) {
        std::cerr << "Erreur: Impossible d'ouvrir le fichier '" << nomFichierComplet << "'.\n";
        return 1;
    }

    // Lecture de l'intégralité du contenu du fichier dans la variable 'motPrincipal'
    std::stringstream buffer;
    buffer << fichier.rdbuf();
    std::string motPrincipal = buffer.str();
    fichier.close();

    int choix;
    std::cout << "Que voulez-vous faire ?\n";
    std::cout << "1. Rechercher un mot\n";
    std::cout << "2. Modifier le fichier\n";
    std::cout << "Votre choix : ";
    std::cin >> choix;
    std::cin.ignore();

    if (choix == 1) {
        std::string sousMot;

        // Saisie du mot à rechercher par l'utilisateur
        std::cout << "Mot a rechercher : ";
        std::getline(std::cin, sousMot);

        // Mesure du temps : From Scratch
        auto debutScratch = std::chrono::high_resolution_clock::now();
        std::vector<size_t> resScratch = rechercherFromScratch(motPrincipal, sousMot);
        auto finScratch = std::chrono::high_resolution_clock::now();
        double tempsScratch = std::chrono::duration<double, std::nano>(finScratch - debutScratch).count();
        afficherResultats("From Scratch", resScratch, tempsScratch);

        // Mesure du temps : std::string::find
        auto debutFind = std::chrono::high_resolution_clock::now();
        std::vector<size_t> resFind = rechercherAvecFind(motPrincipal, sousMot);
        auto finFind = std::chrono::high_resolution_clock::now();
        double tempsFind = std::chrono::duration<double, std::nano>(finFind - debutFind).count();
        afficherResultats("std::string::find", resFind, tempsFind);
    }
    else if (choix == 2) {
        std::string ancienMot;
        std::string nouveauMot;

        std::cout << "Mot a remplacer : ";
        std::getline(std::cin, ancienMot);

        std::cout << "Nouveau mot : ";
        std::getline(std::cin, nouveauMot);

        std::string texteModifie = remplacerSousChaine(motPrincipal, ancienMot, nouveauMot);

        std::ofstream fichierSortie(nomFichierComplet, std::ios::trunc);
        if (!fichierSortie.is_open()) {
            std::cerr << "Erreur: Impossible d'ecrire dans le fichier '" << nomFichierComplet << "'.\n";
            return 1;
        }

        fichierSortie << texteModifie;
        fichierSortie.close();

        std::cout << "Modification appliquee dans le fichier '" << nomFichier << "'.\n";
    }
    else {
        std::cerr << "Choix invalide. Veuillez choisir 1 ou 2.\n";
        return 1;
    }

    return 0;
}
