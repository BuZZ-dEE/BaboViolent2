/// \brief Module de gestion des sons
///
/// \file dks.h
/// Ce module prend en charge la gestion des sons.
/// Ceci comprend :
/// 	- une fonction de chargement d'un son
/// 	- une fonction de destruction d'un son
/// 	- une fonction de destruction de tout les sons pr�sentement charg�es et qui termine l'utilisation du module
/// 	- une fonction d'initialisation du module
///
/// Se r�f�rer � la documentation de FMOD pour connaitre les fonctions disponibles pour faire jouer les sons et musiques charg�es.
///
/// \author David St-Louis (alias Daivuk)
/// \author Louis Poirier (� des fins de documentation seulement)
///


#ifndef DKS_H
#define DKS_H


#pragma comment( lib, "dks.lib" )


// Link
#pragma comment( lib, "fmodvc.lib" )
#pragma comment( lib, "fmodlcc.lib" )


// Les includes pour Fmod
#include <fmod.h>
#include "CVector.h"


// Les fonction du DKS


/// \brief initialise le module et FMOD
/// 
/// Cette fonction effectue l'initialisation du module et de FMOD. Ceci est fait en sp�cifiant un mixrate et un nombre maximal de cannaux audio. Cette onction DOIT �tre appel�e avant tout autres appels � d'autres fonctions de ce module.
///
/// \param mixrate mixrate
/// \param maxsoftwarechannels nombre de cannaux audio
/// \return true si l'initialisation a r�ussi, false sinon
bool			dksInit(int mixrate, int maxsoftwarechannels);



/// \brief lib�re toute la m�moire allou�e pour les sons et termine l'utilisation du module.
///
/// Cette fonction lib�re toute la m�moire allou�e pour les sons et termine l'utilisation du module. Le module pourra �tre red�marr� avec un nouvel appel � dksInit()
///
void			dksShutDown();



/// \brief chargement d'un son en m�moire
///
/// Cette fonction effectue le chargement d'un son ou d'une musique en m�moire. Les formats valides sont les m�me que FMOD. Avant de cr�er un nouveau son, le module v�rifie si ce fichier a d�j� �t� charg�. Si c'est le cas, aucun son ne sera cr�� et le pointeur FMOD correspondant � ce fichier sera retourn�.
///
/// \param filename chemin menant au fichier son ou musique � charger depuis l'endroit o� se situe le fichier EXE du programme.
/// \param loop est-ce que ce son ou cette musique doit jouer en boucle?
/// \return pointeur FMOD vers le son ou la musique charg�e en m�moire
FSOUND_SAMPLE*	dksCreateSoundFromFile(char* filename, bool loop=false);



/// \brief destruction d'un son charg� en m�moire
///
/// Cette fonction permet de lib�rer la m�moire allou�e pour un son pr�sentement charg�e en m�moire.
///
/// \param pointeur FMOD du son � effacer
void			dksDeleteSound(FSOUND_SAMPLE* fsound_sample);


// Ajou de fonctions
int			dksPlaySound(FSOUND_SAMPLE * fsound_sample, int channel, int volume=255);
void			dksPlay3DSound(FSOUND_SAMPLE * fsound_sample, int channel, float range, CVector3f & position, int volume=255);
void			dksPlayMusic(char* filename, int channel=-1, int volume=255);
void			dksStopMusic();


#endif