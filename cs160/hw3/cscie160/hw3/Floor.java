/*----------------------------------------------------------------------------
# File:		Floor.java
# Date:		Fri Oct 21 02:32:55 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements a Floor class for cscie160, hw3.
#-----------------------------------------------------------------------------
# Revision: $Id$
#---------------------------------------------------------------------------*/
package cscie160.hw3;
import java.util.*;




public class Floor
{


    //-----------------------
    //     4 Data Members
    //-----------------------
    private int floorNum;
    private ArrayList<Passenger> resident;
    private ArrayList<Passenger> upQueue;
    private ArrayList<Passenger> downQueue;



    //-----------------------
    //     Constructor
    //-----------------------

   /**
    * A constructor that doesn't take any arguments; initializes
    * the floor with empty occupant queues.
    */
    public Floor(int floorNum)
    {   System.out.print("  * Initializing Floor #" + floorNum + "...\n");

        // Initializing 5 data members
        this.floorNum= floorNum;
	resident  = new ArrayList<Passenger>();
	upQueue   = new ArrayList<Passenger>();
	downQueue = new ArrayList<Passenger>();
    }






    //-----------------------
    //     Method Members
    //-----------------------

    /*---------------------------------------------------------------------
    | method name: toString
    | return type: String
    | param  type: none
    |    Abstract: Overrides toString() method of java.lang.Object;
    |              returns string with status of floor.
    +--------------------------------------------------------------------*/
   /**
    *  Returns a string containing the current status of the Floor.
    */
    public String toString()
    {   String status,requests;
        requests="";
        status= "\n+----------Floor " + floorNum + "----------" +
                "\n|    resident: "   + resident.size()   +
                "\n|   downQueue: "   + downQueue.size()  +
                "\n|     upQueue: "   + upQueue.size()    +
                "\n+---------------------------\n\n";
        return status;
    }



    /*---------------------------------------------------------------------
    | method name: getResidentSize
    | return type: int
    | param  type: none
    |    Abstract: Returns the number of passengers waiting in the resident
    |              queue of the Floor.
    +--------------------------------------------------------------------*/
   /**
    *  Returns the number of passengers waiting in the Resident Queue
    *  of the Floor.
    */
    public int getResidentSize()
    {   return resident.size();
    }






    /*---------------------------------------------------------------------
    | method name: showResidentQueue
    | return type: void
    | param  type: none
    |    Abstract: Displays Resident Queue.
    +--------------------------------------------------------------------*/
   /**
    *  Displays the resident queue.
    */
    public void showResidentQueue()
    {   int queueSize= resident.size();
        System.out.print("#------------------------------\n" +
	                 "#     Resident Queue \n" +
			 "#-------------------------------\n");
        if(queueSize == 0)
	{   System.out.print("Queue is empty.\n");
	}
	else
	{   for(int i=0; i<queueSize; i++)
	    {   System.out.println(resident.get(i));
	    }
        }
    }







    /*---------------------------------------------------------------------
    | method name: getDownQueueSize
    | return type: int
    | param  type: none
    |    Abstract: Returns the number of passengers waiting in the 
    |              downQueue of the Floor.
    +--------------------------------------------------------------------*/
   /**
    *  Returns the number of passengers waiting in the downQueue of the
    *  Floor.
    */
    public int getDownQueueSize()
    {   return downQueue.size();
    }





    /*---------------------------------------------------------------------
    | method name: getUpQueueSize
    | return type: int
    | param  type: none
    |    Abstract: Returns the number of passengers waiting in the 
    |              upQueue of the Floor.
    +--------------------------------------------------------------------*/
   /**
    *  Returns the number of passengers waiting in the upQueue of the
    *  Floor.
    */
    public int getUpQueueSize()
    {   return upQueue.size();
    }





    /*---------------------------------------------------------------------
    | method name: addOccupant
    | return type: void
    | param  type: Passenger
    |    Abstract: Adds the Passenger to the appropriate Floor queue
    |              based on the values of its data members.
    +--------------------------------------------------------------------*/
   /**
    *  Adds the Passenger to the appropriate Queue (resident, downQueue,
    *  upQueue) based on the values of its data members.
    */
    public void addOccupant(Passenger p)
    {   // if Passenger's destFloor is below the current Floor,
        // then place passenger in downQueue.
	//------------------------------------------------------
	if(p.getDestFloor() < floorNum)
	{   downQueue.add(p);
	}


        // if Passenger's destFloor is above the current Floor,
        // then place passenger in upQueue.
	//------------------------------------------------------
	else if(p.getDestFloor() > floorNum)
	{   upQueue.add(p);
	}


        // if Passenger's destFloor is equal to the current Floor,
        // then place passenger in resident queue.
	//---------------------------------------------------------
	else if(p.getDestFloor() == floorNum)
	{   resident.add(p);
	}

    }




    /*---------------------------------------------------------------------
    | method name: unloadPassengers
    | return type: void
    | param  type: Elevator (the elevator that is stopped at this floor)
    |    Abstract: For hw2, the floor object has the responsibility of
    |              unloading and loading passengers.  It will accomplish
    |              this via method members of the Elevator class.
    +--------------------------------------------------------------------*/
   /**
    *  Unloads passengers destined for the floor and loads any Passengers
    *  that are waiting to go in the same direction as the Elevator.
    */
    public void unloadPassengers(Elevator ev1) throws ElevatorFullException
    {
        // Unload passengers from elevator. Unloaded passengers are placed
	// in the 'resident' queue.
	//-----------------------------------------------------------------
        int unloadCount= ev1.passengersForFloor(floorNum);
	for(int i=0; i<unloadCount; i++)
	{   resident.add(ev1.unloadPassenger(floorNum));
	}



	// Load any passengers that are headed in the same direction as
	// the Elevator.
	//---------------------------------------------------------------
	if(ev1.getDirection() == Elevator.Direction.DOWN)
	{   if(downQueue.size() > 0)
	    {   System.out.print("Boarding " + downQueue.size() + " passengers from downQueue.\n");
		int boardingQueue= downQueue.size();
		int j=0;
	        for(int i=0; i<boardingQueue; i++)
		{   try
	            {   ev1.boardPassenger(downQueue.get(j).getName(),
		                           floorNum,
					   downQueue.get(j).getDestFloor()
					  );
		        downQueue.remove(j);
                    }
		    catch (ElevatorFullException e)
		    {   System.out.print("ElevatorFullException Caught:");
		        System.out.print(" leaving " + downQueue.size() + " occupants on the floor; ");
		        System.out.print(" will return later.\n");
			boardingQueue=0;  //break out of for-loop
		    }
		}
            }
        }

        
	if(ev1.getDirection() == Elevator.Direction.UP)
	{   if(upQueue.size() > 0)
	    {   System.out.print("Boarding " + upQueue.size() + " passengers from upQueue.\n");

		int boardingQueue= upQueue.size();
	        int j=0;
	        for(int i=0; i<boardingQueue; i++)
		{   try
	            {   ev1.boardPassenger(upQueue.get(j).getName(),
		                           floorNum,
					   upQueue.get(j).getDestFloor()
					  );
		        upQueue.remove(j);
                    }
		    catch (ElevatorFullException e)
		    {   System.out.print("ElevatorFullException Caught:");
		        System.out.print(" leaving " + upQueue.size() + " occupants on the floor; ");
		        System.out.print(" will return later.\n");
			boardingQueue=0;  //break out of for-loop
		    }
		}
            }
        }



        // show status of floor before proceeding.
	//----------------------------------------
        System.out.println(this);
        System.out.println(ev1 + "\n");
    }







    //----------------------------------
    //     Main Method (test harness)
    //----------------------------------
   /**
    * A simple test harness for the Floor class.
    */
    public static void main(String args[]) throws InterruptedException
    {
        Floor f1= new Floor(1);
        System.out.println(f1.toString());
    }
}


