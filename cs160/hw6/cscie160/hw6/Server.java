package cscie160.hw6;

import java.net.*;
import java.io.*;
import java.util.*;


/**
 * ATM Server.
 */
public class Server
{

    // Data Members
    //---------------
    private ServerSocket serverSocket;
    private ATM atmImplementation;
    private BufferedReader bufferedReader;
    private ArrayList<ATMRunnable> workOrders = new ArrayList<ATMRunnable>();



    // Constructor
    //--------------
    public Server(int port) throws java.io.IOException
    {
        serverSocket = new ServerSocket(port);
	atmImplementation = new ATMImplementation();
    }
	




    // Method Members
    //-----------------

    /** serviceClient accepts a client connection and reads lines from the socket.
     *  Each line is handed to executeCommand for parsing and execution.
     */
    public void serviceClient() throws java.io.IOException
    {
	System.out.println("Accepting clients now");
	Socket clientConnection = serverSocket.accept();
	
	// Arrange to read input from the Socket	
	InputStream inputStream = clientConnection.getInputStream();
	bufferedReader = new BufferedReader(new InputStreamReader(inputStream));
	
	// Arrange to write result across Socket back to client
	OutputStream outputStream = clientConnection.getOutputStream();
        PrintStream printStream = new PrintStream(outputStream);
		
	System.out.println("Client acquired on port #" +
	                   serverSocket.getLocalPort() + ", reading from socket");
		
	try
	{
	    String commandLine;
	    while ((commandLine = bufferedReader.readLine()) != null)
	    {
		try
		{
		    /*
		  |  Float result = executeCommand(commandLine);
		  | // Only BALANCE command returns non-null
		  |  if (result != null)
		  |  {   printStream.println(result);  // Write it back to the client
		  |  }
		    */

		    System.out.println("Attempting to add ATMRunnable to list...");
                    workOrders.add(new ATMRunnable(commandLine,atmImplementation,printStream));
		    executeCommand(getWorkOrder());


		}
		catch (ATMException atmex)
		{   System.out.println("el: something here...");
		    System.out.println("ERROR: " + atmex);
		}

	    }
	}
	catch (SocketException sException)
	{
	    // client has stopped sending commands.  Exit gracefully.
	    System.out.println("done");
	}
    }


    public synchronized ATMRunnable getWorkOrder()
    {   ATMRunnable a= workOrders.get(0);
        workOrders.remove(0);
	return a;
    }
	



	/** The logic here is specific to our protocol.  We parse the string
	 *  according to that protocol.
	 */
	private void executeCommand(ATMRunnable workOrder) throws ATMException
	{
	        System.out.println("executeCommand() has been invoked!");
	        String commandLine= workOrder.getCommand();
		PrintStream clientOut= workOrder.getPrintStream();
		ATM atm= workOrder.getATM();


		System.out.println("Server received command: " + commandLine);


		// Break out the command line into String[]
		StringTokenizer tokenizer = new StringTokenizer(commandLine);
		String commandAndParam[] = new String[tokenizer.countTokens()];
		int index = 0;
		while (tokenizer.hasMoreTokens())
		{
			commandAndParam[index++] = tokenizer.nextToken();
		}

                
		// store the command
		String command = commandAndParam[0];


		// Dispatch BALANCE request without further ado.
		if (command.equalsIgnoreCase(Commands.BALANCE.toString()))
		{       System.out.println("el: Client wants to check balance...");
			clientOut.println(atm.getBalance());
			return;
		}


		// Must have 2nd arg for amount when processing DEPOSIT/WITHDRAW commands
		if (commandAndParam.length < 2)
		{   throw new ATMException("Missing amount for command \"" + command + "\"");
		}



                // execute deposit or withdraw command
		try
		{
		    float amount = Float.parseFloat(commandAndParam[1]);
		    if (command.equalsIgnoreCase(Commands.DEPOSIT.toString()))
		    {
			atm.deposit(amount);
			return;
		    }
		    else if (command.equalsIgnoreCase(Commands.WITHDRAW.toString()))
		    {
			atm.withdraw(amount);
			return;
		    }
		    else
		    {
			throw new ATMException("Unrecognized command: " + command);
		    }
		}
		catch (NumberFormatException nfe)
		{
			throw new ATMException("Unable to make float from input: " + commandAndParam[1]);
		}
		// BALANCE command returned result above.  Other commands return null;
      }




	
    public static void main(String argv[])
    {   int port = 1099;
	if(argv.length > 0)
	{
	    try
	    {
	        port = Integer.parseInt(argv[0]);
	    }	
	    catch (Exception e)
	    {
	        e.printStackTrace();
   	    }
        }
	try
	{   Server server = new Server(port);
            while(true)
	    {   server.serviceClient();
	        System.out.println("Client serviced");
            }
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}
    }
}
