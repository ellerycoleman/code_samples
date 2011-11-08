/*----------------------------------------------------------------------------
# File:		Floor.java
# Date:		Fri Oct 21 02:32:55 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements a Floor class for cscie160, hw2.
#-----------------------------------------------------------------------------
# Revision: $Id$
#---------------------------------------------------------------------------*/
package cscie160.hw3;
import java.util.*;




public class Floor
{


    //-----------------------
    //     5 Data Members
    //-----------------------
    private int floorNum;
    private int occupants;
    private ArrayList<Passenger> resident;
    private ArrayList<Passenger> upQueue;
    private ArrayList<Passenger> downQueue;



    //-----------------------
    //     Constructor
    //-----------------------

   /**
    * A constructor that doesn't take any arguments; initializes
    * the floor with zero occupants.
    */
    public Floor(int floorNum)
    {   System.out.print("Initializing Floor #" + floorNum + "...\n");

        // Initializing 5 data members
        this.floorNum= floorNum;
        this.occupants=0;
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
                "\n|    current occupants: "  + occupants  +
                "\n+---------------------------\n\n";
        return status;
    }




    /*---------------------------------------------------------------------
    | method name: addOccupants
    | return type: void
    | param  type: int (number of occupants)
    |    Abstract: Adds the specified number of occupants to the Floor.
    +--------------------------------------------------------------------*/
   /**
    *  Adds the specified number of occupants to the Floor.
    */
    public void addOccupants(int numOfOccupants)
    {   occupants+= numOfOccupants;
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
    *  Unloads passengers destined for the floor and loads any waiting
    *  passengers.
    */
    public void unloadPassengers(Elevator ev1) throws ElevatorFullException
    {   
        // Unload passengers from elevator
	//---------------------------------- 
        int unloading= ev1.passengersForFloor(floorNum);
        ev1.unloadPassenger(unloading);
        System.out.println("unloaded " + unloading + " passenger(s).");


	// If the passengers are unloading onto floor #1, we assume that
	// they'll leave the building once they exit the elevator.
	//--------------------------------------------------------------- 
	if(floorNum != 1)
	{   occupants+= unloading;
	}


	// With the exception of passengers that were just unloaded, load
	// current floor occupants onto the elevator.  According to the spec,
	// every occupant on a floor will be boarded with a destination of
	// baseFloor.
	//---------------------------------------------------------------------- 
	int boarding= occupants - unloading;
	if(boarding > 0)
	{   for(int i=1; i<=boarding; i++)
	    {   try
	        {   ev1.boardPassenger(ev1.baseFloor);
	            occupants--;
                }
		catch (ElevatorFullException e)
		{   System.out.print("ElevatorFullException Caught:");
		    System.out.print(" leaving " + occupants + " occupants on the floor; ");
		    System.out.print(" will return later.\n");
		    i=boarding+1;  //break out of boarding loop.  
		}
	    }
        }


	// For this simulation, we assume that all unloaded passengers will
	// be ready to leave the building on the next elevator run.  Thus we
	// will signal the elevator to return for them.
	//--------------------------------------------------------------------- 
	if(occupants >= 1)
	{   ev1.registerRequest(floorNum);
	}


        // show status of floor before proceeding.
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



