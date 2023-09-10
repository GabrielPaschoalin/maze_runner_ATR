#include <stdio.h>
#include <stack>
#include <stdlib.h>
#include <unistd.h> // for usleep
#include <thread>
#include <iostream>

using namespace std;

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
stack<pos_t> valid_positions;

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

bool exitFound = false;

void print_maze_with_delay()
{
	while (!exitFound)
	{
		system("clear");
		printf("%i %i\n", num_rows, num_cols);
		print_maze();
		this_thread::sleep_for(chrono::milliseconds(100)); // Sleep for 500ms
	}
}

// Função responsável pela navegação.
// Recebe como entrada a posição initial e retorna um booleando indicando se a saída foi encontrada

bool walk(pos_t pos)
{
	// Repita até que a saída seja encontrada ou não existam mais posições não exploradas
	while (!valid_positions.empty())
	{
		this_thread::sleep_for(chrono::milliseconds(100));
		// Marcar a posição atual com o símbolo 'o'
		maze[pos.i][pos.j] = 'o';

		if (pos.i > 0)
		{
			if (maze[pos.i - 1][pos.j] == 's')
			{
				exitFound = true;
			}
			else if (maze[pos.i - 1][pos.j] == 'x')
			{
				pos_t next = {pos.i - 1, pos.j};
				valid_positions.push(next);
			}
		}

		if (pos.i < num_rows - 1)
		{
			if (maze[pos.i + 1][pos.j] == 's')
			{
				exitFound = true;
			}
			else if (maze[pos.i + 1][pos.j] == 'x')
			{
				pos_t next = {pos.i + 1, pos.j};
				valid_positions.push(next);
			}
		}

		if (pos.j > 0)
		{
			if (maze[pos.i][pos.j - 1] == 's')
			{
				exitFound = true;
			}
			else if (maze[pos.i][pos.j - 1] == 'x')
			{
				pos_t next = {pos.i, pos.j - 1};
				valid_positions.push(next);
			}
		}

		if (pos.j < num_cols - 1)
		{
			if (maze[pos.i][pos.j + 1] == 's')
			{
				exitFound = true;
			}
			else if (maze[pos.i][pos.j + 1] == 'x')
			{
				pos_t next = {pos.i, pos.j + 1};
				valid_positions.push(next);
			}
		}

		// Marcar a posição atual com o símbolo '.'
		maze[pos.i][pos.j] = '.';

		if (exitFound == true)
		{
			return true;
		}

		if (valid_positions.size() == 1)
		{
			pos = valid_positions.top();
			valid_positions.pop();
		}
		else if (valid_positions.size() == 2)
		{
			//Criar outra função
			pos_t pos1 = valid_positions.top();
			valid_positions.pop();
			pos_t pos2 = valid_positions.top();
			valid_positions.pop();

			thread t1(walk, pos1);
			thread t2(walk, pos2);

			t1.detach();
			t2.detach();
		}
		else
		{
			break;
		}
	}

	return false;
}

int main(int argc, char *argv[])
{
	// carregar o labirinto com o nome do arquivo recebido como argumento
	pos_t initial_pos = load_maze(argv[1]);

	valid_positions.push(initial_pos);

	thread print(print_maze_with_delay);

	// chamar a função de navegação
	thread exit(walk, initial_pos);

	print.join();
	exit.join();
	// Tratar o retorno (imprimir mensagem)

	if (exitFound)
	{
		printf("Exit found!\n");
	}
	else
	{
		printf("Exit not found.\n");
	}

	for (int i = 0; i < num_rows; ++i)
	{
		free(maze[i]);
	}

	free(maze);

	return 0;
}
