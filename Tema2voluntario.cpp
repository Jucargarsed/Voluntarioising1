#include <stdio.h>
#include <cmath>
#include <time.h>
# include "gsl_rng.h" //Libreria para generación de números aleatorios

#define N 16
#define N 16
#define rep (N*N*10000)
#define A (rep/(100*N*N))

gsl_rng *tau;

double correlacion(int spines[N][N], int a);
double Energia(int spines[N][N]);
double Magnetización(int spines[N][N]);
double varianza(double vector[A], double media);


int main(){
    
    double T;
    T=1.5;
    int spines[N][N];
    double Mag[A];
    double Energ[A];
    double Correla[A];
    double Energ2[A];
    double magnt, E, corr, varM, varE, varC, varE2;
    int random;
    int f, c, h, media, aux1,aux2,aux3,aux4;
    double VEnergia, sumaE, Ecuad,Cn, correl, difE;
    double exponencial, p, epsilon;
    double magn=0.0;


    extern gsl_rng *tau; //Puntero al estado del número aleatorio
    int semilla=135254; //Semilla del generador de números aleatorios
    
    
    tau=gsl_rng_alloc(gsl_rng_taus); //Inicializamos el puntero
    gsl_rng_set(tau,semilla); //Inicializamos la semilla

    
    //escribir la matriz
    //diferencia de energia

    FILE* Magnet;
    FILE* es;
    FILE* Capcal;
    FILE* Correlac;



    Magnet=fopen("Magnet", "w");
    es=fopen("Energiamatriz", "w");
    Capcal=fopen("Capcal", "w");
    Correlac=fopen("Correlacion", "w");

    for (int z = 0; z < 11; z++)
    {
        sumaE=0.0;
        E=0.0;
        h=0;
        Ecuad=0.0;
        media=0.0;
        correl=0.0;
        int a=5;
        
    
    
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j< N; j++)
        {
            spines[i][j]=1;
            
            
        }
        //
    }

printf("%lf\t", Energia(spines));




//empezamos el bucle paso montecarlo
    for (int k = 0; k < rep ; k++)
    {
    f=gsl_rng_uniform_int(tau,N); //posicion aleatoria de la matriz
    c=gsl_rng_uniform_int(tau,N);
    // printf("%d\t,%d\t", f,c);
    //calculamos la energia 

   aux1=f+1;
   aux2=f-1;
   aux3=c+1;
   aux4=c-1;
   if (f==N-1)
   {
        aux1=0;
   }
    if (f==0)
    {
        aux2=N-1;
    }
    if (c==N-1)
    {
        aux3=0;
    }
    if (c==0)
    {
        aux4=N-1;
    }
    
    VEnergia=2.0 * spines[f][c] * (spines[aux1][c]+spines[aux2][c]+spines[f][aux3]+spines[f][aux4]);




    //printf("%lf\t", Energia);

    exponencial=exp(-VEnergia*1.0/T);

    //tenemos el minimo entre 1 y exponencial

   p=1.0;
   if (exponencial <1)
   {
        p=exponencial;
   }
   //printf("%lf\n", p);
   epsilon=gsl_rng_uniform(tau);
   //printf("%lf\n", epsilon);

   //si es menor lo cambiamos
   if (epsilon<p)
   {
    spines[f][c]=-1*spines[f][c];
   }
   
   //hace lo que debas
    
    if (h==100*N*N)
    {
        magnt=Magnetización(spines);
        E=Energia(spines);
        corr=correlacion(spines, a);
    

        Mag[media]=magnt;
        Energ[media]=E;
        Correla[media]=corr;
        Energ2[media]=E*E;
        //printf("EE=%lf\t", Energ2[media]);

        magn=magn+magnt; 
        sumaE=sumaE+E;
        Ecuad=Ecuad+E*E;
        correl=correl+corr;
        media=media+1;

        //printf("EE=%lf\t", Energ[media]);

        h=0;
    }
     else
    {
        h=h+1;
    }

}
magn=magn/media;
varM=varianza(Mag, magn);
fprintf(Magnet,"%lf\t%lf\t%lf\n", T, magn, 2*varM/sqrt(A));


//printf("Mag=%lf\t", varM);


sumaE=sumaE/media;
//printf("Emedia=%lf\t", sumaE);
//printf("E=%lf\t", sumaE/(2*N));
varE=varianza(Energ, sumaE);
fprintf(es,"%lf\t%lf\t%lf\n", T,sumaE/(2*N),2*varE/((2*N)*sqrt(A)));

Ecuad=Ecuad/media;
varE2=varianza(Energ2,Ecuad);
//printf("var=%lf\t", Ecuad);

varE2=sqrt((pow(varE2,2)+4*sumaE*sumaE*pow(varE,2))/A);

difE=abs(Ecuad-pow(sumaE,2));


Cn=difE/(N*N*T);

//printf("Cn=%lf\t", Cn);
fprintf(Capcal,"%lf\t%lf\t%lf\n", T, Cn,varE2/(N*A*N*T));


correl=correl/(media);
varC=varianza(Correla, correl);
//printf("F=%lf\t", correl);
fprintf(Correlac,"%lf\t%lf\t%lf\n", T, correl,2*varC/sqrt(A));
    
    




T=T+0.2;
    }
    fclose(Magnet);
    fclose(es);
    fclose(Capcal);
    fclose(Correlac);

    return 0;
}



double Magnetización(int spines[N][N]){
    double suma=0.0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            suma=suma+spines[i][j];
        }
        
    }
    return suma/(N*N);
}


double Energia(int spines[N][N])
{
    double E;
    E=0;
    int aux1, aux2, aux3, aux4;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            aux1=j+1;
            aux2=j-1;
            aux3=i+1;
            aux4=i-1;
            if (j==N-1)
            {
                aux1=0;
            }
            if (i==N-1)
            {
                aux3=0;
            }
            if (i==0)
            {
                aux4=N-1;
            }
            if (j==0)
            {
                aux2=N-1;
            }
            
            E = E + spines[i][j] * (spines[i][aux1] + spines[i][aux2] + spines[aux3][j] + spines[aux4][j]);



        }
        
    }
    return -0.5*E;
}

double correlacion(int spines[N][N], int a){
    double suma=0.0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
             if(i<N-a)
             {
                suma=suma+spines[i][j]*spines[i+a][j];
             }
            else
            {
                suma=suma+spines[i][j]*spines[i+a-N][j];
            }
            
        }
        
    }
    return suma/(N*N);   
}

double varianza(double vector[A], double media)
{
    double suma=0;
    for (int i = 0; i < A; i++)
    {
        suma=suma+pow((vector[i]-media),2);
        //printf("suma=%lf\n", suma);
    }
    return sqrt(suma);

}