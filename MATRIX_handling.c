#include "navigate.h"


/******************************************************************************
 * analyse_biggest_L_C_K
 *
 * Arguments: fp_input - ficheiro de entrada
 *            fp_output - ficheiro de saida
 *            max_LCK - estrutura que contem max_C, max_L e max_K
 *            
 *
 * Returns: void
 *
 * Description: Procura os maiores valores L, C e K para alocar a matriz apenas uma vez.
 *****************************************************************************/
void analyse_biggest_L_C_K(FILE* fp_input, FILE *fp_output, max_info* max_LCK) {
    int result;
    int temp_L, temp_C, l1, c1, temp_K=0, temp_value, temp_init_energy, temp_task_energy;

    //Lê a informação da linha com as intruções do problema
    while (1) {
        result = fscanf(fp_input, "%d %d %d %d %d %d %d", &temp_L, &temp_C, &temp_task_energy,&l1, &c1, &temp_K, &temp_init_energy);

        //Sai se detetar end of file
        if (result == EOF)
            break;

        //Sai caso nao consiga ler os 7 ints
        if (result != 7) {
            fclose(fp_input);
            fclose(fp_output);
            exit(0);
        }



        temp_K=abs(temp_K);


        //Substitui até encontrar o maior L, C e K do ficheiro
        if (temp_K > max_LCK->max_K) {
            max_LCK->max_K = temp_K;
        }

        if (temp_L > max_LCK->max_L) {
            max_LCK->max_L = temp_L;
        }

        if (temp_C > max_LCK->max_C) {
            max_LCK->max_C = temp_C;
        }


        //Faz skip ao problema
        for (int i = 0; i < temp_L; i++) {
            for (int j = 0; j < temp_C; j++) {
                temp_value = fscanf(fp_input, "%d", &temp_value);
                if(temp_value != 1) exit(0);
            }
        }
    }

    fseek(fp_input, 0, SEEK_SET);
}


/******************************************************************************
 * matrix_allocation
 *
 * Arguments: max_LCK - estrutura de dados máximos
 *            master_pointer - duplo apontador para inteiros que contém a energia de cada posição associada
 *            
 *
 * Returns: int
 *
 * Description: Aloca uma matriz dinamicamente de dimenções L x C ao qual possam ser contidos
 *               todos os exercicios no ficheiro. Devolve 0 em sucesso e 1 em falha.
 *****************************************************************************/
int matrix_allocation(max_info max_LCK, int ***master_pointer) {


    *master_pointer = (int **) malloc(max_LCK.allocated_row * sizeof(int *));
    //Caso não seja possivel alocar a memoria
    if (*master_pointer == NULL) {
        return 1;
    }

    for (int i = 0; i < max_LCK.allocated_row; i++) {
        (*master_pointer)[i] = (int *) malloc(max_LCK.allocated_colum * sizeof(int));
        //Caso não seja possivel alocar as colunas
        if ((*master_pointer)[i] == NULL) {
            return 1;
        }
    }
    return 0;
}

/******************************************************************************
 * calculate_matrix_boundries
 *
 * Arguments: max_LCK - estrutura de dados máximos
 *
 * Returns: void
 *
 * Description: calcula os parametros para a alocação da matriz dependendo das dimenções do L, C e o k.
 *              allocated_row: tamanho de linhas a alocar
 *              allocated_colum: tamanho de colunas a alocar
 *****************************************************************************/
void calculate_matrix_boundries(max_info* max_LCK) {
    int k_len = 2 * (max_LCK->max_K) + 1;

    //Consoante os valores maximos, escolhe o melhor tipo de alocação a fazer
    if (((max_LCK->max_L * max_LCK->max_C) <= (k_len * k_len)) || (max_LCK->max_K == 0)) {
        max_LCK->allocated_row = max_LCK->max_L;
        max_LCK->allocated_colum = max_LCK->max_C;
    } else {
        max_LCK->allocated_row = k_len;
        max_LCK->allocated_colum = k_len;

    }
}



/******************************************************************************
 * exit_condition()
 *
 * Arguments: max_L
 *            master_pointer - array de duas dimensões com a data da matriz dos problemas a analisar
 *            fp_input - file poiter para o ficheiro de input
 *            fp_output - file poiter para o ficheiro de output
 *
 * Returns: (void)
 *
 * Description: caso o programa tenha de parar a sua execução, esta função trata de fazer free a toda a memoria alocada
 *****************************************************************************/
void exit_condition(max_info max_LCK, int **master_pointer, FILE *fp_input, FILE *fp_output) {


    if (master_pointer != NULL) {
        for (int i = 0; i < max_LCK.allocated_row; i++) {
            //Caso falha na alocação
            if (master_pointer[i] != NULL) {
                free(master_pointer[i]);
            }
        }
        free(master_pointer);
        master_pointer = NULL;
    }

    //Fecha os ficheiros
    if (fp_input != NULL) fclose(fp_input);
    if (fp_output != NULL) fclose(fp_output);

    exit(0);
}
