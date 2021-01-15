#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define IS_NUMBER(n) ((n >= 0 && n <= 9))
#define SIZE_GRID (9)

/* function that return 0 if the grid is wrong else return 1 */
int is_valid_grid(int **grid)
{
    int answer = 1;
    int x, y;

    for (x = 0; x < SIZE_GRID; x++)
    {
        for (y = 0; y < SIZE_GRID; y++)
        {
            if (!IS_NUMBER(grid[x][y]))
                answer = 0;
        }
    }

    return answer;
}

/* function that print the grid */
void print_grid(int **grid)
{
    int x, y;

    printf("\n  +-----+-----+-----+\n");
    for (x = 0; x < SIZE_GRID; x++)
    {
        printf("  ");
        for (y = 0; y < SIZE_GRID; y++)
            printf("|%d", grid[x][y]);

        printf("|\n");
        if ((x + 1) % 3 == 0)
            printf("  +-----+-----+-----+\n");
    }
}

/* function that save solved grid to a file */
void save_grid_to_file(FILE *fp, int **grid)
{
    int x, y;

    for (x = 0; x < SIZE_GRID; x++)
    {
        for (y = 0; y < SIZE_GRID; y++)
        {
            fprintf(fp, "%d", grid[x][y]);
            if (y == 8)
                fprintf(fp, "\n");
        }
    }
}

/* function that free the grid */
void free_grid(int **grid)
{
    int i;

    for (i = 0; i < SIZE_GRID; i++)
    {
        free(grid[i]);
        grid[i] = NULL;
    }

    free(grid);
    grid = NULL;
}

/* function that copy grid's file content into a pointer of a grid array 2D */
void get_grid(FILE *fp, int **grid)
{
    int x, y;
    char temp;

    for (x = 0; x < SIZE_GRID; x++)
    {
        y = 0;
        while (y < SIZE_GRID)
        {
            temp = fgetc(fp);
            if (temp != '\n' &&
                temp != '\r')
            {
                grid[x][y] = temp - 48;
                y++;
            }
        }
    }

    fseek(fp, 0, SEEK_SET);
}

/* function that compare two grids, return 1 if it's the same else return 0 */
int compare_grid(int **grid, int **grid_cpy)
{
    int answer = 1;
    int x, y;

    for (x = 0; x < SIZE_GRID; x++)
    {
        for (y = 0; y < SIZE_GRID; y++)
        {
            if (grid[y][x] != grid_cpy[y][x])
                answer = 0;
        }
    }

    return answer;
}

/* function that return 1 if the num already exist on the column else return 0 */
int same_column(int x, int y, int num, int **grid)
{
    int answer = 0;
    int i;

    for (i = 0; i < SIZE_GRID && answer != 1; i++)
    {
        if (grid[x][i] == num)
            answer = 1;
    }

    return answer;
}

/* function that return 1 if the num already exist on the row else return 0 */
int same_row(int x, int y, int num, int **grid)
{
    int answer = 0;
    int i;

    for (i = 0; i < SIZE_GRID && answer != 1; i++)
    {
        if (grid[i][y] == num)
            answer = 1;
    }

    return answer;
}

/* function that return 1 if the num already exist on the square else return 0 */
int same_square(int x, int y, int num, int **grid)
{
    int answer = 0;
    int i, j;
    int start_x = x - x % 3;
    int start_y = y - y % 3;

    for (i = 0; i < 3 && answer != 1; i++)
    {
        for (j = 0; j < 3 && answer != 1; j++)
        {
            if (grid[i + start_x][j + start_y] == num)
                answer = 1;
        }
    }

    return answer;
}

/* function that return 1 if the passed number num is placable else return 0 */
int is_placable(int x, int y, int num, int **grid)
{
    if (!same_square(x, y, num, grid) && !same_row(x, y, num, grid) && !same_column(x, y, num, grid))
        return 1;

    return 0;
}

/* function that return 0 if the grid is resolved */
int solve_grid(int x, int y, int **solution_counter_ptr, int **try_counter_ptr, int **grid)
{
    int num;

    /* means that this is the end of backtracking */
    if (x == 8 && y == SIZE_GRID)
    {
        **solution_counter_ptr += 1;

        return 1;
    }

    /* if column axis equals 9 move to next row and start at 0 */
    if (y == SIZE_GRID)
    {
        x++;
        y = 0;
    }

    /* to avoid pre writted numbers */
    if (grid[x][y] != 0)
        return solve_grid(x, y + 1, solution_counter_ptr, try_counter_ptr, grid);

    /* loop to try to place num from 1 to SIZE_GRID */
    for (num = 1; num < SIZE_GRID + 1; num++)
    {
        print_grid(grid);
        fputs("\033[H\033[J", stdout);

        /* check if num is placable */
        if (is_placable(x, y, num, grid))
        {
            /* assign num to current grid position */
            grid[x][y] = num;

            /* check possibility at current column position + 1 */
            if (solve_grid(x, y + 1, solution_counter_ptr, try_counter_ptr, grid) == 1)
                return 1;
        }

        /* remove assigned num and go for next try */
        grid[x][y] = 0;
    }

    **try_counter_ptr += 1;

    return 0;
}

void cpy_grid(int **dest, int **src)
{
    int x, y;

    for (x = 0; x < SIZE_GRID; x++)
    {
        for (y = 0; y < SIZE_GRID; y++)
        {
            dest[x][y] = src[x][y];
        }
    }
}

void print_instruction() { printf("program usage: ./sudoku <grid_file> <result_file>\ngrid_file being the grid file and result_file being the output file.\n"); }

void print_timers(double time_spent, double time_spent_solving, int **try_counter_ptr)
{
    printf("\n  Resolved in %d tries\n", **try_counter_ptr);
    printf("  -------------------------------\n");
    printf("  Solving Wait Time  : %f ms\n", time_spent_solving);
    printf("  Printing Wait Time : %f ms\n", time_spent - time_spent_solving);
    printf("  -------------------------------\n");
    printf("  Elapsed Time       : %f ms\n\n", time_spent);
}

int main(int argc, char **argv)
{
    FILE *fp = NULL;
    FILE *out = NULL;
    /* TIME VARs */
    double time_spent, time_spent_solving, time_spent_printing;
    clock_t before_solve, after_solve;
    time_t begin = time(NULL);
    time_t end;
    /* NUMBERS OF TRIES VARs */
    int *try_counter = NULL;
    /* GRID VARs */
    int **grid = NULL;
    int **grid_cpy = NULL;
    int *solution_counter = NULL;
    int i, x, y;
    int delete;

    setvbuf(stdout, NULL, _IONBF, 0); /* make fprintf printable at any time (bug that make fprintf unprintable) */

    if (argv[1] == NULL || argv[1][0] == '-')
    {
        if (argv[1] == NULL || argv[1][1] == 'h')
            print_instruction();
        else
            fprintf(stderr, "ERROR: unknown option %s\n", argv[1]);

        return 0;
    }

    /* if the program is not called with argument */
    if (argv[2] == NULL)
    {
        fprintf(stderr, "ERROR: too few arguments\n");

        return 0;
    }

    /* try to open argv[1] file */
    fp = fopen(argv[1], "r");

    /* if the program is called with a non valid argument */
    if (fp == NULL)
    {
        fprintf(stderr, "ERROR: can't open ./%s file\n", argv[1]);

        return 0;
    }

    /* malloc grid */
    grid = (int **)malloc(sizeof(int *) * SIZE_GRID);
    for (i = 0; i < SIZE_GRID; i++)
        grid[i] = (int *)malloc(sizeof(int) * SIZE_GRID);

    if (grid == NULL)
    {
        fprintf(stderr, "ERROR: allocation of grid pointer failed\n");

        return 0;
    }

    /* extract the file grid */
    get_grid(fp, grid);

    fclose(fp);

    /* check if the grid is valid */
    if (!is_valid_grid(grid))
    {
        fprintf(stderr, "ERROR: ./%s grid file is not valid\n", argv[1]);
        free_grid(grid);

        return 0;
    }

    /* malloc grid_cpy */
    grid_cpy = (int **)malloc(sizeof(int *) * SIZE_GRID);
    for (i = 0; i < SIZE_GRID; i++)
        grid_cpy[i] = (int *)malloc(sizeof(int) * SIZE_GRID);

    if (grid_cpy == NULL)
    {
        fprintf(stderr, "ERROR: allocation of grid_cpy pointer failed\n");
        free_grid(grid);

        return 0;
    }

    /* copy non resolved grid to grid_cpy */
    cpy_grid(grid_cpy, grid);

    /* malloc the solution_counter */
    solution_counter = malloc(sizeof(int));

    if (solution_counter == NULL)
    {
        fprintf(stderr, "ERROR: allocation of solution_counter pointer failed\n");
        free_grid(grid);
        free_grid(grid_cpy);

        return 0;
    }

    *solution_counter = 0;

    try_counter = malloc(sizeof(int));

    if (try_counter == NULL)
    {
        fprintf(stderr, "ERROR: allocation of try_counter pointer failed\n");
        free_grid(grid);
        free_grid(grid_cpy);

        free(solution_counter);
        solution_counter = NULL;

        return 0;
    }

    *try_counter = 0;

    /* hide cursor */
    printf("\e[?25l");

    /* get solve begin time */
    before_solve = clock();

    /* solve it */
    solve_grid(0, 0, &solution_counter, &try_counter, grid);

    /* get solve end time */
    after_solve = clock();
    time_spent_solving = (double)(after_solve - before_solve) / CLOCKS_PER_SEC;

    /* enable cursor */
    printf("\e[?25h");

    /* check if the grid is solved */
    if (compare_grid(grid, grid_cpy) == 1)
    {
        fprintf(stderr, "ERROR: ./%s grid file is unsolvable\n", argv[1]);
        free_grid(grid);
        free_grid(grid_cpy);

        free(solution_counter);
        solution_counter = NULL;

        free(try_counter);
        try_counter = NULL;

        return 0;
    }

    /* print solved grid */
    print_grid(grid);

    /* try to write on argv[2] file */
    out = fopen(argv[2], "w");

    /* if the program is called with a non valid argument */
    if (out == NULL)
    {
        fprintf(stderr, "ERROR: can't open ./%s file\n", argv[2]);
        free_grid(grid);
        free_grid(grid_cpy);

        free(solution_counter);
        solution_counter = NULL;

        free(try_counter);
        try_counter = NULL;

        return 0;
    }

    /* copy resolved grid to new file */
    save_grid_to_file(out, grid);

    fclose(out);

    free_grid(grid);
    free_grid(grid_cpy);

    free(solution_counter);
    solution_counter = NULL;

    /* get program end time */
    end = time(NULL);
    time_spent = (double)difftime(end, begin) * 1000;

    print_timers(time_spent, time_spent_solving, &try_counter);

    free(try_counter);
    try_counter = NULL;

    return 0;
}
