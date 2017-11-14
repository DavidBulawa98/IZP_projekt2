/**
 * @file    proj2.c
 * @author  David Bulawa
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

//Presnost na 7 iteraci pro 10 des. mist
#define PRESNOST 7

/**
 * Funkce vytiskne napovedu.
 */
void napoveda(){
    printf("\nAutor: David Bulawa\n\n"
                   "////////Porovnani presnosti tangens\n"
                   "Porovna presnosti vypoctu tangens uhlu v radianech mezi volanim"
                   " z matematicke knihovny, vypoctu  pomoci Taylorova polynomu a zretezeneho zlomku.\n"
                   "Program se spousti: --tan \"uhel v radianech\" \"prvni zobrazena iterace\" \"posledni zobrazena iterace\" \n"
                   "Napr. --tan 1.2 5 10\n"
                   "Iterace musi byt v intervalu 0 < iterace <= 13\n\n"
                   "/////////Mereni\n"
                   "Pri zadani uhlu alpha program vypisuje vzdalenost merice od mereneho objektu\n"
                   "Program se spousti: -m \"uhel alpha v radianech\"\n"
                   "Napr. -m 0.3\n"
                   "Pokud jako dalsi argument pridame i uhel beta program vypisuje krome vzdalenosti take vysku mereneho objektu"
                   "Program se spousti: -m \"uhel alpha v radianech\" \"uhel beta v radianech\"\n"
                   "Napr. -m 0.3 0.5\n"
                   "Pokud jako dalsi argument pridame i vysku mericiho pristroje, potom se upravi vyska, ktera je implicitne 1.5"
                   "Program se spousti: -c \"vyska mericiho pristroje\" -m \"uhel alpha v radianech\" \"uhel beta v radianech\"\n"
                   "Napr. -c 1.7 -m 0.5 1.1\n"
                   "Uhly musi byt zadavany v intervalu 0 < uhel <= 1.4\n"
                   "Vyska mericiho pristroje musi byt zadana v intervalu 0 < vyska <= 100\n\n");
}

/**
 * Nahrada za funkci z matematicke knihovny.
 * V pripade zaporneho cisla nasobi -1.
 * @param x Cislo, u ktereho pozadujeme absolutni hodnotu.
 * @return Vraci absolutni hodnotu cisla.
 */
double ownAbs(double x){
    if(x < 0.0){
        return x*(-1);
    }else{
        return x;
    }
}

/**
 * Funkce pocita tangens pomoci Taylorova polynomu.
 * Pri kazde dalsi iteraci je vysledek presnejsi.
 * @param x Uhel v radianech.
 * @param n Pocet iteraci.
 * @return Vysledek vypoctu tangens.
 */
double taylor_tan(double x, unsigned int n){
    long citatel[13] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162, 18888466084, 113927491862,58870668456604};
    long jmenovatel[13] = {1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875, 1856156927625, 194896477400625, 49308808782358125, 3698160658676859375};
    double vysl = 0;
    double xNaExponent = x;
    for (unsigned int i = 1; i <= n && i <= 13; ++i) {
        vysl +=(citatel[i-1]*xNaExponent)/jmenovatel[i-1];
        xNaExponent *=x*x;
    }
    return vysl;
}

/**
 * Funkce pocita tangens pomoci zretezenych zlomku.
 * Pri kazde dalsi itereci je vysledek presnejsi.
 * @param x Uhel v radianech.
 * @param n Pocet iteraci.
 * @return Vysledek vypoctu tangens.
 */
double cfrac_tan(double x, unsigned int n){
    double vysl = 0;
    for (int i = n; i > 0 ; --i) {
        vysl =1/( (  (i*2-1)/x) - vysl);
    }
    return  vysl;
}

/**
 * Funkce pro vypocet vzdalenosti.
 * @param c Vyska mericiho pristroje.
 * @param uhelA Uhel alpha v radianech.
 * @return Vypocitana vzdalenost.
 */
double vzdalenost(double c, double uhelA){
    if(uhelA > 0 && uhelA <=1.4){
        return  c/cfrac_tan(uhelA,PRESNOST);
    }else{
        fprintf(stderr,"Uhel alpha neni v poradku\n");
        exit(1);
    }
}

/**
 * Funkce pro vypocet vzdalenosti.
 * @param c Vyska mericiho pristroje.
 * @param vzdalenost Vzdalenost mericiho pristroje od objektu.
 * @param uhelB Uhel beta v radianech.
 * @return Vypocitana vyska.
 */
double vyska(double c, double vzdalenost, double uhelB){
    if( uhelB > 0 && uhelB <=1.4){
        return  vzdalenost * cfrac_tan(uhelB,PRESNOST) + c;
    }else{
        fprintf(stderr,"Uhel beta neni v poradku\n");
        exit(1);
    }
}

int main(int argc, char** argv){
    //alespon 2 argumenty
    if(argc > 1) {
        //pointer pro strtod
        char *ptr;
        //napoveda, tangens nebo vypocet vysky/vzdalenosti
        if (strcmp(argv[1], "--help") == 0) {
            napoveda();
            return EXIT_FAILURE;
        } else if (strcmp(argv[1], "--tan") == 0) {
            //pocet argumentu vzdy 5
            if (argc != 5) {
                fprintf(stderr, "Neplatny pocet argumentu\n");
                return EXIT_FAILURE;
            }
            double uhel = strtod(argv[2], &ptr);
            int iterace1 = atoi(argv[3]);
            int iterace2 = atoi(argv[4]);
            //kontrola rozmezi povolenych iteraci
            if ((iterace1 > 0  && iterace2 <= 13) && (iterace1 <= iterace2 ) && (uhel > 0 && uhel <= 1.4 ))  {
                //vypis radku cyklem
                for (int i = iterace1; i <= iterace2; i++) {
                    printf("%d.  %e   %e   %e   %e   %e\n",
                           i,
                           tan(uhel),
                           taylor_tan(uhel, i),
                           ownAbs(taylor_tan(uhel, i) - tan(uhel)),
                           cfrac_tan(uhel, i),
                           ownAbs(cfrac_tan(uhel, i) - tan(uhel)));
                }
                return EXIT_SUCCESS;
            } else {
                fprintf(stderr, "Iterace musi byt v intervalu 0 < iterace1 <= iterace2 <= 13 a uhel v intervalu 0 < uhel <= 1.4\n");
                return EXIT_FAILURE;
            }
        } else if (strcmp(argv[1], "-m") == 0 || (strcmp(argv[1], "-c") == 0)) {
            //vyska merice, standartne 1.5
            double c;
            //zda-li byla zadana neimplicitni vyska merice
            if (strcmp(argv[1], "-c") == 0) {
                //kontrola spravneho intervalu vysky merice
                if ((strtod(argv[2], &ptr) > 0 && strtod(argv[2], &ptr) <= 100)) {
                    c = atof(argv[2]);
                } else {
                    fprintf(stderr, "Vyska merice musi byt cislo v intervalu 0 < vyska <= 100\n");
                    return EXIT_FAILURE;
                }
                //pocitam vzdalenost(zadan pouze uhel alpha)nebo pocitam i vysku(zadan i uhel beta)
                if (argc == 5) {
                    printf("%.10e\n", vzdalenost(c, strtod(argv[4], &ptr)));
                    return EXIT_SUCCESS;
                } else if (argc == 6) {
                    double vzdalenostAktualni = vzdalenost(c, strtod(argv[4], &ptr));
                    printf("%.10e\n", vzdalenostAktualni);
                    printf("%.10e\n", vyska(c, vzdalenostAktualni, strtod(argv[5], &ptr)));
                    return EXIT_SUCCESS;
                } else {
                    fprintf(stderr, "Neplatny pocet argumentu\n");
                    return EXIT_FAILURE;
                }
            } else {
                c = 1.5;
                //pocitam vzdalenost(zadan pouze uhel alpha)nebo pocitam i vysku(zadan i uhel beta)
                if (argc == 3) {
                    printf("%.10e\n", vzdalenost(c, strtod(argv[2], &ptr)));
                    return EXIT_SUCCESS;
                } else if (argc == 4) {
                    double vzdalenostAktualni = vzdalenost(c, strtod(argv[2], &ptr));
                    printf("%.10e\n", vzdalenostAktualni);
                    printf("%.10e\n", vyska(c, vzdalenostAktualni, strtod(argv[3], &ptr)));
                    return EXIT_SUCCESS;
                } else {
                    fprintf(stderr, "Neplatny pocet argumentu\n");
                    return EXIT_FAILURE;
                }
            }
        } else {
            fprintf(stderr, "Chybne zadane argumenty programu, projdete si --help\n");
            return EXIT_FAILURE;
        }
    } else{
        fprintf(stderr, "Nebyly zadany zadne argumenty, projdete si --help\n");
        return EXIT_FAILURE;
    }
}