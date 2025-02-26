# 📅 Gestionnaire d'Agenda

Une application de gestion d'agenda en console développée en C++.

## 🛠️ Compilation

La compilation a été réalisée sous Linux (Ubuntu 22.04.1 LTS 64 bits).

### Compilation rapide

```bash
# Compiler avec Make
make

# Nettoyer les fichiers objets et l'exécutable
make clean
```

### Étapes détaillées de compilation

1. **Préprocessing, compilation et assemblage du fichier principal**:
   ```bash
   g++ -Wall -std=c++17 -c main.cpp
   ```

2. **Compilation des fichiers sources dans le dossier includes**:
   ```bash
   # Compilation de agenda.cpp
   g++ -Wall -std=c++17 -c includes/agenda.cpp
   
   # Compilation de event.cpp
   g++ -Wall -std=c++17 -c includes/event.cpp
   
   # Compilation de date.cpp
   g++ -Wall -std=c++17 -c includes/date.cpp
   ```

3. **Édition de liens pour créer l'exécutable**:
   ```bash
   g++ -Wall -std=c++17 -o main main.o agenda.o event.o date.o
   ```

## 🧪 Tests automatisés

Les tests peuvent être exécutés avec le fichier `test.exp` via l'outil expect:

```bash
expect test.exp
```

### Scénario de test

1. **Création d'agendas**:
   * Un agenda contenant les jours fériés de 2023
   * Un agenda contenant les week-ends de 3 jours ou plus en 2023
   * Un agenda contenant les vacances scolaires de l'IUT d'Arles 2022-2023
   * Un agenda contenant des rendez-vous divers durant l'année 2023

2. **Export HTML**:
   * Chaque agenda est chargé puis exporté au format HTML

3. **Test de manipulation**:
   * Création d'un agenda de test
   * Vérification de l'affichage avec "aucun événement"
   * Création d'un événement et affichage
   * Suppression de l'événement et vérification
   * Fermeture de l'agenda

4. **Test de suppression**:
   * Suppression du premier agenda de la liste

5. **Fin du programme**

## 🚀 Utilisation du programme

Lancez le programme via la commande:

```bash
./main
```

### 📁 Structure des dossiers et fichiers

* **Agendas**: Stockés dans le dossier `agenda`
  * Chaque agenda est sauvegardé dans un fichier `idagenda.txt`
  * Chaque fichier contient les informations de l'agenda et les IDs de ses événements

* **Événements**: Stockés dans le dossier `agenda/events`
  * Chaque événement est dans un fichier `idévénement.txt`
  * Chaque fichier contient uniquement les informations de l'événement

* **Exports HTML**: Stockés dans le dossier `agenda/html`
  * Chaque export est nommé `titreagenda_idagenda.html`

## 📋 Fonctionnalités

### Menu Principal

* **1️⃣ Charger un agenda**
  * Sélection parmi la liste des agendas disponibles
  * Redirection vers le menu de l'agenda
  * Si aucun agenda n'est trouvé, proposition d'en créer un

* **2️⃣ Créer un agenda vide**
  * Saisie du titre et de la description
  * Redirection vers le menu de l'agenda

* **3️⃣ Supprimer un agenda**
  * Sélection parmi la liste des agendas
  * Confirmation de suppression
  * Si aucun agenda n'est trouvé, proposition d'en créer un

* **🅀 Quitter l'application**
  * Fermeture du programme

### Menu de l'Agenda

* **1️⃣ Afficher l'agenda**
  * Affichage des informations sur l'agenda
  * Liste détaillée des événements
  * Message "aucun événement" si l'agenda est vide

* **2️⃣ Exporter l'agenda au format HTML**
  * Création d'un fichier HTML avec les informations de l'agenda
  * Tableau des événements détaillés

* **3️⃣ Créer un événement**
  * Saisie du titre et de la description
  * Saisie de la date/heure de début et de fin

* **4️⃣ Supprimer un événement**
  * Saisie du titre
  * Affichage d'une liste détaillée des événements correspondants
  * Sélection et confirmation de suppression

* **🅀 Fermer l'agenda**
  * Option de sauvegarde des modifications
  * Retour au menu principal