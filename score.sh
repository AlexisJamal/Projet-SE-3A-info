#!/bin/bash

consignes=0
exercice1=0
exercice2=0
exercice3=0
exercice4=0

echo -e "\033[1;32m# Consigne de base\033[0m"

compile() {
    gcc src/$1.c -std=c11 -Wall -lm -o bin/$1 2> data/warnings \
    && cat data/warnings\
    && export warning=$(grep "warning:" "data/warnings" | wc -l  | cut -d ' ' -f 1) \
    && rm data/warnings
}

score() {
    if [[ $1 != *.zip ]];
    then
        zip -r $basename.zip . >/dev/null
        echo "Archive crée ! Prête à être déposée sur Spiral"
    fi

    echo -e "\033[1;32mConsignes  : $consignes\033[0m/4"
    echo -e "\033[1;33mExercice 1 : $exercice1\033[0m/6"
    echo -e "\033[1;34mExercice 2 : $exercice2\033[0m/7"
    echo -e "\033[1;35mExercice 3 : $exercice3\033[0m/13"
    echo -e "\033[1;36mExercice 4 : $exercice4\033[0m/4"
    echo
    echo -e "\033[1;38mNote indicative : ~\033[1;31m\
$((20*($consignes+$exercice1+$exercice2+$exercice3+$exercice4)/34))\033[0m/20"
}

tnr() {
    echo -e "Violation des consignes : \033[31;1mTravail Non Rendu\033[0m"
    exit 1
}

abs() {
    if [ $1 -lt 0 ]
    then
        return -$1
    else
        return $1
    fi
}


if [ $# -ne 1 ]
then
    basename="$( cd "$(dirname "${BASH_SOURCE[0]}")" && pwd )"
    basename=$(basename $basename)
else
    basename=${1%.*}
fi

prefix="SE2016."

if [[ $basename == $prefix* ]] ;
then
    echo Nom de fichier valide
    ((consignes=consignes+1))
else
    echo "Le script doit être appelé depuis le dossier du projet" 
    echo "ou avec une archive nomée \"SE2016.NOM1_NOM2.zip\" en premier paramètre"
    tnr
fi

if [[ $basename == $prefix"NOM1_NOM2" ]] ;
then
    echo "Bonjour NOM1 ! Comment vas NOM2 ? Bien ?"
    echo "Il a encore oublié de mettre le bon nom sur sa copie ? Ah bah c'est domage !"
    tnr
fi

group=${basename#$prefix}
students=(${group//_/ })

if (( ${#students[@]} > 1 )) && (( ${#students[@]} <= 3 ));
then
    echo "Groupe de ${#students[@]} conforme aux consignes"
    ((consignes=consignes+4-${#students[@]}))
else
    echo "Groupe de ${#students[@]} non conforme aux consignes \!"
    tnr
fi

if [[ $1 == *.zip ]];
then
    if unzip -o $1;
    then
        cd $basename
        echo "Archive valide"
        ((consignes=consignes+1))
    else
        echo "Archive invalide !"
        tnr
    fi
fi

if [ -f doc/rapport.pdf ];
then
    echo "Compte rendu trouvé"
        ((consignes=consignes+1))
else
    echo "Pas de compte rendu trouvé dans doc/rapport.pdf !"
    tnr
fi

echo -e "\033[1;33m# Exercice 1\033[0m"

if compile killbill;
then
    echo "Compilation réussie ! ($warning warnings)"
    ((exercice1=exercice1+3-warning))
    rm data/zombie 2>/dev/null
    bin/zombie > data/zombie &
	if bin/killbill `cat data/zombie | cut -d ' ' -f 1`;
	then
        cat data/zombie
		echo "Bel envoi de bus !"
		((exercice1=exercice1+3))
	else
        cat data/zombie
		echo "GRRRRRR!! Il nous reste un zombie !"
	fi
    rm data/zombie 2>/dev/null
else
    echo "Echec de compilation !"
fi

echo -e "\033[1;34m# Exercice 2\033[0m"

if compile puzzle;
then
    echo "Compilation réussie ! ($warning warnings)"
    ((exercice2=exercice2+3-warning))
	rm data/puzzle 2>/dev/null
    bin/puzzle | tee data/puzzle
    ((exercice2=exercice2+1-$?))
	cat data/puzzle
	if [[ $(md5sum data/puzzle) == "c07712a9f1c248e2a339f5233426e885  data/puzzle" ]];
	then
		echo "U MAD ?"
        ((exercice2=exercice2+3))
	else
		echo "U LUUUZ !"
    fi
    rm data/puzzle 2>/dev/null
else
    echo "Echec de compilation !"
fi

echo -e "\033[1;35m# Exercice 3\033[0m"

if compile count;
then
    echo "Compilation réussie ! ($warning warnings)"
    ((exercice3=exercice3+3-warning))
    answer=$(bin/count)
    if (( $(ps -ef | wc -l)-1 == $answer ));
    then
        echo "Oui il y a bien $answer processus"
        ((exercice3=exercice3+3))
    else
        echo "Non, il n'y a pas $answer processus"
    fi
else
    echo "Echec de compilation !"
fi

if compile mario;
then
    echo "Compilation réussie ! ($warning warnings)"
    ((exercice3=exercice3+3-warning))
    rm data/self 2>/dev/null
    bin/mario > data/self
    ((exercice3=exercice3+1-$?))
    #cat data/self
	if [[ $(md5sum data/self) == "9ffefb35f86c661d645b98b0b2f00074  data/self" ]];
	then
		echo "C'est du propre !"
        ((exercice3=exercice3+3))
	else
		echo "Il y a un problème apparement"
    fi
    rm data/self 2>/dev/null
else
    echo "Echec de compilation !"
fi

echo -e "\033[1;36m# Exercice 4\033[0m"

if compile morpion;
then
    echo "Compilation réussie ! ($warning warnings)"
    ((exercice4=exercice4+3-warning))
    echo "Je testerais le jeu manuellement"
    bin/morpion
    ((exercice4=exercice4+1-$?))
else
    echo "Echec de compilation !"
fi

score $1

