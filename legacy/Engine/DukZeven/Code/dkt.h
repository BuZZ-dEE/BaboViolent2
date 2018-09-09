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

/// \brief Module de gestion de textures
///
/// \file dkt.h
/// Ce module prend en charge la gestion de textures (une texture �tant simplement une image charg�e en m�moire).
/// Ceci comprend :
/// 	- le chargement en m�moire d'un fichier targa (fichier avec extension TGA)
/// 	- la cr�ation en m�moire d'une texure
/// 	- la d�sallocation de la m�moire allou�e pour une texture
/// 	- certaines fonctions de manipulations d'une texture
/// 	- une fonction de mise � jour des textures
/// 	- certaines fonctions permettant d'obtenir de l'information sur une texture
/// 	- une fonction permettant de lib�rer toute la m�moire allou�e par des chargements de textures
///
/// Deux groupe de constantes sont disponibles pour sp�cifier les diff�rents filtre commun de texturage et pour repr�senter les valeurs possibles du nombre de byte par pixel.
/// Un fichier TGA non compress� contient les informations suivantes:
/// 	- un en-t�te de 12 octets (plutot inutile pour nous)
/// 	- les dimensions de l'images (2 octets pour la longueur et 2 autres pour la hauteur)
/// 	- le nombre de bit par pixel (24 ou 32, RGB ou RGBA)
/// 	- le data de l'image ((hauteur*largeur*nombre de bit par pixel/8) octets)
///
/// Les fichiers TGA valides pour ce module doivent �tre non compress� et ayant comme dimensions des puissances de 2 variant de 16 � 512.
///
/// \author David St-Louis (alias Daivuk)
/// \author Louis Poirier (� des fins de documentation seulement)
///

#ifndef DKT_H
#define DKT_H


#include "CVector.h"


/// \name Filtering
/// Les drapeaux repr�sentants les diff�rents filtres communs de texturage qui d�termineront la facon dont une texture affich�e en 3D sera transpos�e en pixel d'une certaine couleur.
/// Avant d'aller plus loin, je me doit d'abord expliquer ce qu'est un Mipmap. Un Mipmap est un assortiment d'images toutes g�n�r�es � partir d'une image source. L'assortiment est simplement l'ensemble des diff�rentes r�solutions de l'image de base trouv�es en divisant toujours la r�solution de l'image source par 2.
/// Par exemple, le Mipmap d'une image source de 64X128 sera l'ensemble : 64X128, 32X64, 16X32, 8X16, 4X8, 2X4, 1X2 et 1X1 (l'image source sera 'stretcher' pour s'ajuster � chaque r�solution). En utilisant cet ensemble d'image plutot que toujours l'image source, on se retrouve � avoir un gain important de performance plus l'image � �tre afficher se trouve loin en 3D. Le seul cout �tant la m�moire n�cessaire pour conserver le Mipmap entier moins l'image source.
/// 
/// Il existe 2 cas dans lequel un certain calcul est n�cessaire pour d�terminer la couleur qu'un certain pixel aura avant d'�tre afficher:
/// 	-# le pixel couvre une r�gion �gale ou plus petite qu'un texel.
/// 	-# le pixel couvre une plus grande r�gion qu'un texel (un texel est un �l�ment de texture)
///
/// OpenGL a d�fini 6 facons de calculer(ou approximer) la couleur que le pixel aura avant d'�tre afficher:
/// 	- GL_NEAREST : utilise le texel de l'image source le plus pr�s du centre du pixel � �tre textur� (valide pour les 2 cas)
/// 	- GL_LINEAR : utilise la moyenne de la couleur des 4 texels les plus pr�s du centre du pixel � �tre textur� (valide pour les 2 cas)
/// 	- GL_NEAREST_MIPMAP_NEAREST : utilise l'image du Mipmap ayant la taille qui se rapproche le plus de la taille du pixel et applique le crit�re GL_NEAREST avec cette image (valide seulement dans le cas 2)
/// 	- GL_LINEAR_MIPMAP_NEAREST : utilise l'image du Mipmap ayant la taille qui se rapproche le plus de la taille du pixel et applique le crit�re GL_LINEAR avec cette image (valide seulement dans le cas 2)
/// 	- GL_NEAREST_MIPMAP_LINEAR : utilise les 2 images du Mipmap ayant les tailles qui se rapprochent le plus de la taille du pixel et applique le crit�re GL_NEAREST avec l'image r�sultante de la moyenne des couleur des 2 images du Mipmap choisies (valide seulement dans le cas 2)
/// 	- GL_LINEAR_MIPMAP_LINEAR : utilise les 2 images du Mipmap ayant les tailles qui se rapprochent le plus de la taille du pixel et applique le crit�re GL_LINEAR avec l'image r�sultante de la moyenne des couleur des 2 images du Mipmap choisies (valide seulement dans le cas 2)
///
/// On peut noter que les Mipmap ne sont utiles que dans le cas 2.
/// De ces principes, il r�sulte 4 configurations les plus fr�quemments utilis�s:
/// 	- DKT_FILTER_NEAREST
/// 		- cas 1 : GL_NEAREST
/// 		- cas 2 : GL_NEAREST
/// 	- DKT_FILTER_LINEAR
/// 		- cas 1 : GL_LINEAR
/// 		- cas 2 : GL_LINEAR
/// 	- DKT_FILTER_BILINEAR
/// 		- cas 1 : GL_LINEAR
/// 		- cas 2 : GL_LINEAR_MIPMAP_NEAREST
/// 	- DKT_FILTER_TRILINEAR
/// 		- cas 1 : GL_LINEAR
/// 		- cas 2 : GL_LINEAR_MIPMAP_LINEAR
/// 	- par d�faut (si aucun sp�cifier)
/// 		- cas 1 : GL_NEAREST
/// 		- cas 2 : GL_NEAREST_MIPMAP_NEAREST
///
/// Ce proc�d� est d�termin� lors de l'application d'une texture sur une face d'un mod�le ou lors de la cr�ation d'une texture � partir d'une autre. On peut donc changer de filtre tant qu'on veut avant le rendu sur l'�cran ou sur un tampon m�moire.
/// Ces drapeaux peuvent �tre utilis� pour chaque fonctions du module qui contient un param�tre nomm� filter. La combinaison de plusieurs drapeaux n'est pas permise.
//@{
const int DKT_FILTER_NEAREST = 0;
const int DKT_FILTER_LINEAR = 1;
const int DKT_FILTER_BILINEAR = 2;
const int DKT_FILTER_TRILINEAR = 3;
//@}

/// \name BytePerPixel
/// Ces drapeaux font r�f�rence � certains formats internes de pixel qu'OpenGL utilise. Ils doivent �tre utilis� pour chaque fonctions du module qui contient un param�tre nomm� bpp ou internalFormat.
//@{
/// 1 byte par pixel qui peut repr�senter 256 tons de gris pour chaque pixel
const int DKT_LUMINANCE = 1;
/// 3 byte par pixel qui peuvent repr�senter 256 tons de rouge, de vert et de bleu pour chaque pixel
const int DKT_RGB = 3;
/// 4 byte par pixel qui peuvent repr�senter 256 tons de rouge, de vert, de bleu et de transparence pour chaque pixel
const int DKT_RGBA = 4;
//@}


// Les fonction du DKT


/// \brief effectue une effet de bluring sur une texture d�j� charg�e en m�moire
///
/// Cette fonction permet de modifier une texture charg�e en m�moire en prenant la moyenne des 8 pixels adjacents pour chaque pixel de l'image. Le nombre d'it�ration est d�termin� par nbPass.
///
/// \param textureID identifiant unique de la texture
/// \param nbPass nombre d'it�ration de l'effet de bluring
void			 dktBlurTexture(unsigned int textureID, int nbPass);



/// \brief permet de changer le filtre de texturage utilis�e pour toute les textures pr�sentement charg�es en m�moire
///
/// Cette fonction permet de changer le filtre de texturage utilis�e pour toute les textures pr�sentement charg�es en m�moire.
/// Le filtre final qui sera utilis� lors du rendu sera celui sp�cifi� par le dernier appel fait � cette fonction (sauf si d'autre fonctions modifie aussi le filtre).
///
/// \param filter filtre de texturage � �tre utiliser pour toute les textures
void			 dktChangeFilter(int filter);



/// \brief cr�e une texture blanche
///
/// Cette texture permet de cr�� une texture vierge ((255,255,255,255) ou (255,255,255) ou (255) pour chaque pixel). La texture cr�� aura les dimension wXh, aura bpp byte par pixel et utilisera le filtre de texturage filter.
///
/// \param w largeur en pixel de la texture � cr�er
/// \param h hauteur en pixel de la texture � cr�er
/// \param bpp drapeau du format de pixel � �tre utilis�
/// \param filter drapeau de filtre de texturage � �tre utiliser
/// \return identifiant unique de la texture cr��e
unsigned int	 dktCreateEmptyTexture(int w, int h, int bpp, int filter);



/// \brief remplace une texture existante par une autre
///
/// Cette fonction permet de remplacer une texture existante par le contenu d'un certain tableau qui contient une liste de pixel d�fini proprement (de dimension wXh et de bpp byte par pixel). Un filtre de texturage peut aussi �tre sp�cifi�.
///
/// \param textureID identifiant unique de la texture existante dont le contenu sera remplac�
/// \param buffer pointeur vers la liste des pixels qui d�fini une images de dimension wXh et de bpp byte par pixel
/// \param w largeur en pixel de la texture � cr�er
/// \param h  hauteur en pixel de la texture � cr�er
/// \param bpp drapeau du format de pixel � �tre utilis�
/// \param filter drapeau de filtre de texturage � �tre utiliser
void			 dktCreateTextureFromBuffer(unsigned int *textureID, unsigned char *buffer, int w, int h, int bpp, int filter);



/// \brief cr�e une texture � partir d'un fichier targa (TGA)
///
/// Cette fonction permet de cr�er une texture � partir d'un fichier targa (TGA) en utilisant un filtre de texturage.
///
/// \param filename chemin menant au fichier TGA � charger depuis l'endroit o� se situe le fichier EXE du programme.
/// \param filter drapeau de filtre de texturage � �tre utiliser
/// \return identifiant unique de la texture cr��e
unsigned int	 dktCreateTextureFromFile(char *filename, int filter);



/// \brief lib�re la m�moire allou�e pour une texture
///
/// Cette fonction lib�re la m�moire allou�e pour une texture charg� pr�c�demment.
///
/// \param textureID identifiant unique de la texture � �tre effac�
void			 dktDeleteTexture(unsigned int *textureID);



/// \brief obtient le nombre de byte par pixel d'une texture
///
/// Cette fonction retourne le nombre de byte par pixel d'une texture d�j� charg�e en m�moire.
///
/// \param textureID identifiant unique de la texture
/// \return le nombre de byte par pixel de la texture
int				 dktGetTextureBytePerPixel(unsigned int textureID);



/// \brief obtient la description de la derni�re erreur encourue par ce module
///
/// Cette fonction retourne la description de la derni�re erreur encourue par ce module
///
/// \return description de la derni�re erreur encourue par ce module
char*			 dktGetLastError();



/// \brief obtient le tableau des pixels qui d�finissent une texture d�j� charg�e
///
/// Cette fonction permet d'obtenir le tableau des pixels qui d�finissent une texture d�j� charg�e.
///
/// \param textureID identifiant unique de la texture cible
/// \param data pointeur qui contiendra l'adresse du tableau des pixels de la texture cible
void			 dktGetTextureData(unsigned int textureID, unsigned char * data);



/// \brief obtient les dimensions d'une texture d�j� charg�e
///
/// Cette fonction permet d'obtenir les dimensions d'une texture d�j� charg�e.
///
/// \param textureID identifiant unique de la texture cible
/// \return dimension de la texture cible
CVector2i		 dktGetTextureSize(unsigned int textureID);



/// n'est plus utilis�
void			 dktInit();



/// \brief remplace une texture existante par le contenu d'une portion du framebuffer (la derni�re image affich�e � l'�cran)
///
/// Cette fonction permet de capturer une partie de l'�cran en la mettant dans une texture existante. Le rectangle de la capture de l'�cran est d�fini en pixel par la position (x,y) et la dimension wXh (la position d�signant le coin inf�rieur gauche du rectangle).
///
/// \param textureID identifiant unique de la texture
/// \param x coordonn�e de la position du coin inf�rieur gauche du rectangle de capture
/// \param y coordonn�e de la position du coin inf�rieur gauche du rectangle de capture
/// \param w dimension de la nouvelle texture (doit �tre une puissance de 2)
/// \param h dimension de la nouvelle texture (doit �tre une puissance de 2)
/// \param internalFormat drapeau du format de pixel � �tre utilis� pour la nouvelle texture
void			 dktRenderToTexture(unsigned int textureID, int x, int y, int w, int h, unsigned int internalFormat);



/// \brief d�salloue toute la m�moire allou�e par la cr�ation de textures
///
/// Cette fonction d�sallou� toute la m�moire allou�e par la cr�ation de textures g�n�r�es par l'appel de fonctions de ce module.
///
void			 dktShutDown();



/// \brief met � jour toutes les textures pr�sentement charg�es en m�moire
///
/// Cette fonction v�rifie si des changements ont �t� apport�s aux textures pr�sentement charg�e en m�moire et fait la mise � jour de leur contenu si n�cessaire.
/// La v�rification est faite pour une seule texture � la fois par appel. Chaque appel successif fera la v�rification pour la texture suivante. Lorsqu'on atteint la derni�re texture, on recommence � v�rifier la premi�re texture.
/// Ceci est particuli�rement utile lorsque le module est utilis� par plus d'une application : on peut modifier la texture avec un �diteur et voir imm�diatement le r�sultat dans le jeu par exemple.
///
void			 dktUpdate();



#endif