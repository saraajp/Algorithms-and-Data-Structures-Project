#include "navigate.h"
/******************************************************************************
 * analyse_arguments()
 *
 * Arguments: argc - número de argumentos de entrada
 *            argv - posição dos argumentos de entrada
 *
 * Returns: (void)
 *
 * Description: Analisa o número de argumentos lidos assim como a extenção associada
 *            ao argumento, em caso de erro, invoca exit(0).
 *****************************************************************************/
void analyse_arguments(int argc, char **argv) {
    if (argc != 2) exit(0);

    char input_ext[] = INPUT_EXT;
    int i = strlen(argv[1]) - 1, j = strlen(INPUT_EXT) - 1;

    for (; i >= 0 && j >= 0; i--, j--) {
        if (argv[1][i] != input_ext[j]) exit(0);
    }
}



/******************************************************************************
 * init_file()
 *
 * Arguments: argv - vetor de argumentos de linha de comando
 *            mode - modo de acesso ao ficheiro. Ex: "r","w"
 *
 * Returns: File -  apontador para o ficheiro aberto no respetivo modo
 *
 * Description: Abre o ficheiro no modo pretendido, em caso de erro, invoca exit(0).
 *****************************************************************************/
FILE* init_file(char** argv, char* mode) {
    FILE* File = fopen(argv[1], mode);

    //Caso não seja possivel abrir
    if (File == NULL) {
        exit(0);
    }
    return File;
}

/******************************************************************************
 * create_output_name()
 *
 * Arguments: argv - vetor de argumentos de linha de comando
 *            p_filename - apontador para o nome do ficheiro de escrita
 *
 * Returns: (void)
 *
 * Description: Aloca memória e cria um nome novo para o ficheiro de output, eliminando a extenção
 *             .1maps e criando a extenção .sol1maps atribuindo o endereço do pointeiro
 *              ao qual a memória foi alocada para p_filename. Em caso de falha invoca exit(0)
 // // *****************************************************************************/
void create_output_name(char **argv, char**p_filename) {
    *p_filename = NULL;

    char *name_string = (char *)malloc(strlen(argv[1]) + strlen(OUTPUT_EXT) + 1);
    //Caso falha na alocação
    if (name_string == NULL) {
        exit(0);
    }

    strcpy(name_string, argv[1]);
    char *aux = strrchr(name_string, '.');
    if (aux != NULL) {
        *aux = '\0';
    }
    strcat(name_string, OUTPUT_EXT);


    *p_filename = name_string;
}
