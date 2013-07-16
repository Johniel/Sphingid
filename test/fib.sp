int fib(int n)
{
  if (n < 2) return 1;
  else return fib(n - 1) + fib(n - 2);
}

int main(int argc, string args)
{
  int i;
  for (i = 0; i <= 5; i = i + 1) {
    print(fib(i));
  }
  return 0;
}
