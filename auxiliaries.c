#include "navigate.h"

/******************************************************************************
 * init_visited()
 *
 * Arguments: max_LCK - Estrutura de dados que contém o número de linhas e colunas alocadas.
 *
 * Returns: bool**
 * 
 * Description: Inicialização de marcadores de visitado
 *****************************************************************************/
bool** init_visited(max_info max_LCK) {
    bool** visited = (bool**)malloc(max_LCK.allocated_row * sizeof(bool*));
    for (int i = 0; i < max_LCK.allocated_row; i++) {
        visited[i] = (bool*)malloc(max_LCK.allocated_colum * sizeof(bool));
    }

    for (int i = 0; i < max_LCK.allocated_row; i++) {
        for (int j = 0; j < max_LCK.allocated_colum; j++) {
            visited[i][j] = false;
        }
    }

    return visited;
}

/******************************************************************************
 * free_visited()
 *
 * Arguments: max_LCK - Estrutura de dados que contém as linhas alocadas
 *            visited - Matriz de marcadores de visitado.
 *
 * Returns: bool**
 * 
 * Description: Inicialização de marcadores de visitado
 *****************************************************************************/
void free_visited(max_info max_LCK, bool** visited){
    for (int i = 0; i < max_LCK.allocated_row; i++) {
        free(visited[i]);
    }
    free(visited);
}

/******************************************************************************
 * init_best_path()
 *
 * Arguments: cabecalho - Cabeçalho do problema a executar
 *
 * Returns: init**
 * 
 * Description: Alocação de memória para definir o melhor caminho
 *****************************************************************************/
int** init_best_path(label* cabecalho) {
    int** best_path = (int**)malloc(cabecalho->k * sizeof(int*));
    for (int i = 0; i < cabecalho->k; i++) {
        best_path[i] = (int*)malloc(2 * sizeof(int));
    }

    return best_path;
}

/******************************************************************************
 * free_best_path()
 *
 * Arguments: cabecalho - Cabeçalho do problema a executar
 *            best_path - vetor do melhor caminho
 *
 * Returns: void
 * 
 * Description: Liberta memória do vetor do melhor caminho
 *****************************************************************************/
void free_best_path(label *cabecalho, int **best_path) {
    for (int i = 0; i < cabecalho->k; i++) {
        free(best_path[i]);
    }
    free(best_path);
}

/******************************************************************************
 * init_path()
 *
 * Arguments: cabecalho - Cabeçalho do problema a executar
 *
 * Returns: init**
 * 
 * Description: Alocação de memória para definir um caminho genérico
 *****************************************************************************/
int** init_path(label* cabecalho) {
    int** path = (int**)malloc(cabecalho->k * sizeof(int*));
    for (int i = 0; i < cabecalho->k; i++) {
        path[i] = (int*)malloc(2 * sizeof(int));
    }

    return path;
}

/******************************************************************************
 * free_path()
 *
 * Arguments: cabecalho - Cabeçalho do problema a executar
 *            path - vetor do melhor caminho
 *
 * Returns: void
 * 
 * Description: Liberta memória do vetor do caminho genérico
 *****************************************************************************/
void free_path(label* cabecalho, int** path){
    for (int i = 0; i < cabecalho->k; i++) {
        free(path[i]);
    }
    free(path);
}


/******************************************************************************
 * return_max()
 *
 * Arguments: max_LCK - Estrutura de dados que contém o número de linhas e colunas alocadas.
 *
 * Returns: int
 *
 * Description: Retorna o valor maximo entre dois limites
 *****************************************************************************/
int return_max(int a, int b) {
    return (a >= b) ? a : b;
}

/******************************************************************************
 * return_min()
 *
 * Arguments: max_LCK - Estrutura de dados que contém o número de linhas e colunas alocadas.
 *
 * Returns: int
 *
 * Description: Retorna o valor minimo entre dois ints
 *****************************************************************************/
int return_min(int a, int b) {
    return (a <= b) ? a : b;
}
