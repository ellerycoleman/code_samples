package cscie160.hw6;

import java.net.*;
import java.io.*;
import java.util.*;


/**
 * ATM Server.
 */
public class Server
{

    //---------------
    // Data Members
    //---------------
    private ServerSocket serverSocket;
    private ATM atmImplementation;
    private BufferedReader bufferedReader;
    private ArrayList<ATMRunnable> workOrders;
    



    //--------------
    // Constructor
    //--------------
    public Server(int port) throws java.io.IOException
    {   serverSocket      = new ServerSocket(port);
	atmImplementation = new ATMImplementation();
        workOrders        = new ArrayList<ATMRunnable>();
    }
	




    //-----------------
    // Method Members
    //-----------------

    /** serviceClient accepts a client connection and reads lines from the socket.
     *  Each line is handed to executeCommand for parsing and execution.
     */
    public void serviceClient() throws java.io.IOException
    {
	System.out.println("Server: Accepting clients now");
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
	{   String commandLine;
	    while ((commandLine = bufferedReader.readLine()) != null)
	    {   addWorkOrder(new ATMRunnable(commandLine,atmImplementation,printStream));
	        System.out.println("++ workOrder \"" + commandLine + "\" added to queue");
	    }
	}
	catch (SocketException sException)
	{
	    // client has stopped sending commands.  Exit gracefully.
	    System.out.println("done");
	}
    }




   /**
    * Returns a reference to the workOrder queue.
    */
    public ArrayList<ATMRunnable> getWorkOrders()
    {   return workOrders;   
    }



	

   /**
    * Returns a work order from the workOrder queue.
    * It returns the work order that has been in the queue the longest.
    */
    public ATMRunnable getWorkOrder()
    {   if(workOrders.size() == 0)
        {   return null;
	}
	else
        {   synchronized (workOrders) 
	    {   ATMRunnable a= workOrders.get(0);
                workOrders.remove(0);
		workOrders.notifyAll();
	        return a;
            }
        }
    }



   /**
    * Adds a work order to the workOrder queue.
    * Work orders are added to the end of the queue and retrieved
    * from the beginning of the queue. This provides the
    * first-come-first-served behavior for the program.
    */
    public void addWorkOrder(ATMRunnable a)
    {   synchronized (workOrders)
        {   workOrders.add(a);
            workOrders.notifyAll();
        }
    }
	



	
    public static void main(String argv[])
    {   int port = 1099;


        // Retrieve port number from command line
	//----------------------------------------
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
	{   
	    // Bind to specified port 
	    //------------------------
	    Server server = new Server(port);



            // Create a pool of 5 threads to handle client requests
	    //------------------------------------------------------
    	    Thread thread0= new Thread(new ATMThread(0,server.getWorkOrders())); thread0.start();
	    Thread thread1= new Thread(new ATMThread(1,server.getWorkOrders())); thread1.start();
	    Thread thread2= new Thread(new ATMThread(2,server.getWorkOrders())); thread2.start();
	    Thread thread3= new Thread(new ATMThread(3,server.getWorkOrders())); thread3.start();
	    Thread thread4= new Thread(new ATMThread(4,server.getWorkOrders())); thread4.start();



            // Listen for client requests and place them into workOrder queue
	    //----------------------------------------------------------------
            while(true)
	    {   server.serviceClient();
            }
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}
    }
}
