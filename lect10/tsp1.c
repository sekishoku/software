#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <unistd.h>

// const による定数定義
const int width = 70;
const int height = 40;
const int max_cities = 1000;


// 町の構造体（今回は2次元座標）を定義
typedef struct
{
  int x;
  int y;
} City;

// 整数最大値をとる関数
int max(const int a, const int b)
{
  return (a > b) ? a : b;
}

// 以下は実装が長くなるのでプロトタイプ宣言
// draw_line: 町の間を線で結ぶ
// draw_route: routeでの巡回順を元に移動経路を線で結ぶ
// plot_cities: 描画する
// distance: 2地点間の距離を計算
// solove(): 巡回問題をといて距離を返す/ 引数route に巡回順を格納

void draw_line(char **map, City a, City b);
void draw_route(char **map, City *city, int n, const int *route);
void plot_cities(FILE* fp, char **map, City *city, int n, const int *route);
double distance(City a, City b);
double route_distance(City *city, int n, int *route);
void switch_route(int *route, int n, int m);
double solve(City *city, int n, int *route);


int main(int argc, char**argv)
{
  FILE *fp = stdout; // とりあえず描画先は標準出力としておく
  if (argc != 2){
    fprintf(stderr, "Usage: %s <the number of cities>\n", argv[0]);
    exit(1);
  }
  const int n = atoi(argv[1]);
  assert( n > 1 && n <= max_cities);

  City *city = (City*) malloc(n * sizeof(City));
  char **map = (char**) malloc(width * sizeof(char*));
  char *tmp = (char*)malloc(width*height*sizeof(char));
  for (int i = 0 ; i < width ; i++)
    map[i] = tmp + i * height;
  
  srand((unsigned int)time(NULL));

  /* 町の座標をランダムに割り当て */
  for (int i = 0 ; i < n ; i++){
    city[i].x = rand() % (width - 5);
    city[i].y = rand() % height;
  }
  // 町の初期配置を表示
  plot_cities(fp, map, city, n, NULL);
  sleep(1);

  // 訪れる順序を記録する配列を設定
  int *route = (int*)calloc(n, sizeof(int));

  const double d = solve(city, n, route);
  printf("total distance = %f\n", d);
  plot_cities(fp, map, city, n, route);

  // 動的確保した環境ではfreeをする
  free(route);
  free(map[0]);
  free(map);
  free(city);
  
  return 0;
}

// 繋がっている都市間に線を引く
void draw_line(char **map, City a, City b)
{
  const int n = max(abs(a.x - b.x), abs(a.y - b.y));
  for (int i = 1 ; i <= n ; i++){
    const int x = a.x + i * (b.x - a.x) / n;
    const int y = a.y + i * (b.y - a.y) / n;
    if (map[x][y] == ' ') map[x][y] = '*';
  }
}

void draw_route(char **map, City *city, int n, const int *route)
{
  if (route == NULL) return;

  for (int i = 0; i < n; i++) {
    const int c0 = route[i];
    const int c1 = route[(i+1)%n];// n は 0に戻る必要あり
    draw_line(map, city[c0], city[c1]);
  }
}

void plot_cities(FILE *fp, char **map, City *city, int n, const int *route)
{
  fprintf(fp, "----------\n");

  memset(map[0], ' ', width * height); // グローバルで定義されたconst を使用

  // 町のみ番号付きでプロットする
  for (int i = 0; i < n; i++) {
    char buf[100];
    sprintf(buf, "C_%d", i);
    for (int j = 0; j < strlen(buf); j++) {
      const int x = city[i].x + j;
      const int y = city[i].y;
      map[x][y] = buf[j];
    }
  }

  draw_route(map, city, n, route);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      const char c = map[x][y];
      fputc(c, fp);
    }
    fputc('\n', fp);
  }
  fflush(fp);
}

double distance(City a, City b)
{
  const double dx = a.x - b.x;
  const double dy = a.y - b.y;
  return sqrt(dx * dx + dy * dy);
}

// 現在の経路の総距離を計算
double route_distance(City *city, int n, int *route)
{
  double sum_d = 0;
  for (int i = 0 ; i < n ; i++){
    const int c0 = route[i];
    const int c1 = route[(i+1)%n];
    sum_d += distance(city[c0],city[c1]);
  }

  return sum_d;
}

// 経路の交換
void switch_route(int *route, int i, int j)
{
  int tem = route[i];
  route[i] = route[j];
  route[j] = tem;
}

double solve(City *city, int n, int *route)
{
  // 初期化
  int change = 1;

  for (int i = 0 ; i < n ; i++)
    route[i] = i;
  
  // 現在の経路長を記録
  double distance_cur = route_distance(city,n,route);

  // 改良がなくなるまで山登りする
  while(change == 1){
      change = 0;       // ループ内で改良がなければ0、あれば1
      //printf("tem distance = %f\n",distance_cur);
      for(int i = 0; i < n; i++){
          for(int j = i+1; j < n; j++){
              switch_route(route,i,j);
              double distance_tem = route_distance(city,n,route);
              if(distance_tem >= distance_cur){   // 改良されていなかったら元に戻す
                  switch_route(route,i,j);
              }else{
                  change = 1;
                  distance_cur = distance_tem;
              }
          }
      }
  }
  
  return distance_cur;
}