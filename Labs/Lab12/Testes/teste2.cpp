// exemplo de programa em linguagem simplificada
int main()
{
    int i; int j; int k; int vet[10];
    i = 0;
    j = 9;
    k = 1;

    do 
    { 
        while(i < k)
            i = i + 1;

        // testa operadores logicos
	    while(j > k && j <= 5 || true)
            j = j - 1;
         
	    if (vet[i] < vet[j])
        {
            vet[i] = vet[i-1] * 2;
            vet[j] = vet[j+1] / 2;
        }
    }
    while(i < j);
}
