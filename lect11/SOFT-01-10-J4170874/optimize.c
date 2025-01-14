#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "optimize.h"

double calc_norm(const int dim, double v[])
{
  double tmp = 0;
  for (int i = 0; i < dim; i++) {
    tmp += v[i] * v[i];
  }
  tmp = sqrt(tmp);
  return tmp;
}

int optimize(const double alpha, const int dim, double x[], 
             void (*calc_grad)(const double [], double []),
             double (*f_value)(const double []))
{
  // 勾配ベクトルを記録する領域を確保
  double *g = malloc(dim * sizeof(double));

  int iter = 0;
  while (++iter < 10000) {

    // 引数で渡された関数を使って勾配ベクトルを計算
    (*calc_grad)(x, g);

    // 勾配ベクトルのノルムを評価
    const double norm = calc_norm(dim, g);
    printf("%3d norm = %7.4f", iter, norm);
    for (int i = 0; i < dim; i++) {
      printf(", x[%d] = %7.4f", i, x[i]);
    }
    printf(", f(x) = %7.4f", f_value(x));
    printf("\n");

    if (norm < 0.0001) break;

    // 最急降下法による更新
    for (int i = 0; i < dim; i++) {
      x[i] -= alpha * g[i];
    }
  }

  double temp_fj = x[0] * 3.776 + x[1];
  printf("temperture of Mt.Fuji in July would be %7.4f degrees celcius\n", temp_fj); 

  free(g);

  return iter;
}

