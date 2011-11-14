import java.net.*;
import java.io.*;
import java.util.*;






public class SocketTest
{   public static void main(String args[])
    {   Socket socket= null;
        try
	{   socket= new Socket("time-a.timefreq.bldrdoc.gov", 13);
	    InputStream inputStream= socket.getInputStream();
	    Scanner scanner= new Scanner(inputStream);
	    while(scanner.hasNextLine())
	    {   System.out.println(scanner.nextLine());
	    }
        }
	catch (IOException e)
	{   System.out.println(e);   
	}
	finally
	{   if(socket != null)
	    {   try
	        {   socket.close();
		}
		catch (IOException ioe)
		{   System.out.println(ioe);
		}
            }
        }
    }
}
