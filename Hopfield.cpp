/*
MODELO DE HOPFIELD

Tenemos una red de NxN neuronas, cada una activada (vale 1) o desactivada (vale 0). Cada neurona interactúa con
todas las de alrededor. La fórmula concreta está en el guion, pero lo importante es que el incremento de energía al
activar o desactivar una neurona, s_ij----->1-s_ij es:

Suma de k=1 hasta N {
    Suma de l=1 hasta N {
        w_ijkl*((2s_ij-1)s_kl + s_ij/2)

        Parece que en realidad es:
        w_ijkl*(2s_ij-1)*(2s_kl-1)/2
    }
}

Recuerda que los w_ijkl dependen de los patrones almacenados, y por tanto, son fijos a lo largo de todo el programa.

¿Cómo averiguamos la evolución del conjunto de neuronas? Con un algoritmo de Metrópolis:

0) Dar una temperatura entre 0 y 5. Generar una configuración inicial de neuronas, quizá aleatoriamente
o todas activadas o desactivadas

1) Elegir un punto al azar de la red (n,m)

2) Evaluar p=min(1, exp(-DeltaE/T)), siendo DeltaE la calculada por la formulita de arriba.

3) Generar un número aleatorio entre 0 y 1. Si es menor que p, cambiar el estado de la neurona

4) Volver a 1)
*/

#include <iostream>
#include <fstream> // Ficheros
#include <cmath>
#include <iomanip> // Formato en los ficheros
#include <gsl/gsl_rng.h> // Números aleatorios. Cortesía de gsl
#include <sys/time.h> // Tiempo

// El número de neuronas es N^2
#define N 100
#define P 3 // Cantidad de estados anteriores que la red recuerda

using namespace std;

long int semillatiempo();
void leerpatrones(int patrones[][N][P]);
// void calcularcoefw(int patrones[][N][P], double coefw[][N][N][N]);
void calcularcoefa(double coefa[], int patrones[][N][P]);
void inicializarneuronas(int neuronas[][N], int patrones[][N][P], int caso, gsl_rng *tau);
void iteracion(int neuronas[][N], int patrones[][N][P], double coefa[], double temp, gsl_rng *tau);
double incEnergia(int neuronas[][N], int patrones[][N][P], double coefa[], int i, int j);
void calcsolapamiento(double solapamiento[], int neuronas[][N], int patrones[][N][P], double coefa[]);

/**************************************************** FUNCIÓN PRINCIPAL ****************************************************/
int main(void) {

    // Declaro las variables
    int neuronas[N][N];
    int patrones[N][N][P];
    double coefa[P];
    double solapamiento[P];

    double temp;
    int pasosMC;
    int caso;

    // Inicializo el puntero de números aleatorios
    gsl_rng *tau;
    tau=gsl_rng_alloc(gsl_rng_taus);
    gsl_rng_set(tau,semillatiempo());

    // Pido los parámetros de la simulación
    cout << "Escoja una temperatura entre 0 y 5\n";
    cin >> temp;

    cout << "Escoja el número de pasos Monte Carlo\n";
    cin >> pasosMC;

    cout << "Elija una inicialización. 1 = Aleatorios, 2 = Patrón deformado\n";
    cin >> caso;

    // Leo los patrones y calculo los a_mu
    leerpatrones(patrones);
    calcularcoefa(coefa, patrones);

    // Inicializo las neuronas, dependiendo del caso que se haya escogido, y calculo el solapamiento
    inicializarneuronas(neuronas, patrones, caso, tau);
    calcsolapamiento(solapamiento, neuronas, patrones, coefa);
    

    // Abro el fichero donde escribo las neuronas. Imprimo las primeras
    ofstream fichero;
    ofstream fich_solap;
    fichero.open("neuronas.dat");
    fich_solap.open("solapamiento.dat");

    for(int i=0; i<N; i++) {
        for(int j=0; j<N-1; j++) fichero << neuronas[i][j] << ",";

        fichero << neuronas[i][N-1] << "\n";
    }

    // Imprimo los solapamientos
    for(int mu=0; mu<P; mu++) fich_solap << setw(15) << solapamiento[mu];
    

    // Variables para el proceso de iteración
    int pasos = pasosMC*N*N;

    // Itero en el tiempo
    for(int k=1; k<=pasos; k++) {

        // Intento cambiar una neurona
        iteracion(neuronas, patrones, coefa, temp, tau);

        /*Imprimo las neuronas en el archivo, pero solo cada paso de Monte Carlo.
        Como el solapamiento solo usa información del mismo paso, lo calculo solo cuando vaya a imprimirlo*/
        if(k%(N*N)==0) {

            calcsolapamiento(solapamiento, neuronas, patrones, coefa);

            fichero << "\n";
            fich_solap << "\n";
            
            for(int i=0; i<N; i++) {
                for(int j=0; j<N-1; j++) fichero << neuronas[i][j] << ",";

                fichero << neuronas[i][N-1] << "\n";
            }

            for(int mu=0; mu<P; mu++) fich_solap << setw(15) << solapamiento[mu];

        }
    }

    fichero.close();
    fich_solap.close();

    return 0;
}
/***************************************************************************************************************************/

/*Función semillatiempo. Crea una semilla para generar números aleatorios, basada en el tiempo actual.*/
long int semillatiempo()
{
    struct timeval tv;
    gettimeofday(&tv,0);
    return (tv.tv_sec + tv.tv_usec);
}

/*Función leerpatrones. Lee los patrones correspondientes y los almacena en patrones[N][N][P]*/
void leerpatrones(int patrones[][N][P]) {
    ifstream fichero;
    string str;

    // Para cada patrón
    for(int mu=0; mu<P; mu++) {
        // Intento abrir el fichero correspondiente
        fichero.open("patron_" + to_string(mu+1) + ".dat");

        // Si lo consigue
        if(fichero.is_open()) {

            // Leo la fila correspondiente
            for(int i=0; i<N; i++) {

                // Paso la línea al string
                fichero >> str;

                int j=0;
                for(char numerito : str) {
                    patrones[i][j][mu]=numerito-'0';
                    j++;
                }

            }

            fichero.close();
        }
    }

    return;
}

/*Función calcularcoefa. Calcula los a_mu y los almacena en el array coefa*/
void calcularcoefa(double coefa[], int patrones[][N][P]) {

    // Calculo las a_mu que intervienen en el cálculo de w_ijkl
    for(int mu=0; mu<P; mu++) {

        coefa[mu]=0;
        for(int i=0; i<N; i++) {
            for(int j=0; j<N; j++) coefa[mu]+=patrones[i][j][mu];
        }

        coefa[mu]=coefa[mu]/(N*N);
    }

    return;
}

/*Función inicializarneuronas. Simplemente inicializa las neuronas*/
void inicializarneuronas(int neuronas[][N], int patrones[][N][P], int caso, gsl_rng *tau) {

    // Si las inicializamos aleatoriamente (caso 1)
    if(caso==1) {

        // Asigno valores
        for(int i=0; i<N; i++) {
            for(int j=0; j<N; j++) {

                // Esta función da 0 ó 1 con probabilidad 1/2. Los 0 se quedan
                neuronas[i][j]=gsl_rng_uniform_int(tau,2);
            }
        }
    }

    // Si queremos uno de los patrones deformados (caso 2)
    else {
        int mu;
        int iesc; int jesc;
        int numeroacambiar;
        double porcentajeacambiar;

        cout << "Elija cuál de ellos: 1, 2 ó 3\n";
        cin >> mu;

        cout << "Porcentaje de deformación:\n";
        cin >> porcentajeacambiar;

        numeroacambiar=floor(porcentajeacambiar*N*N/100);

        // Corrijo para que sea 0, 1 ó 2
        mu+=-1;

        // Copio el patrón en neuronas
        for(int i=0; i<N; i++) {
            for(int j=0; j<N; j++) neuronas[i][j]=patrones[i][j][mu];
        }

        // Deformo el patrón
        for(int k=0; k<numeroacambiar; k++) {
            iesc=gsl_rng_uniform_int(tau,N);
            jesc=gsl_rng_uniform_int(tau,N);

            neuronas[iesc][jesc]=1-neuronas[iesc][jesc];
        }


    }

    return;
}

/*Función iteracion. Es el núcleo del programa. Realiza los pasos que determinan si se activa o desactiva
alguna neurona escogida aleatoriamente*/
void iteracion(int neuronas[][N], int patrones[][N][P], double coefa[], double temp, gsl_rng *tau) {

    double p;
    double numerito;

    // En primer lugar, escojo una neurona (i,j) al azar
    int i=gsl_rng_uniform_int(tau, N);
    int j=gsl_rng_uniform_int(tau, N);

    // Calculo la p correspondiente
    numerito=exp(-incEnergia(neuronas, patrones, coefa, i, j)/temp);
    
    if(numerito>=1) p=1;
    else p=numerito;

    /*Calculo un número aleatorio entre 0 y 1. Si es <p, cambio la neurona de estado. Utilizo el mismo double numerito
    que ahora no sirve de nada más*/
    numerito=gsl_rng_uniform(tau);
    if(numerito<p) neuronas[i][j]=1-neuronas[i][j];

    return;

}

/*Función incEnergia. Calcula el incremento de energía que supondría cambiar la neurona (i,j) de estado.
Recibe el conjunto de neuronas, los patrones y los coefa para calcular w_ijkl, así como el i,j escogido*/
double incEnergia(int neuronas[][N], int patrones[][N][P], double coefa[], int i, int j) {

    double suma;
    double coefw;

    // Calculo el incremento de energía
    suma=0;
    for(int k=0; k<N; k++) {
        for(int l=0; l<N; l++) {

            // Calculo el w_ijkl y sumo salvo si i=k y j=l
            if((i!=k)||(j!=l)) {
                
                coefw=0;
                for(int mu=0; mu<P; mu++) {
                    coefw+=(patrones[i][j][mu]-coefa[mu])*(patrones[k][l][mu]-coefa[mu]);
                }

                coefw=coefw/(N*N);

                // Sumo el término correspondiente
                suma+=coefw*(neuronas[k][l]-0.5);
            }
        }
    }

    // Producto fuera del sumatorio
    suma=suma*(2*neuronas[i][j]-1);

    return suma;
}

/*Función calcsolapamiento. Calcula el solapamiento de la configuración de neuronas con cada uno de
los patrones*/
void calcsolapamiento(double solapamiento[], int neuronas[][N], int patrones[][N][P], double coefa[]) {

    // Calculo el solapamiento
    for(int mu=0; mu<P; mu++) {

        // Inicializo y empiezo a sumar
        solapamiento[mu]=0;
        for(int i=0; i<N; i++) {
            for(int j=0; j<N; j++) solapamiento[mu]+=(patrones[i][j][mu]-coefa[mu])*(neuronas[i][j]-coefa[mu]);
        }

        solapamiento[mu]=solapamiento[mu]/(N*N*coefa[mu]*(1-coefa[mu]));
    }

    return;
}