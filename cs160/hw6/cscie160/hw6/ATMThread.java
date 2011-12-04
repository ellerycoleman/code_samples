/*----------------------------------------------------------------------------
# File:		ATMThread.java
# Date:		Mon Nov 14 03:08:07 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements an ATMThread class for cscie160, hw6.
#-----------------------------------------------------------------------------
# Revision: $Id$
#---------------------------------------------------------------------------*/
package cscie160.hw6;


import java.io.*;
import java.util.*;


/**
 * Implements ATMThread, which acts as an agent to carry out client requests.
 */

public class ATMThread implements Runnable
{


    //----------------
    // 2 Data Members
    //----------------
    public int id;                             // id number for this thread
    ArrayList<ATMRunnable> workOrders;         // list of client work orders





    //---------------
    // Constructor
    //---------------

   /**
    * A constructor that sets the ID for this thread and provides a
    * reference to the workOrder queue.
    */
    public ATMThread(int i, ArrayList<ATMRunnable> arraylist)
    {   id= i;
        workOrders= arraylist;
    }





    //-----------------
    // Method Members
    //-----------------

   /**
    * Waits for a client work order to show up on the workOrder queue,
    * and then processes the work order.
    */
    public void run()
    {   boolean firstPass= true;
        while(true)
	{   
	    // If queue is empty, call wait() until another thread
	    // adds a work order to the queue.
	    //-----------------------------------------------------
	    synchronized (workOrders)
	    {   while(workOrders.size() == 0)
	        {   if(firstPass)
		    {   System.out.println("Thread-" + id + ": " +
		                           "No work orders to process; waiting...");
                        firstPass= false;
                    }
		    try
		    {   workOrders.wait();
		    }
		    catch(Exception e)
		    {   System.out.println(e);
		    }
		}


	        // Retrieve the ATMRunnable from the work order queue and
	        // execute it.
		//----------------------------------------------------------
		ATMRunnable workorder= this.workOrders.get(0);
		workOrders.remove(0);

		try
		{   System.out.println("Running request in Thread-" + id); 
		    workorder.run();
		}
		catch (ATMException a)
		{   System.out.println(a);
		}

		workOrders.notifyAll();
            }
        }
    }
}
