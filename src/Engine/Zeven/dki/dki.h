/*
	Copyright 2012 bitHeads inc.

	This file is part of the BaboViolent 2 source code.

	The BaboViolent 2 source code is free software: you can redistribute it and/or 
	modify it under the terms of the GNU General Public License as published by the 
	Free Software Foundation, either version 3 of the License, or (at your option) 
	any later version.

	The BaboViolent 2 source code is distributed in the hope that it will be useful, 
	but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
	FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along with the 
	BaboViolent 2 source code. If not, see http://www.gnu.org/licenses/.
*/

/// \brief Module de gestion des controles (clavier, souris, manette de jeu, joystick)
///
/// \file dki.h
/// Ce module prend en charge la gestion des �v�nements syst�me relevant de l'�tat de chaque touche du clavier, de la position de la souris, de chaque bouton de la souris et possiblement, la position de chaque axe d'un joystick ou d'une manette de jeu et de chaque boutons d'un joystick ou d'une manette de jeu.
/// Ceci comprend:
/// 	- une fonction d'initialisation du module
/// 	- une fonction de terminaison du module
/// 	- une fonction de mise � jour du module
/// 	- des fonctions permettant d'obtenir l'�tat de chaque touche, boutons, axe ou position d'un clavier, d'un joystick ou d'une souris.
///
/// \note Ce module n�cessite Direct Input pour fonctionner
/// \author David St-Louis (alias Daivuk)
/// \author Louis Poirier (� des fins de documentation seulement)
///


#ifndef DKI_H
#define DKI_H


#ifdef WIN32


#include <windows.h>
#include <dinput.h>
#else
#ifdef CALLBACK
#undef CALLBACK
#endif
#define CALLBACK
#endif

#include "CVector.h"



// Les �tats des touches
/// \name �tats des touches et boutons possibles
/// Dans le cas normal, la s�quence d'�tats pour une touche qui est appuy� pendant un certain temps sera :
/// ..., DKI_NOTHING, DKI_NOTHING,(l'�v�nement d'enfoncement de la touche se produit ici) DKI_DOWN, DKI_HOLD, DKI_HOLD, ..., DKI_HOLD, DKI_HOLD, (l'�v�nement de relachement de la touche se produit ici)DKI_UP, DKI_NOTHING, DKI_NOTHING,...
//@{
/// utilis�e pour d�signer qu'aucune touche n'a encore �t� pes� (voir dkiGetFirstDown())
const int DKI_NOKEY = -1;
/// utilis�e pour d�signer qu'une touche n'est pas appuy�e
const int DKI_NOTHING = 0;
/// utilis�e pour d�signer qu'une touche vient d'�tre enfonc�e. Cet �tat est pr�sent seulement une fois pour toute la dur�e de l'enfoncement d'une touche.
const int DKI_DOWN = 1;
/// utilis�e pour d�signer qu'une touche est maintenue enfonc�e. Cet �tat est pr�sent tant que la touche reste enfonc�.
const int DKI_HOLD = 2;
/// utilis�e pour d�signer qu'une touche vient d'�tre relach�e. Cet �tat est pr�sent seulement une fois d�s que la touche a �t� relach�e.
const int DKI_UP = 3;
/// non utilis�
const int DKI_CLIC = 3;
/// non utilis�
const int DKI_DBL_CLIC = 4;
//@}


// Les boutons de la mouse
/// \name Constantes d�signants les diff�rents boutons d'une souris
//@{
const int DKI_MOUSE_BUTTON1 = 256;
const int DKI_MOUSE_BUTTON2 = 257;
const int DKI_MOUSE_BUTTON3 = 258;
const int DKI_MOUSE_BUTTON4 = 259;
const int DKI_MOUSE_BUTTON5 = 260;
const int DKI_MOUSE_BUTTON6 = 261;
const int DKI_MOUSE_BUTTON7 = 262;
const int DKI_MOUSE_BUTTON8 = 263;
//@}


// Les boutons du joystick
/// \name Constante d�signant les diff�rents boutons d'un joystick
/// Il y a une limite maximum de 128 buttons d�tectables. Pour atteindre le n i�me bouton, on n'a qu'� faire : DKI_JOY_BUTTON1 + n
//@{
const int DKI_JOY_BUTTON1 = 264; // 128 Buttons. DKI_JOY_BUTTON1 + n
//@}


// Les fonction du DKI

/// \brief retourne la constante d�signant la premi�re touche ayant �t� appuy�e
///
/// Cette fonction permet de connaitre la touche ou le bouton qui a �t� appuy� en premier. La fonction effectue la v�rification pour un instant seulement (lors de l'appel). Un appel pour chaque cycle d'ex�cution est donc n�cessaire afin de savoir si une touche a �t� pes� dans un certain intervalle de temps.
///
/// \return l'index repr�sentant la touche ou le bouton qui a �t� appuy� en premier
int				dkiGetFirstDown();


/// \brief retourne la vitesse � laquelle la roulette de la souris est d�plac�e
///
/// Cette fonction retourne la vitesse � laquelle la roulette de la souris est d�plac�e en nombre de clic
/// -2 signifirait que la wheel a �t� d�cendu 2 fois. (normalement on s'en tien � -1,0 ou 1)
/// Exemple d'utilisation : if (dkiGetMouseWheelVel() < 0) zoomOut();
///
/// \return retourne la vitesse � laquelle la roulette de la souris est d�plac�e
int				dkiGetMouseWheelVel();



/// \brief retourne la position actuelle de la souris
///
/// Cette fonction retourne la position actuelle de la souris. Cette position est en pixel et l'origine est le coin sup�rieur gauche de l'�cran.
///
/// \return retourne la position actuelle de la souris en pixel
CVector2i		dkiGetMouse();



/// \brief retourne la vitesse � laquelle se d�place la souris
///
/// Cette fonction retourne le d�placement effectu� par la souris en pixels depuis le dernier appel a dkiUpdate().
/// Le d�placement retourn� n'est pas d�pendant de la grandeur de la fen�tre (il y aura un certain d�placement retourn� m�me si on d�place la souris vers la gauche et que la derni�re position �tait (0,0).
/// Parfait pour les jeux de style FPS
///
/// \return retourne la vitesse � laquelle se d�place la souris
CVector2i		dkiGetMouseVel();



/// \brief retourne l'�tat d'une touche ou d'un bouton
///
/// Cette fonction retourne l'�tat d'une touche ou d'un bouton.
///
/// \param inputID identifiant unique de la touche ou du bouton
/// \return �tat de la touche ou du bouton
int				dkiGetState(int inputID);



/// \brief retourne la position de chaque axe d'un joystick ou d'une manette de jeu
///
/// Cette fonction retourne la position de chaque axe d'un joystick ou d'une manette de jeu. La position au repos �tant 0 et les extr�mes �tant -1 et 1.
/// Aucune "dead zone" et courbe de progression est d�fini ici. C'est au client de le faire.
///
/// \return position de chaque axe
CVector3f		dkiGetJoy();
CVector3f		dkiGetJoyR();



/// \brief retourne la vitesse � laquelle se d�place chacun des axes d'un joystick ou d'une manette de jeu
///
/// Cette fonction retourne la vitesse � laquelle se d�place chacun des axes d'un joystick ou d'une manette de jeu par rapport au dernier appel � dkiUpdate().
/// Si la derni�re position d'un axe �tait de -1 et que sa position est � 1 lors de l'appel, la valeur 2 sera retourn� pour cet axe.
///
/// \return vitesse de chaque axe
CVector3f		dkiGetJoyVel();



/// \brief initialise le module pour usage
///
/// Cette fonction initialise le module afin que son utilisation puisse d�buter. Cette fonction DOIT �tre appel� avant tout autres appels � des fonctions de ce module.
///
/// \param appInstance pointeur vers l'instance de l'application
/// \param appHandle pointeur vers le lien logique de l'application
/// \return 0 s'il y a un �chec, 1 si tout s'est bien pass�
int				dkiInit(HINSTANCE appInstance, HWND appHandle);



/// \brief termine l'utilisation du module
///
/// Cette fonction lib�re le syst�me des ressources allou�es qui �taient n�cessaires au fonctionnement du module et termine sont utilisation. Cette fonction de doit pas �tre appel� plusieurs fois de suite : pour chaque appel � cette fonction, un appel � dkiInit() correspondant doit pr�c�demment avoir �t� fait.
///
void			dkiShutDown();



/// \brief mise � jour des �tats des p�riph�riques d'entr�es
///
/// Cette fonction effectue la mise � jour des �tats des p�riph�riques d'entr�es (clavier, souris, joystick). Elle doit �tre appel�e une fois par cycle d'ex�cution.
/// On doit sp�cifier la dimension de la fen�tre dans laquelle le pointeur de la souris ne pourra pas exc�der.
///
/// \param elapsef non utilis�
/// \param width dimension en pixel de la fen�tre
/// \param height dimension en pixel de la fen�tre
void			dkiUpdate(float elapsef, int width, int height);


// Setter la position du cursor
void			dkiSetMouse(CVector2i & mousePos);




#endif
