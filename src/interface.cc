/*
** Stechec project is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** The complete GNU General Public Licence Notice can be found as the
** `NOTICE' file in the root directory.
**
** Copyright (C) 2014 Prologin
*/

#include "api.hh"

#include <iostream>
#include <sstream>
#include <vector>

// from api.cc
extern Api* api;

template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& arr)
{
  os << "[";
  typename std::vector<T>::const_iterator it;
  for (it = arr.begin(); it != arr.end(); ++it)
  {
    if (it != arr.begin())
      os << ", ";
    os << *it;
  }
  os << "]";
  return os;
}


// todo avoir un ostringstream a la place de std::string

std::string convert_to_string(int i){
  std::ostringstream s;
  s << i;
  std::string result = s.str();
  return result;
}
std::string convert_to_string(std::string i){
  return i;
}
std::string convert_to_string(bool i){
  return i?"true":"false";
}
std::string convert_to_string(std::vector<int> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(case_info in){
  switch (in)
  {
    case CASE_SIMPLE: return "\"case_simple\"";
    case CASE_TOURELLE: return "\"case_tourelle\"";
    case CASE_CASE: return "\"case_case\"";
    case CASE_FONTAINE: return "\"case_fontaine\"";
    case CASE_ARTEFACT: return "\"case_artefact\"";
    case CASE_ERREUR: return "\"case_erreur\"";
  }
  return "bad value";
}
std::string convert_to_string(std::vector<case_info> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(erreur in){
  switch (in)
  {
    case OK: return "\"ok\"";
    case ANNULER_IMPOSSIBLE: return "\"annuler_impossible\"";
    case CASE_IMPOSSIBLE: return "\"case_impossible\"";
    case CASE_ADVERSE: return "\"case_adverse\"";
    case CASE_UTILISEE: return "\"case_utilisee\"";
    case CASE_VIDE: return "\"case_vide\"";
    case VALEUR_INVALIDE: return "\"valeur_invalide\"";
    case MAGIE_INSUFFISANTE: return "\"magie_insuffisante\"";
    case SORCIERS_INSUFFISANTS: return "\"sorciers_insuffisants\"";
    case ATTAQUE_INSUFFISANTE: return "\"attaque_insuffisante\"";
    case PHASE_INCORRECTE: return "\"phase_incorrecte\"";
  }
  return "bad value";
}
std::string convert_to_string(std::vector<erreur> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(position in){
  std::string x = convert_to_string(in.x);
  std::string y = convert_to_string(in.y);
  std::string out = "{";
  out += "x:" + x;
  out += ", ";
  out += "y:" + y;
  return out + "}";
}

std::string convert_to_string(std::vector<position> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
std::string convert_to_string(tourelle in){
  std::string pos = convert_to_string(in.pos);
  std::string portee = convert_to_string(in.portee);
  std::string joueur = convert_to_string(in.joueur);
  std::string vie = convert_to_string(in.vie);
  std::string attaque = convert_to_string(in.attaque);
  std::string out = "{";
  out += "pos:" + pos;
  out += ", ";
  out += "portee:" + portee;
  out += ", ";
  out += "joueur:" + joueur;
  out += ", ";
  out += "vie:" + vie;
  out += ", ";
  out += "attaque:" + attaque;
  return out + "}";
}

std::string convert_to_string(std::vector<tourelle> in){
  if (in.size()){
    std::string s = "[" + convert_to_string(in[0]);
    for (int i = 1, l = in.size(); i < l; i++){
      s = s + ", " + convert_to_string(in[i]);
    }
    return s + "]";
  }else{
    return "[]";
  }
}
///
// Retourne le type de la case à l'emplacement `pos`
//
extern "C" case_info api_info_case(position pos)
{
  return api->info_case(pos);
}

///
// Retourne la liste des tourelles qui appartiennent au joueur ``joueur``
//
extern "C" std::vector<tourelle> api_tourelles_joueur(int joueur)
{
  return api->tourelles_joueur(joueur);
}

///
// Retourne la magie que possède le joueur ``joueur``
//
extern "C" int api_magie(int joueur)
{
  return api->magie(joueur);
}

///
// Retourne le nombre de sorciers du joueur ``joueur`` sur la case ``pos``
//
extern "C" int api_nb_sorciers(position pos, int joueur)
{
  return api->nb_sorciers(pos, joueur);
}

///
// Retourne le nombre de sorciers du joueur ``joueur`` déplacables sur la case ``pos``
//
extern "C" int api_nb_sorciers_deplacables(position pos, int joueur)
{
  return api->nb_sorciers_deplacables(pos, joueur);
}

///
// Retourne le numéro du joueur qui contrôle la case ``pos``
//
extern "C" int api_joueur_case(position pos)
{
  return api->joueur_case(pos);
}

///
// Retourne la tourelle située sur la case ``pos``
//
extern "C" tourelle api_tourelle_case(position pos)
{
  return api->tourelle_case(pos);
}

///
// Retourne la position de la base du joueur ``joueur``
//
extern "C" position api_base_joueur(int joueur)
{
  return api->base_joueur(joueur);
}

///
// Retourne vrai si l'on peut construire sur la case ``pos``
//
extern "C" bool api_constructible(position pos, int joueur)
{
  return api->constructible(pos, joueur);
}

///
// Retourne la liste des positions constituant le plus court chemin allant de la case ``pos1`` à la case ``pos2``. Attention : Cette fonction est lente.
//
extern "C" std::vector<position> api_chemin(position pos1, position pos2)
{
  return api->chemin(pos1, pos2);
}

///
// Construire une tourelle à la position ``pos``
//
extern "C" erreur api_construire(position pos, int portee)
{
  return api->construire(pos, portee);
}

///
// Supprimer une tourelle à la position ``pos``
//
extern "C" erreur api_supprimer(position pos)
{
  return api->supprimer(pos);
}

///
// Tirer avec ``pts`` points de dégats depuis la tourelles ``tourelle`` sur la position ``cible``
//
extern "C" erreur api_tirer(int pts, position tourelle, position cible)
{
  return api->tirer(pts, tourelle, cible);
}

///
// Créer ``nb`` sorciers dans la base
//
extern "C" erreur api_creer(int nb)
{
  return api->creer(nb);
}

///
// Déplace ``nb`` sorciers de la position ``depart`` jusqu'à la position ``arrivee``.
//
extern "C" erreur api_deplacer(position depart, position arrivee, int nb)
{
  return api->deplacer(depart, arrivee, nb);
}

///
// Attaquer la tourelle à la position ``cible`` depuis la position ``pos``
//
extern "C" erreur api_assieger(position pos, position cible)
{
  return api->assieger(pos, cible);
}

///
// Retourne le numéro de votre joueur
//
extern "C" int api_moi()
{
  return api->moi();
}

///
// Retourne la liste des numéros de vos adversaires
//
extern "C" std::vector<int> api_adversaires()
{
  return api->adversaires();
}

///
// Retourne le numéro du tour actuel
//
extern "C" int api_tour_actuel()
{
  return api->tour_actuel();
}

///
// Retourne la distance entre deux positions
//
extern "C" int api_distance(position depart, position arrivee)
{
  return api->distance(depart, arrivee);
}

///
// Annule la dernière action
//
extern "C" erreur api_annuler()
{
  return api->annuler();
}

///
// Affiche le contenu d'une valeur de type case_info
//
std::ostream& operator<<(std::ostream& os, case_info v)
{
  switch (v) {
  case CASE_SIMPLE: os << "CASE_SIMPLE"; break;
  case CASE_TOURELLE: os << "CASE_TOURELLE"; break;
  case CASE_CASE: os << "CASE_CASE"; break;
  case CASE_FONTAINE: os << "CASE_FONTAINE"; break;
  case CASE_ARTEFACT: os << "CASE_ARTEFACT"; break;
  case CASE_ERREUR: os << "CASE_ERREUR"; break;
  }
  return os;
}
extern "C" void api_afficher_case_info(case_info v)
{
  std::cerr << v << std::endl;
}

///
// Affiche le contenu d'une valeur de type erreur
//
std::ostream& operator<<(std::ostream& os, erreur v)
{
  switch (v) {
  case OK: os << "OK"; break;
  case ANNULER_IMPOSSIBLE: os << "ANNULER_IMPOSSIBLE"; break;
  case CASE_IMPOSSIBLE: os << "CASE_IMPOSSIBLE"; break;
  case CASE_ADVERSE: os << "CASE_ADVERSE"; break;
  case CASE_UTILISEE: os << "CASE_UTILISEE"; break;
  case CASE_VIDE: os << "CASE_VIDE"; break;
  case VALEUR_INVALIDE: os << "VALEUR_INVALIDE"; break;
  case MAGIE_INSUFFISANTE: os << "MAGIE_INSUFFISANTE"; break;
  case SORCIERS_INSUFFISANTS: os << "SORCIERS_INSUFFISANTS"; break;
  case ATTAQUE_INSUFFISANTE: os << "ATTAQUE_INSUFFISANTE"; break;
  case PHASE_INCORRECTE: os << "PHASE_INCORRECTE"; break;
  }
  return os;
}
extern "C" void api_afficher_erreur(erreur v)
{
  std::cerr << v << std::endl;
}

///
// Affiche le contenu d'une valeur de type position
//
std::ostream& operator<<(std::ostream& os, position v)
{
  os << "{ ";
  os << "x" << "=" << v.x;
  os << ", ";
  os << "y" << "=" << v.y;
  os << " }";
  return os;
}
extern "C" void api_afficher_position(position v)
{
  std::cerr << v << std::endl;
}

///
// Affiche le contenu d'une valeur de type tourelle
//
std::ostream& operator<<(std::ostream& os, tourelle v)
{
  os << "{ ";
  os << "pos" << "=" << v.pos;
  os << ", ";
  os << "portee" << "=" << v.portee;
  os << ", ";
  os << "joueur" << "=" << v.joueur;
  os << ", ";
  os << "vie" << "=" << v.vie;
  os << ", ";
  os << "attaque" << "=" << v.attaque;
  os << " }";
  return os;
}
extern "C" void api_afficher_tourelle(tourelle v)
{
  std::cerr << v << std::endl;
}

