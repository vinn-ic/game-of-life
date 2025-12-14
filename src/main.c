#include "cairo-deprecated.h"
#include "cairo.h"
#include "glib-object.h"
#include "glib.h"
#include "glibconfig.h"
#include <gtk/gtk.h>
#include <stdio.h>
// #include <stdio.h>
#define WINDOW_X 1162
#define WINDOW_Y 650
// 125
// 105
#define BLOCK_X WINDOW_X / 6
#define BLOCK_Y WINDOW_Y / 6

int matriz[BLOCK_X][BLOCK_Y];
int matriz2[BLOCK_X][BLOCK_Y];
// a celula continua viva com 2 ou 3 outras celular ao redor (==2 || ==3)
// a celular morre com menos menos 2 ou mais de 3 (<2 || >3)
// a celula morta ganha vida com exatamente 3 outras celulas vivas ao redor (==
// 3)

gboolean render(GtkWidget *widget, cairo_t *cr, gpointer data) {
  int size = 5;
  // blocos
  // x = 35
  // y = 57

  int counterX = 1;
  int counterY = 1;

  for (int x = 10; x < WINDOW_X; x += (size + 1)) {
    counterY = 1;

    for (int y = 10; y < WINDOW_Y; y += (size + 1)) {
      if (matriz[counterX][counterY] == 1) {
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_rectangle(cr, x, y, size, size);
        cairo_fill(cr);

      } else {
        cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
        cairo_rectangle(cr, x, y, size, size);
        cairo_fill(cr);
      }
      counterY++;
    }
    counterX++;
  }

  return FALSE;
}

int updateGen() {
  for (int x = 1; x < BLOCK_X - 1; x++) {
    for (int y = 1; y < BLOCK_Y - 1; y++) {
      matriz2[x][y] = matriz[x][y];
    }
  }

  int counter = 0;

  for (int x = 1; x < BLOCK_X; x++) {
    for (int y = 1; y < BLOCK_Y; y++) {
      counter = 0;

      if (matriz[x + 1][y + 1]) {
        counter += 1;
      }
      if (matriz[x - 1][y - 1]) {
        counter += 1;
      }
      if (matriz[x + 1][y - 1]) {
        counter += 1;
      }
      if (matriz[x - 1][y + 1]) {
        counter += 1;
      }
      if (matriz[x][y + 1]) {
        counter += 1;
      }
      if (matriz[x][y - 1]) {
        counter += 1;
      }
      if (matriz[x - 1][y]) {
        counter += 1;
      }
      if (matriz[x + 1][y]) {
        counter += 1;
      }
      if (matriz[x][y]) {
        if (counter > 3 || counter < 2) {
          matriz2[x][y] = 0;
        }
      } else {
        if (counter == 3) {
          matriz2[x][y] = 1;
        }
      }
    }
  }
  return 0;
}

gboolean updateGame(gpointer area) {
  printf("começando o updateGen ......\n\n");
  updateGen();

  printf("updateGen ja rodou, començando o render....\n\n");
  gtk_widget_queue_draw(area);

  for (int x = 0; x < BLOCK_X; x++) {
    for (int y = 0; y < BLOCK_Y; y++) {
      matriz[x][y] = matriz2[x][y];
    }
  }

  return TRUE;
}

int main(int argc, char *argv[]) {
  setbuf(stdout, NULL);
  printf("iniciando main...\n\n");

  for (int x = 0; x < BLOCK_X; x++) {
    for (int y = 0; y < BLOCK_Y; y++) {
      matriz[x][y] = 1;
      matriz2[x][y] = 1;
    }
  }
  matriz2[10][1] = 1;
  matriz2[10][2] = 1;
  matriz2[11][1] = 1;
  matriz2[11][2] = 1;

  gtk_init(&argc, &argv);

  GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(win), "game of live");
  gtk_window_set_default_size(GTK_WINDOW(win), WINDOW_X, WINDOW_Y);

  GtkWidget *boxMain = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
  gtk_container_add(GTK_CONTAINER(win), boxMain);
  g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  GtkWidget *area = gtk_drawing_area_new();
  gtk_widget_set_size_request(area, 250, 150);
  gtk_box_pack_start(GTK_BOX(boxMain), area, TRUE, TRUE, 0);

  printf("iniciando o primeiro render.....\n\n");
  g_signal_connect(area, "draw", G_CALLBACK(render), NULL);

  printf("iniciando o timeout......\n\n");
  g_timeout_add(100, updateGame, area);

  gtk_widget_show_all(win);
  gtk_main();

  return 0;
}
