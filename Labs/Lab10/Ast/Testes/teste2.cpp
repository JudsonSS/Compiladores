// meu programa de teste
int main()
{
    int i; int j; int k; float vet[10]; bool flag;
    i = 0;
    j = 9;
    k = 1;
    flag = false;

    do
    {
        while (i < k)
            i = i + 1;

        // testa operadores logicos
        while (j > k && j <= 5 || true && !flag)
            j = j - 1;

        if (vet[i] < vet[j])
        {
            vet[i] = vet[i - 1] * 2.0;
            vet[j] = vet[j + 1] / 2.0;
        }
    } 
    while (i < j);
}