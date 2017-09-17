/* C library */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <math.h>
/* OpenGL */
#include <dmatrix.h>

#define GRID_W	256
#define GRID_H	256

void key_callback(GLFWwindow *win, int key, int scan, int action, int mods);

void htorgb(uint8_t *rgb, int h);
int  subset_sum(uint8_t *a, int siz, int n);
int  compare(const void *a, const void *b);

int
main(int argc, char **argv)
{
	struct dot_matrix dm;
	uint8_t scr_buf[GRID_H][GRID_W][3], target, reward, penalty, arr[8];
	int cell_grid[GRID_H][GRID_W], tmp_cell_grid[GRID_H][GRID_W], i, j;

	srand(time(NULL));
	memset(scr_buf[0][0], 0, sizeof scr_buf);

	dm.scr_buf = scr_buf[0][0];
	dm.scr_buf_w = GRID_W;
	dm.scr_buf_h = GRID_H;

	target = 9;
	reward = 5;
	penalty = 6;

	create_matrix(&dm, 960, 960);
	glfwSetKeyCallback(dm.win, key_callback);
	
	memset(cell_grid, 0, sizeof cell_grid);
	for (i = 0; i != GRID_H; ++i) {
		for (j = 0; j != GRID_W; ++j) {
			cell_grid[i][j] = rand() % 60;
		}
	}

	/*
	cell_grid[GRID_H / 2][GRID_W / 2] = 2;
	cell_grid[GRID_H / 2][GRID_W / 2 + 1] = 2;
	cell_grid[GRID_H / 2 + 1][GRID_W / 2] = 2;
	cell_grid[GRID_H / 2 + 1][GRID_W / 2 + 1] = 2;

	cell_grid[GRID_H / 2][GRID_W / 2] = 3;
	cell_grid[GRID_H / 2 + 1][GRID_W / 2 - 1] = 1;
	cell_grid[GRID_H / 2 + 1][GRID_W / 2] = 4;
	cell_grid[GRID_H / 2 + 1][GRID_W / 2 + 1] = 1;
	*/

	while (!glfwWindowShouldClose(dm.win)) {
		glfwPollEvents();

		memcpy(tmp_cell_grid, cell_grid, sizeof cell_grid);
		for (i = 0; i != GRID_H; ++i) {
			for (j = 0; j != GRID_W; ++j) {
				arr[0] = tmp_cell_grid[(i - 1 + GRID_H) % GRID_H][(j - 1 + GRID_W) % GRID_W];
				arr[1] = tmp_cell_grid[(i - 1 + GRID_H) % GRID_H][(j + 0 + GRID_W) % GRID_W];
				arr[2] = tmp_cell_grid[(i - 1 + GRID_H) % GRID_H][(j + 1 + GRID_W) % GRID_W];

				arr[3] = tmp_cell_grid[(i + 0 + GRID_H) % GRID_H][(j - 1 + GRID_W) % GRID_W];
				arr[4] = tmp_cell_grid[(i + 0 + GRID_H) % GRID_H][(j + 1 + GRID_W) % GRID_W];

				arr[5] = tmp_cell_grid[(i + 1 + GRID_H) % GRID_H][(j - 1 + GRID_W) % GRID_W];
				arr[6] = tmp_cell_grid[(i + 1 + GRID_H) % GRID_H][(j + 0 + GRID_W) % GRID_W];
				arr[7] = tmp_cell_grid[(i + 1 + GRID_H) % GRID_H][(j + 1 + GRID_W) % GRID_W];

				qsort(arr, 8, sizeof (uint8_t), compare);
				if (subset_sum(arr, 8, cell_grid[i][j] + target)) {
					if ((cell_grid[i][j] + reward) > 255) {
						cell_grid[i][j] = 255;

					} else {
						cell_grid[i][j] += reward;
					}

				} else {
					if ((cell_grid[i][j] - penalty) < 0) {
						cell_grid[i][j] = 0;

					} else {
						cell_grid[i][j] -= penalty;
					}
				}

				htorgb(scr_buf[i][j], cell_grid[i][j]);
			}
		}

		update_matrix(&dm);
		render_matrix(&dm);
		glfwSwapBuffers(dm.win);
	}

	free_matrix(&dm);
	return 0;
}

void
key_callback(GLFWwindow *win, int key, int scan, int action, int mods)
{
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_ESCAPE:
		case GLFW_KEY_Q:
			glfwSetWindowShouldClose(win, GLFW_TRUE);
			break;
		}
	}
}

void
htorgb(uint8_t *rgb, int h)
{
	switch ((h % 360) / 60) {
	case 0:
		rgb[0] = 255;
		rgb[1] = 255 * (1 - fabs(fmod((double)h / 60.0, 2) - 1));
		rgb[2] = 0;
		break;
		
	case 1:
		rgb[0] = 255 * (1 - fabs(fmod((double)h / 60.0, 2) - 1));
		rgb[1] = 255;
		rgb[2] = 0;
		break;
		
	case 2:
		rgb[0] = 0;
		rgb[1] = 255;
		rgb[2] = 255 * (1 - fabs(fmod((double)h / 60.0, 2) - 1));
		break;
		
	case 3:
		rgb[0] = 0;
		rgb[1] = 255 * (1 - fabs(fmod((double)h / 60.0, 2) - 1));
		rgb[2] = 255;
		break;
		
	case 4:
		rgb[0] = 255 * (1 - fabs(fmod((double)h / 60.0, 2) - 1));
		rgb[1] = 0;
		rgb[2] = 255;
		break;
		
	case 5:
		rgb[0] = 255;
		rgb[1] = 0;
		rgb[2] = 255 * (1 - fabs(fmod((double)h / 60.0, 2) - 1));
		break;
		
	}
}

int
subset_sum(uint8_t *a, int siz, int n)
{
	/* Skip numbers larger than 'n' */
	while (--siz >= 0 && a[siz] > n)
		;

	while (siz >= 0) {
		if (a[siz] == n || subset_sum(a, siz, n - a[siz])) {
			return 1;
		}

		--siz;
	}

	return 0;
}

int
compare(const void *a, const void *b)
{
	int ia, ib;

	ia = *(int *) a;
	ib = *(int *) b;

	if (ia < ib) {
		return -1;
	}

	return ia > ib;
}

