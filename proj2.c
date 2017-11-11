#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
void napoveda(){
    printf("Napoveda\n"
                   "--tan\n"
                   "porovná přesnosti výpočtu tangens úhlu v radiánech mezi voláním"
                   " z matematické knihovny, výpočtu  pomocí Taylorova polynomu a zřetězeného zlomku.\n"
                   "Program se spouští: --tan \"úhel v radiánech\" \"první zobrazená iterace\" \"poslední zobrazená iterace\" \n"
                   "Např. --tan 1.2 5 10\n");
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
 * Funce pocita tangens pomoci Taylorova polynomu.
 * Pri kazde iteraci je vysledek presnejsi.
 * @param x Uhel v radianech.
 * @param n Pocet iteraci.
 * @return Vysledek vypoctu tangens.
 */
double taylor_tan(double x, unsigned int n){
    long citatel[13] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162, 18888466084, 113927491862,58870668456604};
    long jmenovatel[13] = {1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875, 1856156927625, 194896477400625, 49308808782358125, 3698160658676859375};
    double vysl ;
    double xNaExponent = x;
    for (unsigned int i = 1; i <= n && i <= 13; ++i) {
        vysl +=(citatel[i-1]*xNaExponent)/jmenovatel[i-1];
        xNaExponent*=x*x;
    }
    return vysl;
}

double cfrac_tan(double x, unsigned int n){
    double vysl;
    for (int i = n; i > 0 ; --i) {
        vysl =1/( (  (i*2-1)/x) - vysl);
    }
    return  vysl;
}


int main(int argc, char** argv){
    if(argc > 1) {
        if (strcmp(argv[1], "--help") == 0) {
            napoveda();
        } else if(strcmp(argv[1],"--tan") == 0) {
            for(int i = atoi(argv[3]); i <= atoi(argv[4]);i++){
                printf("%d.  %e   %e   %e   %e   %e\n",
                       i,
                       tan(atof(argv[2])),
                       taylor_tan(atof(argv[2]),i),
                       ownAbs(taylor_tan(atof(argv[2]),i) - tan(atof(argv[2]))),
                       cfrac_tan(atof(argv[2]),i),
                       ownAbs(cfrac_tan(atof(argv[2]),i) - tan(atof(argv[2]))));

            }
        } else if(strcmp(argv[1],"-m") == 0 || (strcmp(argv[1],"-c") == 0 && strcmp(argv[3],"-m") == 0)){
            double c;
            char *ptr;
            if(strcmp(argv[1],"-c") == 0){
                if((strtod(argv[2],&ptr) > 0 && strtod(argv[2],&ptr) <= 100)) {
                    c = atof(argv[2]);
                }else{
                    fprintf(stderr,"Vyska merice musi byt cislo\n");
                    exit(0);
                }
                if(argc == 5){
                    //pointer na konec porovnani, zde = INF
                    if(strtod(argv[4], &ptr) > 0.0 && strtod(argv[4], &ptr) <= 1.4){
                        double vysledek = c/cfrac_tan(atof(argv[4]),14);
                        printf("%e\n",vysledek);
                    }else{
                        fprintf(stderr,"Uhel aplha neni v poradku\n");
                    }
                }else if(argc == 6){
                    //pointer na konec porovnani, zde = INF
                    if( (strtod(argv[5], &ptr) > 0 && strtod(argv[5], &ptr) < 1.4) && (strtod(argv[4], &ptr) > 0.0 && strtod(argv[4], &ptr) <= 1.4)){
                        double vzdalenost = c/cfrac_tan(atof(argv[4]),14);
                        printf("%e\n",vzdalenost);
                        double vysledek = (vzdalenost * cfrac_tan(atof(argv[5]),14) ) + c;
                        printf("%e\n",vysledek);
                    }else{
                        fprintf(stderr,"Uhel alpha nebo beta neni v poradku\n");
                    }
                }else{
                    fprintf(stderr,"Neplatny pocet argumentu\n");
                }
            }else{
                c = 1.5;
                if(argc == 3){
                    //pointer na konec porovnani, zde = INF
                    if(strtod(argv[2], &ptr) > 0.0 && strtod(argv[2], &ptr) <= 1.4){
                        double vysledek = c/cfrac_tan(atof(argv[2]),14);
                        printf("%e\n",vysledek);
                    }else{
                        fprintf(stderr,"Uhel aplha neni v poradku\n");
                    }
                }else if(argc == 4){
                    //pointer na konec porovnani, zde = INF
                    if( (strtod(argv[3], &ptr) > 0 && strtod(argv[3], &ptr) < 1.4) && (strtod(argv[2], &ptr) > 0.0 && strtod(argv[2], &ptr) <= 1.4)){
                        double vzdalenost = c/cfrac_tan(atof(argv[2]),14);
                        printf("%e\n",vzdalenost);
                        double vysledek = (vzdalenost * cfrac_tan(atof(argv[3]),14) ) + c;
                        printf("%e\n",vysledek);
                    }else{
                        fprintf(stderr,"Uhel alpha nebo beta neni v poradku\n");
                    }
                }else{
                    fprintf(stderr,"Neplatny pocet argumentu\n");
                }
            }
        }else{
            fprintf(stderr,"Chybne pouziti programu, projdete si --help\n");
        }
    }
}