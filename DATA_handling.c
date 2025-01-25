#include "navigate.h"

/******************************************************************************
 * find_matrix_limits()
 *
 * Arguments: matrix_limits - limites da matriz alocada para um respetivo problema
 *            cabecalho - respetivo cabeçalho
 *            allocation_type - regime de alocação da matriz: (2) 2k+1 x 2k+1  ou  (1) L x C
 *
 * Returns: (void)
 *
 * Description: Consoante o tipo de alocação decidido, vai calcular os limites para
 *              as 4 direções da matriz alocada em relação ao ponto inicial
 *****************************************************************************/
void find_matrix_limits(limits* matrix_limits, label* cabecalho, int allocation_type){//YES

    //Se for alocada uma matriz de lado 2k+1, estes serão os seus limites
    if(allocation_type==2){
        matrix_limits->up= return_min(cabecalho->l1 - 1, abs(cabecalho->k));
        matrix_limits->down= return_min(cabecalho->L - cabecalho->l1, abs(cabecalho->k));
        matrix_limits->left= return_min(cabecalho->c1 - 1, abs(cabecalho->k));
        matrix_limits->right= return_min(cabecalho->C - cabecalho->c1, abs(cabecalho->k));

    //Se for alocada uma matriz L x C, estes serão os seus limites
    }else {
        matrix_limits->up= cabecalho->l1 - 1;
        matrix_limits->down= cabecalho->L - cabecalho->l1;
        matrix_limits->left= cabecalho->c1 - 1;
        matrix_limits->right= cabecalho->C - cabecalho->c1;
    }

}


/******************************************************************************
 * get_matrix_info
 *
 * Arguments: master_pointer - duplo apontador para inteiros que contém a energia de cada posição associada
 *            cabecalho - apontador para struct que contém as informações sobre cabeçahos
 *            fp_input - apontador para ficheiros de input
 *            max_LCK - estrutura de dados máximos
 *            matrix_limits - condições limite para cada exercício
 *
 * Returns: int** master_pointer
 *
 * Description: Guarda as informações do ficheiro para a matriz. Invoca exit(0) em caso de falha
 *****************************************************************************/
int **get_matrix_info(int **master_pointer, label *cabecalho, FILE *fp_input, max_info max_LCK, limits* matrix_limits) {
    int temp;
    int lim_horizontal, lim_vertical;


    //Guarda a informção da matriz toda e calcula os seus limites para futuro uso
    int k_len = (2 * max_LCK.max_K +1);
    if (((max_LCK.max_L * max_LCK.max_C) <= (k_len *k_len))|| (max_LCK.max_K==0)) {
        for (int i = 0; i < cabecalho->L; i++) {
            for (int j = 0; j < cabecalho->C; j++) {
                temp=fscanf(fp_input, "%d", &master_pointer[i][j]);
                if(temp != 1) exit(0);
            }
        }

        find_matrix_limits(matrix_limits, cabecalho,1);




    //Guada a informação do quadrado que contém o losangulo, com o auxilio dos limites
    } else {

        find_matrix_limits(matrix_limits, cabecalho,2);

        //Mudanças de coordenadas
        int new_origin_L = cabecalho->l1 - matrix_limits->up -1;
        int new_origin_C = cabecalho->c1 - matrix_limits->left -1;

        lim_horizontal = matrix_limits->left + matrix_limits->right + 1;
        lim_vertical = matrix_limits->up + matrix_limits->down + 1;

        int a = 0, b = 0, temp2 = 0;

        for (int i = 0; i < cabecalho->L; i++) {
            for (int j = 0; j < cabecalho->C; j++) {
                temp2 = fscanf(fp_input, "%d", &temp);
                if(temp2 != 1) exit(0);

                if (i >= new_origin_L && i < new_origin_L + lim_vertical && j >= new_origin_C && j < new_origin_C + lim_horizontal) {
                    master_pointer[a][b] = temp;
                    b++;
                }
            }

            if (b >= lim_horizontal) {
                b = 0;
                a++;
            }

        }

    }
    return master_pointer;
}

/******************************************************************************
 * invalid_cabecalho
 *
 * Arguments: cabecalho - apontador para struct que contém as informações sobre cabeçahos
 *            fp_output - apontador para o ficheiro de output
 *
 * Returns: 0 em caso de sucesso e 1 em caso de falha
 *
 * Description: Avalia o cabeçalho do exercício
 *****************************************************************************/
int invalid_cabecalho(label *cabecalho, FILE *fp_output){

    //Problema mal posto
    if(cabecalho->k<=0 || cabecalho->L <= 0 || cabecalho->C <= 0 || cabecalho->l1 <= 0 || cabecalho->c1 <= 0 || cabecalho->l1 > cabecalho->L || cabecalho->c1 > cabecalho->C || cabecalho->Energy_init <=0 ||cabecalho->Task_Energy==0  ||cabecalho->Task_Energy==-1 || cabecalho->Task_Energy<-2){
        fprintf(fp_output, "%d %d %d %d %d %d %d\n", cabecalho->L, cabecalho->C, cabecalho->Task_Energy, cabecalho->l1, cabecalho->c1, cabecalho->k, cabecalho->Energy_init);
        return 1;
    }

    //Problema impossível
    if (cabecalho->L *cabecalho->C <= cabecalho->k) {
        fprintf(fp_output, "%d %d %d %d %d %d %d -1\n", cabecalho->L, cabecalho->C, cabecalho->Task_Energy, cabecalho->l1, cabecalho->c1, cabecalho->k, cabecalho->Energy_init);
        return 1;

    }

    return 0;
}

/******************************************************************************
 * analyse_cabecalho
 *
 * Arguments: fp_input - ficheiro de entrada
 *            fp_output - ficheiro de saida
 *            cabecalho - estrutura que contem as informações do problema
 *            
 *
 * Returns: int** master_pointer
 *
 * Description: Aloca uma matriz dinamicamente de dimenções L x C ao qual possam ser contidos
 *               todos os exercicios no ficheiro. Em caso de sucesso devolve 0, senão devolve 1.
 *****************************************************************************/
int analyse_cabecalho(label *cabecalho, FILE *fp_input, FILE*fp_output) {
    while (1) {

        //Lê a informação da linha com as intruções do problema
        int result = fscanf(fp_input, "%d %d %d %d %d %d %d", &cabecalho->L, &cabecalho->C, &cabecalho->Task_Energy, &cabecalho->l1, &cabecalho->c1, &cabecalho->k,&cabecalho->Energy_init);


        //Sai caso detete end of file
        if (result == EOF) {
            return 1;
        }

        //Sai caso nao consigue ler os 7 ints
        if (result != 7) {
            return 1;
        }


        break;
    }
    return 0;
}




/******************************************************************************
 * calculate_matrix_values()
 *
 * Arguments:   max_LCK - estrutura que contém o l1 e c1 convertidos para 0 index.
 *              cabecalho - estrutura que contem as informações do problema
 *
 *
 * Returns: void
 *
 * Description: Caso a matriz alocada tenha sido L x C. Os pontos iniciais serão obrigatóriamente
 *              l1 - 1 e c1 - 1. Se não, dependendo da alocação e da localização dos pontos, estes
 *              terão uma distância de k ou menos caso o ponto esteja a menos de k passos do limite
 *               da matriz.
 *****************************************************************************/
void calculate_matrix_values(max_info* max_LCK, label* cabecalho) {
    int k_len = 2 * (max_LCK->max_K) + 1;

    //Com a informação de max_LCK coneguimos fazer a conversão das coordenadas que temos que utilizar

    //Mudança de coordenadas para a matriz de tamanho L x C
    if (((max_LCK->max_L * max_LCK->max_C) <= (k_len * k_len)) || (max_LCK->max_K == 0)) {
        max_LCK->new_l1 = cabecalho->l1 - 1;
        max_LCK->new_c1 = cabecalho->c1 - 1;

    //Mudança de coordenadas para a matriz de lado 2k+1
    } else {
        max_LCK->new_l1 = return_min(cabecalho->l1 - 1, cabecalho->k);
        max_LCK->new_c1 = return_min(cabecalho->c1 - 1, cabecalho->k);
    }
}
