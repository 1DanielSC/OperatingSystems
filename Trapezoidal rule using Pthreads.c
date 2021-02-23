#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>

float *resultados;

typedef struct argumentos{
    float a;
    float b;
    int func;
    int n;
    int resultado;
}Args;

float funcao_constante(float t) {return 5.0;}
float funcao_senoidal(float t) {return sin(t*2.0)+cos(t*5.0);}

int charParaInt(char *c){
    int ret = 0;
    while (*c!='\0'){
        ret =ret *10 +*c - 48;
        c++;
    }
    return ret;
}

void notacaoCientifica(float num){
    int expoente = 0;
    while((num < 1 || num >= 10) && (num > -1 || num <= -10)) {

        if (num < 1 && num > -1){
            num*=10;
            expoente--;

        }else {
            num/=10;
            expoente++;
        }

    }
    printf("Resultado: %.2fe%d\n",num,expoente);
}


void *trapezio(void *entra){
    Args * entrada = (Args *) entra;
    float h = (entrada->b - entrada->a)/entrada->n;
    float area;
    float x_i;

    if (entrada->func){

        area = funcao_constante(entrada->a)+funcao_constante(entrada->b);
        area /=2;

        x_i = entrada->a+h;

        while(x_i<entrada->b){
            area +=funcao_constante(x_i);
            x_i+=h;
        }

        area *=h;

    } else {

        area = funcao_senoidal(entrada->a)+funcao_senoidal(entrada->b);
        area /=2;

        x_i = entrada->a + h;

        while(x_i < entrada->b){
            area +=funcao_senoidal(x_i);
            x_i += h;
        }

        area *=h;
    }

    resultados[entrada->resultado] = area;
    pthread_exit(NULL);
}

void main(int argc, char **argv){

    int threads;
    int trapezios;

    int funcao;
    if(argc>1){
        if(argv[1][0]=='c') funcao = 0;
        else funcao = 1;
            
        threads = charParaInt(argv[2]);
        trapezios = charParaInt(argv[3]);

    }else{
        printf("Funcoes disponiveis: 0 [constante] e 1 [trigonometrica]\n");
        printf("Digite o numero da funcao: ");
        scanf("%d",&funcao);

        printf("Digite o numero de threads: ");
        scanf("%d",&threads);

        printf("Digite o numero de trapezios: ");
        scanf("%d",&trapezios);
    }

    pthread_t *fios;
    fios = (pthread_t*)malloc(threads*sizeof(pthread_t));

    resultados = (float*)malloc(threads*sizeof(float));

    int trapezios_restantes = trapezios;
    int particao = trapezios/threads;

    Args * tempArg = (Args *) malloc(sizeof(Args)*threads);


    int parcela;
    if(funcao) parcela = 2.0*M_PI/threads;
    else parcela = 10.0/threads;


    for (int ii=0; ii<threads; ii++){

        if (trapezios_restantes < particao) tempArg[ii].n = trapezios_restantes;
        else tempArg[ii].n = particao;


        tempArg[ii].a = 0.0 + ii*parcela;
        tempArg[ii].b = parcela + ii*parcela;

        tempArg[ii].func = funcao;
        tempArg[ii].resultado = ii;

        int status = pthread_create(&fios[ii],NULL,trapezio,&tempArg[ii]);
        if (status != 0){
            printf("Erro ao criar thread\n");
            return;
        }
        trapezios_restantes -= particao;
    }


    int ii = 0;
    while(ii < threads){
        pthread_join(fios[ii],NULL);
        ii++;
    }

    ii = 0;
    float resultado_final = 0.0;
    while(ii < threads){
        resultado_final += resultados[ii];
        ii++;
    }

    notacaoCientifica(resultado_final);

    free(fios);
    free(resultados);
    free(tempArg);
}
