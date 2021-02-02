#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>


struct Pessoa{
	time_t nasce;
	time_t morte;
};


void Neto1_processo(){
	struct Pessoa neto1;

	time_t raw_time = time(NULL);
	neto1.nasce = raw_time;
	
	struct tm *horario = localtime(&raw_time);

	printf("Neto1 nasceu! Horario do nascimento: %02d:%02d:%02d\n", horario->tm_hour, horario->tm_min, horario->tm_sec);

	sleep(12);


	raw_time = time(NULL);
	neto1.morte = time(NULL);
	

	printf("\n");
	printf("Neto1 faleceu!\n");
	printf("Informacoes sobre o Neto1: \n");

	printf("\tHorario do nascimento:  %02d:%02d:%02d\n", horario->tm_hour, horario->tm_min, horario->tm_sec);

	horario = localtime(&raw_time);

	printf("\tHorario da morte: %02d:%02d:%02d\n", horario->tm_hour, horario->tm_min, horario->tm_sec);
	printf("\tTempo de vida Neto1: %d anos\n", (int) difftime(neto1.morte, neto1.nasce));

	printf("\n");
	exit(0);
}


void Filho1_processo(){
	struct Pessoa filho1;
	filho1.nasce = time(NULL);

	time_t raw_time = time(NULL);
	
	struct tm *horario;
	horario = localtime(&raw_time);
	printf("Filho1 nasceu! Horario do nascimento: %02d:%02d:%02d \n", horario->tm_hour, horario->tm_min, horario->tm_sec);

	sleep(12);
	//Filho1 com 12 anos, Neto1 vai nascer
		
	pid_t Neto1 = fork();//Neto1 nasce

	if(Neto1 < 0){
			printf("Erro ao criar o Neto1\n");
			exit(-1);
	}
	else if (Neto1 == 0)
	{	
		Neto1_processo();
	}else{


		sleep(18);
		//Filho1 com 30 anos

		printf("\n");
		printf("Filho1 morreu.\n");
		printf("Informacoes sobre Filho1: \n");
		printf("\tHorario de nascimento: %02d:%02d:%02d\n", horario->tm_hour, horario->tm_min, horario->tm_sec);

		raw_time = time(NULL);
		horario = localtime(&raw_time);
		filho1.morte = raw_time;

		printf("\tHorario da morte: %02d:%02d:%02d\n", horario->tm_hour, horario->tm_min, horario->tm_sec);
		printf("\tTempo de vida Filho1: %d anos\n", (int) difftime(filho1.morte, filho1.nasce));

		printf("\n");
		exit(0);
	}

}



void Neto2_processo(){

	time_t raw_time = time(NULL);
	struct tm *horario = localtime(&raw_time);

	struct Pessoa neto2;
	neto2.nasce = raw_time;

	printf("Neto2 nasceu! Horario do nascimento:  %02d:%02d:%02d\n", horario->tm_hour, horario->tm_min, horario->tm_sec);

	sleep(18);
	
	printf("\n");

	printf("Neto2 faleceu!\n");
	printf("Informacoes sobre o Neto2: \n");
	printf("\tHorario do nascimento: %02d:%02d:%02d\n", horario->tm_hour, horario->tm_min, horario->tm_sec);

	raw_time = time(NULL);
	neto2.morte = raw_time;
	horario = localtime(&raw_time);
	printf("\tHorario da morte: %02d:%02d:%02d\n", horario->tm_hour, horario->tm_min, horario->tm_sec);
	printf("\tTempo de vida: %d anos\n", (int) difftime(neto2.morte, neto2.nasce));

	printf("\n");
	exit(0);

}


void Filho2_processo(){
	
	time_t raw_time = time(NULL);
	struct tm *horario = localtime(&raw_time);

	struct Pessoa filho2;
	filho2.nasce = raw_time;

	printf("Filho2 nasceu! Horario do nascimento: %02d:%02d:%02d\n", horario->tm_hour, horario->tm_min, horario->tm_sec);

	sleep(14);

	//Neto2 nasce
	pid_t Neto2 = fork();

	if (Neto2 < 0){
		printf("Erro ao criar o Neto2\n");
		exit(-1);

	}else if (Neto2 == 0){
		Neto2_processo();

	}else{

		sleep(16);

		printf("\n");
		printf("Filho2 faleceu.\n");
		printf("Informacoes sobre o Filho2:\n");
		printf("\tHorario de nascimento: %02d:%02d:%02d \n", horario->tm_hour, horario->tm_min, horario->tm_sec);

		raw_time = time(NULL);
		filho2.morte = time(NULL);
		horario = localtime(&raw_time);

		printf("\tHorario da morte: %02d:%02d:%02d\n", horario->tm_hour, horario->tm_min, horario->tm_sec);
		printf("\tTempo de vida: %d anos\n", (int) difftime(filho2.morte, filho2.nasce));
		printf("\n");
		exit(0);
	}
	
}



int main(){

	time_t raw_time = time(NULL);

	struct Pessoa pai;
	pai.nasce = raw_time;

	struct tm *horario = localtime(&raw_time);
	
	printf("Pai nasceu! Hora do nascimento do pai: %02d:%02d:%02d \n", horario->tm_hour, horario->tm_min, horario->tm_sec);


	sleep(14);
		//Pai com 14 anos

	pid_t Filho1 = fork();

	if (Filho1 < 0){
		printf("Erro ao criar o Filho1\n");
		exit(-1);

	}else if (Filho1 == 0){ //Processo Filho1

		Filho1_processo();

	}else{

	//Processo pai
		sleep(2); //Pai com 16 anos

		pid_t Filho2 = fork();

		if (Filho2 < 0){
			printf("Erro ao criar o Filho2\n");
			exit(-1);

		}else if (Filho2 == 0){ //Processo Filho2

			Filho2_processo();
			
		}else{
		//Processo Pai


			sleep(10);//Pai com 26 anos
			printf("Pai com 26 anos!\n");
			//Agora o Filho1 deve ter o Neto1

			sleep(4); //Pai com 30 anos
			printf("Pai com 30 anos!\n");
			//Agora o Filho2 deve ter o Neto2

			sleep(30);

			//Morte do pai
			raw_time = time(NULL);
			pai.morte = raw_time;

			printf("\n");

			printf("Pai faleceu. \n");
			printf("Informacoes sobre o pai:\n");

			printf("\tHorario do nascimento: %02d:%02d:%02d \n", horario->tm_hour, horario->tm_min, horario->tm_sec);

			horario = localtime(&raw_time);
			printf("\tHorario da morte do pai: %02d:%02d:%02d \n", horario->tm_hour, horario->tm_min, horario->tm_sec);
			printf("\tTempo de vida: %d anos\n", (int) difftime(pai.morte, pai.nasce));
			printf("\n");

		}

	}

	return 0;
}
