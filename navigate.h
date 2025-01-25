#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


#define INPUT_EXT ".maps"
#define OUTPUT_EXT ".solmaps"

typedef struct _label {
    int L, C;               //Nº de linhas e colunas da matriz
    int l1, c1;             //Coordenadas iniciais
    int k;                  //K passos que podemos dar
    int Task_Energy;        //Energia da task
    int Energy_init;        //Energia inicial
    int result;             //Resultado
} label;


typedef struct _max_info{
    int max_L;              //Maior L da matriz
    int max_C;              //Maior C da matriz
    int max_K;              //Maior K da matrix
    int allocated_row;      //Nº de linhas a alocar
    int allocated_colum;    //Nº de colunas a alocar
    int new_l1;             //Coordenada l da mudança de variável
    int new_c1;             //Coordenada c da mudança de variável
    int biggest_energy_cell;//Maior valor do losangulo
    int sum_energy_cells;   //Somatorio dos valores das células positivas
} max_info;


//Limites em relação às posiçoes iniciais
typedef struct _limits{
    int up;                 //Limite superior
    int down;               //Limite inferior
    int left;               //Limite esquedo
    int right;              //Limite direiro
} limits;





int return_max(int a, int b);
int return_min(int a, int b);
void analyse_arguments(int argc, char **argv);
FILE* init_file(char** argv, char* mode);
void create_output_name(char **argv, char**filename);
void analyse_biggest_L_C_K(FILE* fp_input, FILE *fp_output, max_info* max_LCK);
int matrix_allocation(max_info max_LCK, int ***master_pointer);
int analyse_cabecalho(label *cabecalho, FILE *fp_input, FILE* fp_output);
void exit_condition(max_info max_LCK, int **master_pointer, FILE *fp_input, FILE *fp_output);
int **get_matrix_info(int **master_pointer, label *cabecalho, FILE *fp_input, max_info max_LCK, limits* matrix_limits);
void choose_Task(int **master_pointer, label *cabecalho, FILE *fp_output, limits* matrix_limits, max_info max_LCK);
void find_matrix_limits(limits* matrix_limits, label* cabecalho, int allocation_type);
void calculate_matrix_values(max_info* max_LCK, label* cabecalho);
void calculate_matrix_boundries(max_info* max_LCK);
int invalid_cabecalho(label *cabecalho, FILE *fp_output);
void old_Tasks(int** master_pointer, label *cabecalho, max_info* max_LCK, limits* matrix_limits);
bool worth_continue( label* cabecalho, max_info max_LCK, int current_energy, int remaining_steps, int* best_energy);
bool is_valid(int x, int y, limits* matrix_limits, bool** visited);
bool DFS(int x, int y, int steps, int energy, int** master_pointer, bool** visited, int** path, int** best_path, max_info max_LCK, label* cabecalho, FILE* fp_output, int* max_energy, int task_type, limits* matrix_limits);
bool** init_visited(max_info max_LCK);
void free_visited(max_info max_LCK, bool** visited);
int** init_best_path(label* cabecalho);
void free_best_path(label *cabecalho, int **best_path);
int** init_path(label* cabecalho);
void free_path(label* cabecalho, int** path);
void choose_Task(int **master_pointer, label *cabecalho, FILE *fp_output, limits* matrix_limits, max_info max_LCK);
int invalid_cabecalho(label *cabecalho, FILE *fp_output);

