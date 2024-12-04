# Projet Integrateur 1 INF1900-A24

## Auteurs

- **Anis Boukerdoune**
- **Buntep Chea**
- **Emmanuel Berberi**
- **Fadi Mousa**

## Table des Matières

- [Description](#description)
- [Modes de fonctionnement](#Modes-de-fonctionnement)
- [Composants materiels ](#hardware-components)
- [Instruction de compilation et d'installation](#make)

## Description

Ce projet consiste à programmer un robot capable de réaliser plusieurs tâches sur un parcours prédéfini, avec des capacités d’adaptation aux changements d’environnement et des modes de fonctionnement distincts :

Chercher les extrémités du parcours.
Traverser le parcours en évitant les obstacles et en suivant un chemin optimisé.
Afficher un rapport détaillé des tâches effectuées.
Le parcours est représenté par un ruban noir avec des intersections numérotées et des points spécifiques (C, D, E). Chaque intersection a des poids spécifiques associés pour guider le robot.

## Modes de Fonctionnement

- **Sélection du Mode :**
  -Le robot démarre avec une séquence d’éclairage LED permettant de choisir un mode parmi les trois proposés :
  Vert : Chercher une extrémité.
  Rouge : Traverser le parcours.
  Ambre : Afficher un rapport.
  L'évaluateur sélectionne un mode en

appuyant sur le bouton Reset lorsque la LED affiche la couleur correspondant au mode désiré. Une fois le mode sélectionné, le robot clignote pour confirmer et démarre son exécution.

1. **Chercher une Extrémité :**
   -Le robot débute sa recherche à partir d'un point central (A ou B).
   -Il navigue jusqu’à atteindre une extrémité (C, D ou E), où il s'arrête, émet un signal sonore, et affiche une couleur LED correspondant à l'extrémité atteinte.

2. **Traverser le Parcours :**
   -Le robot suit un chemin optimisé (le plus court possible) pour atteindre un poteau placé par l’évaluateur.
   -Il répète cette opération pour trois paires de points différentes, en évitant les obstacles.

3. **Afficher un Rapport :**
   -Le robot transmet les informations (points visités, orientations, etc.) via USB à un ordinateur, permettant une analyse complète.

## Composants Matériels

- **Capteur de Suivi de Ligne (Cytron Maker Line):**

  - Connecte aux PORTC (2:7)

- **Capteur de Distance (Sharp GP2D120):**
  -Permet de détecter les obstacles, connecté à PA4.

- **Piezo:**

  - Genere le song , connecte aux pins PD7

- **LEDs:**

  - Connecte to ports PA0 and PA1

- **Roues et Moteurs:**
  - Connectés aux ports PD2,PD3,PD4 et PD5

### Bouttons

- **Reset Button:**

  - Selectionne un mode .

- **Interrupt:**
  - Déclenche ou redémarre le robot.

## Instruction de compilation et d'installation

Avant de commencer, assurez-vous de compiler la bibliothèque dans le dossier projet/lib en exécutant la commande make dans le terminal.
Retournez ensuite dans le dossier projet/app et exécutez la commande make install pour installer le code sur le robot. Assurez-vous que le robot est connecté à l'ordinateur via un câble USB.
