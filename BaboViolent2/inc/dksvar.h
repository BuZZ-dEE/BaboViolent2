/// \brief Module de gestion des variables de la console
///
/// \file dksvar.h
/// Ce module prend en charge la gestion des variables de la console.
/// Ceci comprend :
/// 	- des fonction d'enregistrement de variables
/// 	- une fonction de d�senregistrement de variables
/// 	- une fonction de d'ex�cution d'un fichier de configuration
/// 	- une fonction d'ex�cution de commandes
///
/// Ce module est tr�s utile pour d�bugger : on peut modifier le contenu d'une variables pr�alablement enregistr�e en temps-r�el lors de l'ex�cution du programme (habituellement via une fen�tre console). 
///
/// \author David St-Louis (alias Daivuk)
/// \author Louis Poirier (� des fins de documentation seulement)



#ifndef DKSVAR_H
#define DKSVAR_H


#pragma comment (lib, "dksvar.lib")


#include "CVector.h"
#include "CString.h"
#include "dksvardef.h"

class CStringInterface
{
public:
	virtual void updateString(CString* string, char * newValue) = 0;
};

// Les fonction du DKSVAR


/// \brief ex�cute une commande
///
/// Cette fonction permet d'ex�cuter une commande pass�e sous forme d'une chaine de caract�res. Pr�sentement, la seul commande valide est 'set' et sa syntaxe est la suivante:
/// set nomDeLaVariable param1 [param2 param3 ...]
/// L'ex�cution de cette commande tente de remplir les champs de la variable enregistr�e nomDeLaVariable avec les param�tres param1 param2 ... fournis.
///
/// \param command chaine de caract�res repr�sentant une commande valide
CMD_RET			dksvarCommand(char * command);



/// \brief ex�cution d'un fichier de configuration
///
/// Cette fonction permet d'ex�cuter toutes les commandes contenues dans un fichier de configuration.
///
/// \param filename chemin menant au fichier de configuration � ex�cuter depuis l'endroit o� se situe le fichier EXE du programme.
void			dksvarLoadConfig(char * filename);
void			dksvarLoadConfigSVOnly(char * filename);
void			dksvarSaveConfig(char * filename);



/// \name VariableRegistering
///
/// \brief enregistre une variable d'un certain type
/// Ce groupe de fonctions permet d'enregistrer une variable d'un certain type (bool,int,float,CVector2i,CVector2f,CVector3i,CVector3f,CVector4f,CString) en sp�cifiant si cette variable permet d'�tre modifi�e par l'ex�cution d'un fichier de configuration.
///
/// \param screenName nom de la variable qui sera associ� � la variable elle-m�me
/// \param defaultValue valeur par d�faut de la variable
/// \param true si cette variable permet d'�tre modifi�e par l'ex�cution d'un fichier de configuration, false sinon
//@{
void			dksvarRegister(const CString &screenName, bool		 *defaultValue,	bool mConfigBypass);
void			dksvarRegister(const CString &screenName, int		 *defaultValue, int minValue,
											 int maxValue, int flags, bool mConfigBypass);
void			dksvarRegister(const CString &screenName, float		*defaultValue, float minValue,
											 float maxValue, int flags, bool mConfigBypass);
void			dksvarRegister(const CString &screenName, CVector2i *defaultValue, bool mConfigBypass);
void			dksvarRegister(const CString &screenName, CVector2f *defaultValue, bool mConfigBypass);
void			dksvarRegister(const CString &screenName, CVector3i *defaultValue, bool mConfigBypass);
void			dksvarRegister(const CString &screenName, CVector3f *defaultValue, bool mConfigBypass);
void			dksvarRegister(const CString &screenName, CVector4f *defaultValue, bool mConfigBypass);
void			dksvarRegister(const CString &screenName, CString	 *defaultValue,	bool mConfigBypass);
//@}


/// \brief d�senregistre une variable enregistr�e
///
/// Cette fonction permet de d�senregistrer une variable enregistr�e. La variable correspondant au nom fournis ne sera plus assujettie � des modifications provenant de l'ex�cution de commandes.
///
/// \param screenName nom de la variable associ� � la variable elle-m�me
void			dksvarUnregister(const CString &screenName);



void			dksvarInit(CStringInterface * stringInterface);
void			dksvarGetFilteredVar(char * varName, char ** array, int size);
void			dksvarGetFormatedVar(char * varName, CString * formatedString);



#endif