#include <stdio.h>
#include <stack>
#include <stdlib.h>
#include <unistd.h> // for usleep

// Matriz de char representando o labirinto
char **maze; // Voce também pode representar o labirinto como um vetor de vetores de char (vector<vector<char>>)

// Numero de linhas e colunas do labirinto
int num_rows;
int num_cols;

// Representação de uma posição
struct pos_t
{
	int i;
	int j;
};

// Estrutura de dados contendo as próximas
// posicões a serem exploradas no labirinto
std::stack<pos_t> valid_positions;

// Função que le o labirinto de um arquivo texto, carrega em
// memória e retorna a posição inicial
pos_t load_maze(const char *file_name)
{
	pos_t initial_pos;
	// Abre o arquivo para leitura (fopen)
	FILE *arq = fopen(file_name, "r");

	// Le o numero de linhas e colunas (fscanf)
	// e salva em num_rows e num_cols
	fscanf(arq, "%i %i", &num_rows, &num_cols);

	// Aloca a matriz maze (malloc)
	maze = (char **)malloc(num_rows * sizeof(char *));
	for (int i = 0; i < num_rows; ++i)
		maze[i] = (char *)malloc((num_cols) * sizeof(char));

	// Aloca cada linha da matriz
	for (int i = 0; i < num_rows; ++i)
	{
		for (int j = 0; j < num_cols; ++j)
		{
			// Le o valor da linha i+1,j do arquivo e salva na posição maze[i][j]
			// Se o valor for 'e' salvar o valor em initial_pos
			fscanf(arq, " %c", &maze[i][j]);
			if (maze[i][j] == 'e')
			{
				initial_pos.i = i;
				initial_pos.j = j;
			}
		}
	}
	fclose(arq);

	return initial_pos;
}

void print_maze()
{
	for (int i = 0; i < num_rows; ++i)
	{
		for (int j = 0; j < num_cols; ++j)
		{
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
}
int count = 1;

void print_maze_with_delay()
{
    usleep(10000); // Delay for 10 milliseconds (adjust as needed)
    system("clear");
    printf("%i %i %i\n", num_rows, num_cols, count);
    print_maze();
    count++;
}


// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada

bool walk(pos_t pos)
{
	// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
	while (!valid_positions.empty())
	{

		// Marcar a posição atual com o símbolo '.'
		maze[pos.i][pos.j] = '.';


		// Imprime o labirinto
		print_maze_with_delay();

		pos_t next_position[4] = {
			{pos.i, pos.j + 1},
			{pos.i, pos.j - 1},
			{pos.i + 1, pos.j},
			{pos.i - 1, pos.j},

		};

		if (pos.i >= 0 && pos.i < num_rows)
		{
			if (maze[pos.i + 1][pos.j] == 's' || maze[pos.i - 1][pos.j] == 's')
			{
				return true;
			}
			else if (maze[pos.i + 1][pos.j] == 'x')
			{
				valid_positions.push(next_position[2]);
			}
			else if (maze[pos.i - 1][pos.j] == 'x')
			{
				valid_positions.push(next_position[3]);
			}
		}
		if (pos.j >= 0 && pos.j < num_cols)
		{
			if (maze[pos.i][pos.j + 1] == 's' || maze[pos.i][pos.j - 1] == 's')
			{
				return true;
			}
			else if (maze[pos.i][pos.j + 1] == 'x')
			{
				valid_positions.push(next_position[0]);
				printf("teste");
			}
			else if (maze[pos.i][pos.j - 1] == 'x')
			{
				valid_positions.push(next_position[1]);
			}
		}

		if (!valid_positions.empty())
		{
			pos_t next_position = valid_positions.top();
			valid_positions.pop();
			
			pos = next_position;
		}
	}

	return false;
}

int main(int argc, char *argv[])
{
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze(argv[1]);

	valid_positions.push(initial_pos);
	// chamar a função de navegação
	bool exit_found = walk(initial_pos);

	// Tratar o retorno (imprimir mensagem)

	if (exit_found)
	{
		printf("Exit found!\n");
	}
	else
	{
		printf("Exit not found.\n");
	}

	return 0;
}
