unsigned long ic;

void empty_func() {}

int main_func(int a, int b, int c) {
  ic = 0;

  // Function body
  int d = 0;
  for (int i = 0; i < a; i++) {
    if (d < b)
      d += c;
  }
  return d;
}

int nested_func(int n) {
  int res = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      res += i;
      res += j;
    }
  }
  return res;
}