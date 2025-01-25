#include "navigate.h"




/******************************************************************************
 * main()
 *
 * Arguments: argc - número de argumentos de entrada
 *            argv - posição dos argumentos de entrada
 *
 * Returns: (void)
 *
 * Description: Executa o programa
 *****************************************************************************/
int main(int argc, char **argv) {

    //Inicialização da estrututa a zeros
    label cabecalho = {0};
    limits matrix_limits={0};
    max_info max_LCK={0};


    char* filename = NULL;
    int **master_pointer = NULL;

    analyse_arguments(argc, argv);
    FILE *fp_input = init_file(argv, "r");

    create_output_name(argv, &filename);
    FILE *fp_output = fopen(filename, "w");
    //Se não for possível abrir o ficheiro
    if (fp_output == NULL) {

        free(filename);
        exit_condition(max_LCK, master_pointer, fp_input,NULL);
    }
    free(filename);

    //Percorre o ficheiro todo de modo a encontrar os maiores valores de L e C
    analyse_biggest_L_C_K(fp_input, fp_output, &max_LCK);
    calculate_matrix_boundries(&max_LCK);


    //Aloca uma matrix do tamanho do maior valor de L x C  ou de 2k+1 de lado consoante a informação recolhida
    if (matrix_allocation(max_LCK,&master_pointer) == 1) {
        //Caso não seja possóvel alocar
        exit_condition(max_LCK,master_pointer, fp_input, fp_output);
    }


    //Loop para executar um problema de cada vez
    while(1){

        if (analyse_cabecalho(&cabecalho, fp_input, fp_output) == 1) {
            exit_condition(max_LCK, master_pointer, fp_input, fp_output);
        }


        //Caso  o cabeçalho for ben posto, procegue
        if(invalid_cabecalho(&cabecalho,fp_output )==0){
            calculate_matrix_values(&max_LCK, &cabecalho);
            master_pointer = get_matrix_info(master_pointer, &cabecalho, fp_input, max_LCK, &matrix_limits);
            choose_Task(master_pointer, &cabecalho, fp_output,&matrix_limits, max_LCK);

        //Caso o cabeçalho foi mal posto, faz skip ao problema
        }else {
            int temp_value;

            for (int i = 0; i < cabecalho.L; i++) {
                for (int j = 0; j < cabecalho.C; j++) {
                    temp_value = fscanf(fp_input, "%d", &temp_value);
                }
            }
        }


         //Se for detetato EOF, sai do ciclo
        if (feof(fp_input)) {
            break;
        }

        fprintf(fp_output,"\n");

    }

    //Faz free de tudo antes de terminar a execução do programa
    exit_condition(max_LCK ,master_pointer, fp_input, fp_output);
    return 0;
}

