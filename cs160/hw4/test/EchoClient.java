import java.io.*;
import java.net.*;
import java.util.*;



public class EchoClient
{

    static private final int     DEFAULT_PORT   = 8189;
    static private final String  DEFAULT_SERVER = "localhost";
    static private final String  PROMPT         = "--> ";
    static private final String  SIGNOFF_TOKEN  = "BYE";



    public static void main(String args[])
    {   Socket echoSocket= null;
        Scanner socketScanner= null;
        Scanner keyboardScanner= null;
        PrintWriter out= null;
        String hostname = (args.length == 0) ? DEFAULT_SERVER : args[0];


        try
        {
            System.out.println("Creating Socket on " + DEFAULT_PORT);
	    echoSocket= new Socket(hostname, DEFAULT_PORT);

	    out= new PrintWriter(echoSocket.getOutputStream(), true);
	    socketScanner= new Scanner(echoSocket.getInputStream());
	    keyboardScanner= new Scanner(System.in);
        }
	catch (UnknownHostException e)
	{   System.err.println("Don't know about host " + hostname);
	    return;
	}
	catch (IOException e)
	{   System.err.println("Couldn't get I/O for the connection to " + hostname);
	    return;
        }



        String instructionsFromServer= socketScanner.nextLine();
	System.out.println(instructionsFromServer);
	boolean done= false;
	String userInput= "";
	while(userInput != null  &&  !done)
	{   System.out.print(PROMPT);
	    userInput= keyboardScanner.nextLine(); //reading line of keyboard input
	    if(userInput == null)
	    {   done= true;
	    }
	    else
	    {   if(userInput.trim().toUpperCase().startsWith(SIGNOFF_TOKEN))
	        {   done= true;
		}
		out.println(userInput);  //Write user input to server via socket
		String fromServer= socketScanner.nextLine(); //Read server's reply from socket
		System.out.println("Server reply: " + fromServer);
            }
	}


        // clean up
        try
        {   echoSocket.close();
        }
        catch (IOException eio)
        {   System.err.println(eio);
        }
    }
}
    

