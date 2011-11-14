package cscie160.hw4;

import java.net.*;
import java.io.*;

/** Client-side proxy class which manages the connection to the server.
 *  The proxy forwards the client's requests to the server by writing
 *  the text of requests to the server's socket.
 */

public class ATMProxy implements ATM
{
    // Data Members
    //---------------
    private Socket socket;
    private PrintStream  printStream;
    BufferedReader inputReader;
	


    // Constructor
    //--------------
    public ATMProxy(String host, int port) throws UnknownHostException, java.io.IOException
    {
	socket = new Socket(host, port);
	OutputStream outputStream = socket.getOutputStream();
	printStream = new PrintStream(outputStream);
	InputStream inputStream = socket.getInputStream();
	InputStreamReader inputStreamReader = new InputStreamReader(inputStream);
	inputReader = new BufferedReader(inputStreamReader);
    }
	


    // Method Members
    //----------------
    public void deposit(float amount) throws ATMException
    {
	// Commands is an enum in this package
	//System.out.println("ATMProxy writing command to server: " + Commands.DEPOSIT);
	printStream.println(Commands.DEPOSIT + " " + amount);
    }
	


    public void withdraw(float amount) throws ATMException
    {
	//System.out.println("ATMProxy writing command to server: " + Commands.WITHDRAW);
	printStream.println(Commands.WITHDRAW +  " " +  amount);
    }
	


    public Float getBalance() throws ATMException
    {
	//System.out.println("ATMProxy writing command to server: " + Commands.BALANCE);
	printStream.println(Commands.BALANCE);
	try
	{
	    String response = inputReader.readLine();
	    if (response != null)
	    {
		//System.out.println("Server returned: " + response);
		return Float.parseFloat(response.trim());
	    }
	    else
	    {
		throw new ATMException("ATMProxy: Unexpected end of stream reading commands in getBalance()");
	    }
				
	}
	catch (Exception ex)
	{
	    throw new ATMException(ex.toString());
	}
    }
}
