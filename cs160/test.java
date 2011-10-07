public class test
{   public static void main(String args[])
    {   int dual[][] = new int[5][2];
        int j=1;

        for(int i=0; i<5; i++)
        {   dual[i][0]= j++;
            dual[i][1]= j++;
        }

        for(int i=0; i<5; i++)
        {   System.out.println("dual[" + i + "][0]: " + dual[i][0] );
            System.out.println("dual[" + i + "][1]: " + dual[i][1] );
        }
    }
}
