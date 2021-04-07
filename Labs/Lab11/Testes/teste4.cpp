int main()
{
    int i; int j; float v; float x; float a[100]; bool flag;

    flag = true;
    while (flag)
    {
        do
        {
            i = i+1;
        } 
        while (a[i] < v);
        
        do
        {
            j = j-1;
        } 
        while (a[j] > v);
        
        if (i >= j)
            flag = false;
    
        x = a[i]; a[i] = a[j]; a[j] = x;
    }
}