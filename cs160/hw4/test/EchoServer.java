import java.io.*;
import java.net.*;
import java.util.*;


public class EchoServer
{

    static private final int    DEFAULT_PORT   = 8189;
    static private final String SIGN_OFF_TOKEN = "BYE";


    public static void main(String args[])
    {   ServerSocket serverSocket= null;
        Socket clientSocket= null;

	try
	{   System.out.println("Creating ServerSocket on " + DEFAULT_PORT);
	    serverSocket= new ServerSocket(DEFAULT_PORT);
	    Scanner socketScanner= null;
	    while(true)
	    {   System.out.println("Looking for a client...");
	        clientSocket= serverSocket.accept();  //blocks until client connects
		System.out.println("Client connected.");


		socketScanner= new Scanner(clientSocket.getInputStream());
		OutputStream outputStream= clientSocket.getOutputStream();
		PrintWriter outputToClient= new PrintWriter(outputStream, true);

		outputToClient.println("Hello! Enter BYE to exit.");
		boolean done= false;
		while(!done)
		{   String line= socketScanner.nextLine();
		    if(line == null)
		    {   done= true;
		    }
		    else
		    {   System.out.println(">> " + line);  // prints on server side
		        if(line.trim().toUpperCase().startsWith(SIGN_OFF_TOKEN))
			{   outputToClient.println("Goodbye!"); //client side output
			    System.out.println("Client signed off");
			    done= true;
			}
			else
			{   // echo input to client
			    outputToClient.println(line);
			}
		    }
		}
            }
	}
	catch (Exception e)
	{   System.err.println(e);
	}
	catch (Throwable t)
	{   System.err.println("Caught throwable t: " + t);
	}
	finally
	{   if(serverSocket != null)
	    {   System.out.println("Finally clause");
	        try
		{   System.out.println("Cleaning up, closing sockets");
		    serverSocket.close();
		    if(clientSocket != null)
		    {   clientSocket.close();
		    }
                }
		catch (IOException eio)
		{ //do nothing
		}
	    }
        }
    }
}
