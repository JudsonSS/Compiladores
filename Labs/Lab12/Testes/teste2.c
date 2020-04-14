// exemplo de programa em linguagem simplificada
main 
{
    int i; int j; float v; float a[10];
    i = 0;
    j = 9;
    v = 5;

    do 
    { 
        while(a[i] < v)
            i = i + 1;

	    while(a[j] > v)
            j = j - 1;
         
	    if (i<j)
        {
            a[i] = a[i-1] + 1;
            a[j] = a[j+1] - 1;
        }
    }
    while(i < j);
}
