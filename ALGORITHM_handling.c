#include "navigate.h"


/******************************************************************************
* old_Tasks()
*
* Arguments: matrix_limits - limites da matriz alocada para um respetivo problema
*            cabecalho - respetivo cabeçalho
*            master_pointer - duplo apontador para as posições do mapa
*            max_LCK - estrutura de dados máximos
*
* Returns: (void)
*
* Description: Preenche alguns dos dados da estrutura max_LCK: biggest_energy_cell e sum_energy_cells. Posteriormente servirá
*              de bound. Esta função realiza as Tasks 1 e 2 da submição intermédia, mas na mesma leitura.
*****************************************************************************/
void old_Tasks(int** master_pointer, label *cabecalho, max_info* max_LCK, limits* matrix_limits) {
    int sum = 0, max = 0;
    int aux_l1, aux_c1;


     //Itera para ler a informação das posiçoes da matriz alocada
    for (int i = 0; i <= matrix_limits->up + matrix_limits->down; i++) {
        for (int j = 0; j <= matrix_limits->left + matrix_limits->right; j++) {

            //Conversões para garantir  não lê fora dos limites da matriz
            aux_l1 = max_LCK->new_l1 - matrix_limits->up + i;
            aux_c1 = max_LCK->new_c1 - matrix_limits->left + j;


            //Vê se a distancia desse dado ponto está a uma distancia de k
            //ou seja, se pertence ao losango para não contabilizar "lixo"
            int k_distance = abs(i - max_LCK->new_l1) + abs(j - max_LCK->new_c1);

            //Se estiver a uma distancia de k passos, a energia for positiva e não for a coordenada inicial
            if (k_distance <= cabecalho->k && master_pointer[aux_l1][aux_c1] > 0 && !(i == max_LCK->new_l1 && j == max_LCK->new_c1)) {
                sum += master_pointer[aux_l1][aux_c1];
                if (master_pointer[aux_l1][aux_c1] > max) {
                    max = master_pointer[aux_l1][aux_c1];
                }
            }
        }
    }

    max_LCK->biggest_energy_cell = max;      //Incrementa o somatório
    max_LCK->sum_energy_cells = sum;        //Novo máximo


}

/******************************************************************************
 * worth_continue()
 *
 * Arguments: cabecalho - respetivo cabeçalho
 *            max_LCK - estrutura de dados máximos
 *            remaining_steps - k passos que ainda faltam dar
 *            current_energy - energia atual
 *            best_energy - melhor energia encontrada no caminho
 *
 * Returns: bool
 *
 * Description: Assume se valerá a pena continuar a partir da soma da energia com o produto entre os passos restantes e a
 *              maior energia encontrada.
 *****************************************************************************/
bool worth_continue( label* cabecalho, max_info max_LCK, int current_energy, int remaining_steps, int* best_energy) {


      //Energia maxima que podemos vir a alcançar
    int max_potential_energy = max_LCK.biggest_energy_cell *remaining_steps;


    //Se não for suficiente, retorna falso -> não há esperança em encontrar um caminho
    if (cabecalho->Task_Energy != -2) { // Task 1
        if (current_energy + max_potential_energy < cabecalho->Task_Energy) {
            return false;
        }
    } else { // Task 2
        if (current_energy + max_potential_energy <= *best_energy) {
            return false;
        }
    }

    //Se for true, o programa deve continuar a busca
    return true;
}

/******************************************************************************
 * is_valid()
 *
 * Arguments: x - coordenada atual de l
 *            y - coordenada atual de c
 *            matrix_limits - offsets da matriz para o respetivo cabeçalho
 *            visited - matriz de ocorrências do mapa.
 *
 * Returns: bool
 * 
 * Description: Indica se a posição apresentada deve ser tomada.
 *****************************************************************************/
bool is_valid(int x, int y, limits* matrix_limits, bool** visited) {
    //Conversão
    int rows = matrix_limits->up + matrix_limits->down + 1;
    int cols = matrix_limits->left + matrix_limits->right + 1;


    return (x >= 0 && x < rows && y >= 0 && y < cols && !visited[x][y]);
}

/******************************************************************************
 * DFS()
 *
 * Arguments: x
 *            y
 *            steps - Número atual de passos
 *            energy - Energia atual do aluno
 *            master_pointer - Duplo apontador para o mapa necessário para o problema
 *            visited - Duplo apontador para marcadores de posições visitadas.
 *            path - Apontador para um caminho a ser percorrido
 *            best_path - Melhor caminho percorrido
 *            max_LCK - Estrutura para dados que contêm máximos
 *            cabecalho - Cabeçalho do problema
 *            fp_output - apontador para o ficheiro de output
 *            matrix_limits - Limites da matriz a seguir no seguinte problema
 *            max_energy - Energia máxima encontrada
 *            task_type - Tarefa a executar.
 *
 * Returns: bool
 * 
 * Description: Algoritmo DFS recursivo.
 *****************************************************************************/
bool DFS(int x, int y, int steps, int energy, int** master_pointer, bool** visited, int** path, int** best_path, max_info max_LCK, label* cabecalho, FILE* fp_output, int* max_energy, int task_type, limits* matrix_limits) {
    int dir[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    int offtset_l = cabecalho->l1 - max_LCK.new_l1;
    int offtset_c = cabecalho->c1 - max_LCK.new_c1;
    int remaining_steps;


    //Se já demos k passos
    if (steps == cabecalho->k) {


        if (task_type == 1) {
            //TASK 1
            //Se conseguimos chegar à energia necessária às condições
            if (energy >= cabecalho->Task_Energy) {
                fprintf(fp_output, "%d %d %d %d %d %d %d %d\n", cabecalho->L, cabecalho->C, cabecalho->Task_Energy, cabecalho->l1, cabecalho->c1, cabecalho->k, cabecalho->Energy_init, energy);

                //Faz print do caminho e da energia da células
                for (int i = 0; i < cabecalho->k; i++) {
                    int px = path[i][0];
                    int py = path[i][1];
                    fprintf(fp_output, "%d %d %d\n", px + offtset_l, py + offtset_c, master_pointer[px][py]);
                }
                return true;
            }


        //TASK 2
        } else if (task_type == 2) {
            //Se a nossa energia final obtida for superior à nossa antiga melhor, guarda o caminho no best_path
            if (energy > *max_energy) {
                *max_energy = energy;
                for (int i = 0; i < cabecalho->k; i++) {
                    best_path[i][0] = path[i][0];
                    best_path[i][1] = path[i][1];
                }
            }
        }
        return false;
    }


    //Atualização dos passos que faltam
    remaining_steps = cabecalho->k - steps;


    //Verifica o bound se vale apena continuar a busca
    if (worth_continue(cabecalho, max_LCK,  energy, remaining_steps, max_energy) == false) {
        return false;
    }


    //Procura pelas 4 coordenadas
    for (int i = 0; i < 4; i++) {
        int new_x = x + dir[i][0];
        int new_y = y + dir[i][1];


        //Verifica a se  posição é válida
        if (is_valid(new_x, new_y, matrix_limits, visited)) {

            //Atualização da energia atual
            int new_energy = energy + master_pointer[new_x][new_y];

            //Se a nossa nova energia atualizada for positiva, continua a procura
            if (new_energy > 0) {
                visited[new_x][new_y] = true;
                path[steps][0] = new_x;
                path[steps][1] = new_y;
                steps++;

                if (DFS(new_x, new_y, steps, new_energy, master_pointer, visited, path, best_path, max_LCK, cabecalho, fp_output, max_energy, task_type, matrix_limits)) {
                    return true;
                }

                //Se não for, volta a trás
                steps--;
                visited[new_x][new_y] = false;
            }
        }
    }
    return false;
}



/******************************************************************************
 * choose_task()
 *
 * Arguments: master_pointer - Duplo apontador para o mapa
 *            cabecalho - Cabeçalho para a realização dos problemas
 *            fp_output - Apontador para o ficheiro de output
 *            matrix_limits - Limites da função para o respetivo problema
 *            max_LCK - estrutura de dados que contém os máximos
 *
 * Returns: void
 * 
 * Description: Determina se vale a pena prosseguir com a procura DFS com branch and bound
 *          atravéz dos dados obtidos pela tarefa 1 e 2. Se sim determina qual a tarefa que
 *          deve ser executada. Aplicando a partir daí a recursão do DFS.
 *****************************************************************************/
void choose_Task(int **master_pointer, label *cabecalho, FILE *fp_output, limits* matrix_limits, max_info max_LCK) {

    //Procura dentro do losangualo o seu maior valor e o somatório dos valores positivos
    old_Tasks(master_pointer, cabecalho, &max_LCK,matrix_limits);


    int start_l = max_LCK.new_l1;
    int start_c = max_LCK.new_c1;
    int steps = 0;

    bool** visited = init_visited(max_LCK); //Alocação da tabela de boleanos que indica se uma dada posição ja foi visitada
    int** path = init_path(cabecalho);      //Alocação da tabela (tamanho k) onde vamos guardar o caminho

    //Marca a posição inicial como visited e poẽ-na no começo do caminho
    visited[start_l][start_c] = true;
    path[0][0] = start_l;
    path[0][1] = start_c;



    //TASK 2
    if (cabecalho->Task_Energy == -2) {

        int max_energy = -1;
        int** best_path = init_best_path(cabecalho);     //Alocação da tabela (tamanho k) onde vamos guardar o melhor caminho futuramente
        int offtset_l= cabecalho->l1 -max_LCK.new_l1;   //Conversões
        int offtset_c= cabecalho->c1 -max_LCK.new_c1;


        DFS(start_l, start_c, steps, cabecalho->Energy_init, master_pointer, visited, path, best_path, max_LCK, cabecalho, fp_output, &max_energy, 2, matrix_limits);

        //Se a energia maxima não for -1,tem solução
        if (max_energy != -1) {

            //Faz print do cabecalho, dos caminhos e das energias em células
            fprintf(fp_output, "%d %d %d %d %d %d %d %d\n", cabecalho->L, cabecalho->C, cabecalho->Task_Energy, cabecalho->l1, cabecalho->c1, cabecalho->k, cabecalho->Energy_init, max_energy);

            for (int i = 0; i < cabecalho->k; i++) {
                int bp_x = best_path[i][0];
                int bp_y = best_path[i][1];
                fprintf(fp_output, "%d %d %d\n", bp_x + offtset_l, bp_y + offtset_c, master_pointer[bp_x][bp_y]);
            }


        //Se não, o problema nao é valido
        } else {
            fprintf(fp_output, "%d %d %d %d %d %d %d %d\n", cabecalho->L, cabecalho->C, cabecalho->Task_Energy, cabecalho->l1, cabecalho->c1, cabecalho->k, cabecalho->Energy_init, -1);
        }


        //Free do melhor caminho
        free_best_path(cabecalho, best_path);


        //TASK 1
    } else {

        //Invoca o DFS, se ele retornar false, não tem solução
        if (!DFS(start_l, start_c, steps, cabecalho->Energy_init, master_pointer, visited, path, NULL, max_LCK, cabecalho, fp_output, NULL, 1, matrix_limits)) {
            fprintf(fp_output, "%d %d %d %d %d %d %d %d\n",
                    cabecalho->L, cabecalho->C, cabecalho->Task_Energy,
                    cabecalho->l1, cabecalho->c1, cabecalho->k,
                    cabecalho->Energy_init, -1);
        }
    }



    //Free das tabelas previamente alocadas
    free_visited(max_LCK, visited);
    free_path(cabecalho, path);
}
