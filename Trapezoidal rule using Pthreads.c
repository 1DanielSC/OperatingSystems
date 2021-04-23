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

void printArg(Args a){
    printf("Thead %d, de %f ate %f, com %d trapezios.\n"
            ,a.resultado,a.a,a.b,a.n);
}
float funcao(int func, float t) {
    if(func)return sin(t*2.0)+cos(t*5.0);
    else return 5.0;
}

int charParaInt(char *c){
    int ret = 0;
    int sinal = 0;
    if(*c=='-') {
        sinal  = 1;
        c++;
    }
    while (*c!='\0'){
        ret = ret*10 + *c - '0';
        c++;
    }
    if (sinal) ret*=-1;
    return ret;
}

void notacaoCientifica(float num){
    int expoente = 0;
    int neg = 0;
    if (num<0){
        num*=-1;
        neg=1;
    }
    while(!(num > 1 && num < 10) ) {
        if (num < 1){
            num*=10;
            expoente--;
        }else {
            num/=10;
            expoente++;
        }

    }
    if(neg) num*=-1;
    printf("Resultado: %.2fe%d\n",num,expoente);
}


void *trapezio(void *entra){
    Args * entrada = (Args *) entra;
    float h = (entrada->b - entrada->a)/entrada->n;
    float area;
    float x_i = entrada->a;

    area = funcao(entrada->func,entrada->a)+funcao(entrada->func,entrada->b);
    area /=2;
    
    for(int ii=0;ii<entrada->n-1;ii++){
        x_i += h;
        area += funcao(entrada->func,x_i);
    }
    
    area *=h;
    resultados[entrada->resultado] = area;
    pthread_exit(NULL);
}

void main(int argc, char **argv){

    int threads;
    int trapezios;
    int funcao;
    if(argc>1){
        funcao = *argv[1]-'0';
        if (funcao!= 0 && funcao != 1){
            printf("Codigo de funcao invalido.\n");
            return;
        }
        threads = charParaInt(argv[2]);
        if(threads<1){
            printf("Numero de threads invalido.\n");
            return;
        }
        trapezios = charParaInt(argv[3]);
        if(trapezios<1){
            printf("Numero de trapezios invalido.\n");
            return;
        }
        

    }else{
        printf("Funcoes disponiveis: 0 [constante] e 1 [trigonometrica]\n");
        do{
            printf("Digite o numero da funcao: ");
            scanf("%d",&funcao);
            if (funcao !=0 && funcao !=1){
                printf("Numero de funcao invalido, digite 0 ou 1");
            }
        }while(funcao!=0 && funcao !=1);
        do{
            printf("Digite o numero de threads: ");
            scanf("%d",&threads);
            if(threads<1){
                printf("Numero de threads invalido, digite um inteiro positivo.");
            }
        }while(threads<1);
        do{
            printf("Digite o numero de trapezios: ");
            scanf("%d",&trapezios);
            if(trapezios<1){
                printf("Numero de trapezios invalido, digite um inteiro positivo.");
            }
        }while(trapezios<1);
    }

    pthread_t *fios;
    fios = (pthread_t*)malloc(threads*sizeof(pthread_t));

    resultados = (float*)malloc(threads*sizeof(float));

    int trapezios_restantes = trapezios;
    int particao = trapezios/threads;

    Args * tempArg = (Args *) malloc(sizeof(Args)*threads);


    float parcela;
    if(funcao) parcela = 2.0*M_PI/threads;
    else parcela = 10.0/threads;

    for (int ii=0; ii<threads; ii++){
        tempArg[ii].a = ii*parcela;
        tempArg[ii].b = (ii+1)*parcela;
        if (ii == threads-1) tempArg[ii].n = trapezios_restantes;
        else {
            tempArg[ii].n = particao;
            trapezios_restantes -= particao;
        }

        tempArg[ii].func = funcao;
        tempArg[ii].resultado = ii;

        int status = pthread_create(&fios[ii],NULL,trapezio,&tempArg[ii]);
        printArg(tempArg[ii]);
        if (status != 0){
            printf("Erro ao criar thread\n");
            return;
        }
    }


    int ii = 0;
    void *temp;
    float resultado_final = 0.0;
    while(ii < threads){
        pthread_join(fios[ii],NULL);
        resultado_final += resultados[ii];
        ii++;
    }

    notacaoCientifica(resultado_final);
    free(tempArg);
    free(resultados);
    free(fios);
}
