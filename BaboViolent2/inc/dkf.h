/// \brief Module de gestion des polices de caract�res
///
/// \file dkf.h
/// Ce module prend en charge la gestion des polices de caract�res cr��es � partir d'ume image TGA.
/// Ceci comprend :
/// 	- une fonction de chargement de police de caract�res
/// 	- une fonction de destruction de polices de caract�res
/// 	- des fonctions retournant diverses informations sur une chaine de caract�re selon la police choisie
/// 	- une fonction de s�lection de la police � utiliser
/// 	- une fonction de destruction de toutes les polices de caract�res pr�sentement charg�es
/// 	- une fonction permettant de dessiner une chaine de caract�re sur un QUAD (polygone a 4 cot�) en 3D d'une certaine taille et avec une certaine couleur
///
/// Les couleurs peuvent �tre sp�cifi�es � m�me la chaine de caract�res. Voici la liste des couleurs disponibles et leur caract�re correspondant (la couleur sera utilis� pour les caract�re suivant) :
/// 	- \x1 = texte bleu
/// 	- \x2 = texte vert
/// 	- \x3 = texte cyan
/// 	- \x4 = texte rouge
/// 	- \x5 = texte magenta
/// 	- \x6 = texte brun
/// 	- \x7 = texte gris clair
/// 	- \x8 = texte gris fonc�
/// 	- \x9 = texte jaune
///
/// De plus, le caract�re retour de chariot (\n) peut �tre utilis� afin de pouvoir �crire sur plus d'une ligne avec un seul appel � dkfPrint().
/// \author David St-Louis (alias Daivuk)
/// \author Louis Poirier (� des fins de documentation seulement)
///


#ifndef DKF_H
#define DKF_H


#include "CVector.h"



// Les fonction du DKT

/// \brief sp�cifie la police de caract�re � devant �tre utilis�
///
/// Cette fonction permet de sp�cifier la police de caract�res qui sera active. La police de caract�res active est celle qui sera utilis� pour l'affichage d'une chaine de caract�res et pour divers calculs de longueurs de chaines.
/// Si on d�sire changer de police, un nouvel appel sp�cifiant la nouvelle police de caract�re devant �tre active devra �tre fait.
///
/// \param ID identifiant unique de la police de caract�res
void			dkfBindFont(unsigned int ID);



/// \brief cr�e une police de caract�res
///
/// Cette fonction cr�e une police de caract�res � partir d'une image TGA valide. Pour �tre consid�rer valide, une image TGA doit respecter certaine r�gle:
/// 	- doit �tre de dimension 512x512 pixels
/// 	- doit repr�senter un caract�re dans une zone de 32x64 pixels maximum (afin de disposer d'assez d'aire pour 128 caract�res)
/// 	- un masque alpha doit d�limiter verticalement chaque caract�re avec au moins une ligne de 64 pixels de couleur (0,0,0) (ceci veut aussi dire que le dessin d'un caract�re ne doit pas �tre divis� en deux par une ligne verticale de 64 pixels de noir)
/// 	- les caract�res repr�sent�es doivent �tre ceux entre le code ASCII 32 et 160 exclusivement et dans l'ordre
/// 	- le premier caract�re (33) doit �tre repr�sent� � partir du coin sup�rieur gauche de l'image, les caract�res suivants �tant successivement repr�sent�s vers la droite et ensuite de nouveau � partir de gauche lorsqu'on atteint la limite de l'image
///
/// \param filename chemin menant au fichier image TGA depuis l'endroit o� se situe le fichier EXE du programme � partir de laquelle une police de caract�re sera cr��
/// \return identifiant unique r�f�rant la nouvelle police de caract�re
unsigned int	dkfCreateFont(char *filename);



/// \brief destruction d'une police de caract�res
///
/// Cette fonction lib�re la m�moire allou�e dans la cr�ation d'une police de caract�res
///
/// \param ID pointeur vers l'identifiant unique r�f�rant la police de caract�res devant �tre effac�e
void			dkfDeleteFont(unsigned int *ID);



/// \brief obtient la position d'un caract�re dans une chaine de caract�re
///
/// Cette fonction obtient la position de la premi�re occurence d'un caract�re qui se trouve possiblement dans une chaine de caract�re.
/// La police de caract�res active est utilis� pour �valuer la largeur de chaque caract�re. Les caract�res \n sont aussi consid�r�s dans le calcul. Ce qui veut dire que la hauteur du caract�re peut ne pas �tre nulle.
///
/// \param size hauteur en pixel de la chaine de caract�res
/// \param text chaine de caract�res � consid�rer
/// \param caracter caract�re dont la position sera retourner
/// \return position de la premi�re occurence du caract�re dans la chaine de caract�res � consid�rer
CPoint2f		dkfGetCaracterPos(float size, char *text, int caracter);



/// \brief obtient l'index du caract�re se trouvant � une certaine position dans une chaine de caract�res
///
/// Cette fonction retourne le nombre d�signant le n i�me caract�re d'une chaine de caract�res qui se trouve � la position onStringPos.
///
/// \param size hauteur en pixel de la chaine de caract�res
/// \param text chaine de caract�res � consid�rer
/// \param onStringPos position cible en pixel dont l'origine se trouve dans le coin sup�rieur gauche du premier caract�re
/// \return le n i�me caract�re d'une chaine de caract�res qui se trouve � la position onStringPos
int				dkfGetOverStringCaracter(float size, char *text, CPoint2f & onStringPos);



/// \brief retourne la hauteur total d'une chaine de caract�res en consid�rant les caract�res \n
///
/// Cette fonction retourne la hauteur total en pixel d'une chaine de caract�res en consid�rant les caract�res \n.
///
/// \param size hauteur en pixel d'un caract�re
/// \param text chaine de caract�res � consid�rer
/// \return hauteur total en pixels
float			dkfGetStringHeight(float size, char *text);



/// \brief retourne la largeur de la plus grande sous-chaine d'une chaine de caract�res d�limit�es par les caract�res \n
///
/// Cette fonction retourne la largeur de la plus grande sous-chaine d'une chaine de caract�res d�limit�es par les caract�res \n
///
/// \param size hauteur en pixel d'un caract�re
/// \param text chaine de caract�res � consid�rer
/// \return largeur de la plus grande sous-chaine
float			dkfGetStringWidth(float size, char *text);



/// \brief dessine une chaine de caract�res
///
/// Cette fonction dessine une chaine de caract�res sur un QUAD (polygone � 4 cot�) en 3D en utilisant la police de caract�res active, les caract�res de couleurs et le caract�res retour de chariot (\n).
///
/// \param size grandeur du texte � dessiner en pourcentage par rapport � la hauteur originale (l'aggrandissement est valide pour la largeur et la hauteur du texte)
/// \param x position du texte en 3D
/// \param y position du texte en 3D
/// \param z position du texte en 3D
/// \param text chaine de caract�res � dessiner � l'�cran
void			dkfPrint(float size, float x, float y, float z, char *text);



/// \brief destruction de toutes les polices de caract�res pr�sentement charg�es en m�moire
///
/// Cette fonction lib�re toute la m�moire allou�e pour toutes les polices de caract�res pr�sentement charg�es.
///
void			dkfShutDown();




#endif