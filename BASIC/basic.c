#include "basic.h"

static void swap (SortIns * a, SortIns * b);
static void qsort_r (SortIns arr[], int l, int r, QsortType type);

/* float32_t abs function. */
float32_t
f32abs (float32_t x)
{
	if (x < 0)
		return -x;

	return x;
}

/* float32_t power function. */
float32_t
f32pow (float32_t x, uint8_t n)
{
	if (n == 0)
		return 1.0;
	else if (n == 1)
		return x;
	
	for (uint8_t i = 1; i < n; ++i)
		x *= x;

	return x;
}

/* Integral method for arctan function. */
float32_t
arctan (float32_t x)
{
	float32_t tmp;
	float32_t sum = 0.0;
	float32_t stepSize = f32abs (x) / ATAN_INT_STEP;
	
	for (uint32_t i = 0; i < ATAN_INT_STEP; ++i)
	{
		tmp = stepSize*i + stepSize/2.0;
		sum += stepSize / (1.0 + tmp*tmp);
	}
	
	if (x < 0)
		sum *= -1;
	
	return sum;
}

float32_t
mean (float32_t arrIn[], int N)
{
  float32_t sum = 0.0;

  for (int i = 0; i < N; ++i)
    sum += arrIn[i];

  return sum / N;
}

int
is_equal_f (float32_t a, float32_t b, float32_t tolerance)
{
  if (f32abs (a-b) <= tolerance)
    return 1;
  
  return 0;
}

void
init_qsort (float32_t arrIn[], SortIns insts[], int N)
{
  for (int i = 0; i < N; ++i)
  {
    insts[i].val = arrIn[i];
    insts[i].idx = i;
  }
}

void
qsort (SortIns insts[], int N, QsortType type)
{
  qsort_r (insts, 0, N-1, type);
}

void
print_arr_f (float32_t arrIn[], int N)
{
  for (uint32_t i = 0; i < N; ++i)
    printf ("%f\r\n", arrIn[i]);
}

void
print_arr_u32 (uint32_t arrIn[], int N)
{
  for (uint32_t i = 0; i < N; ++i)
    printf ("%d\r\n", arrIn[i]);
}

static void
swap (SortIns * a, SortIns * b)
{
  SortIns tmp = *a;
  *a = *b;
  *b = tmp;
}

static void
qsort_r (SortIns arr[], int l, int r, QsortType type)
{
  if (l >= r) return;

  float32_t pivot = arr[(l + r) / 2].val;
  int i = l;
  int j = r;

  while (i <= j)
  {
    if (type == ASCENDING)
    {
      while (arr[i].val < pivot) ++i;
      while (arr[j].val > pivot) --j;
    } else
    {
      while (arr[i].val > pivot) ++i;
      while (arr[j].val < pivot) --j;
    }

    if (i <= j)
    {
      swap(&arr[i], &arr[j]);
      ++i;
      --j;
    }
  }

  if (l < j) qsort_r (arr, l, j, type);
  if (i < r) qsort_r (arr, i, r, type);
}
