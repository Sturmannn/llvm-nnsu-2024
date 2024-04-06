void func()
{
    int a = 0;
    a += 1;
}

int foo(int num)
{
    func();
    return num;
}

// void empty_func()
// {
//     int a = 0;
//     for (int i = 0; i < 5; i++)
//     {
//         a += i;
//     }
// }

// void main_foo(int N) {
//     for (int i = 0; i < N; i++) {
//         empty_func();
//     }
// }
