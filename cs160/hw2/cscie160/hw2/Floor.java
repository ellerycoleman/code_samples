/*----------------------------------------------------------------------------
# File:		Floor.java
# Date:		Tue Sep 13 17:13:24 EDT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements a Floor class for cscie160, hw2.
#-----------------------------------------------------------------------------
# Revision: $Id: Elevator.java 8 2011-10-16 23:57:41Z ellery $
#---------------------------------------------------------------------------*/
package cscie160.hw2;





public class Floor
{




    //-----------------------
    //     Data Members
    //-----------------------
    private int floorNum;
    private int occupants;




    //-----------------------
    //     Constructor
    //-----------------------

   /**
    * A constructor that doesn't take any arguments; initializes
    * the floor with zero passengers.
    */
    public Floor(int floorNum)
    {   System.out.println("Initializing Floor #" + floorNum + "...\n");
        this.floorNum= floorNum;
        this.occupants=0;
    }






    //-----------------------
    //     Method Members
    //-----------------------

    /*---------------------------------------------------------------------
    | method name: toString
    | return type: String
    |    Abstract: Overrides toString() method of java.lang.Object;
    |              returns string with status of floor.
    +--------------------------------------------------------------------*/
   /**
    *  Returns string with status of floor.
    */
    public String toString()
    {   String status,requests;
        requests="";
        status= "\n+----------Floor " + this.floorNum + "------------" +
                "\n|    current occupants: "  + this.occupants  +
                "\n+---------------------------\n\n";
        return status;
    }




    /*---------------------------------------------------------------------
    | method name: unloadPassengers
    | return type: void
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
        System.out.println("Floor.unloadPassengers() was invoked with this elevator:");
        System.out.println(ev1.toString());
        int unloading= ev1.passengersForFloor(floorNum);
        ev1.unloadPassenger(unloading);
        System.out.println("unloaded " + unloading + " passenger(s).");

	// If the passengers are unloading onto floor #1, we will assume
	// that they're leaving the building.
	if(floorNum != 1)
	{   occupants+= unloading;
	}


	// With the exception of passengers that were just unloaded, load
	// floor occupants onto the elevator.
	int boarding= occupants - unloading;
	if(boarding > 0)
	{   for(int i=1; i<=boarding; i++)
	    {   ev1.boardPassenger(ev1.baseFloor);
	        occupants--;
	    }
        }


	// For this simulation, we assume that the unloaded passengers are
	// ready to leave the floor on the next elevator run.  We will tell
	// the elevator to return for them.
	if(occupants >= 1)
	{   ev1.registerRequest(floorNum);
	}


        // show status of floor before proceeding.
        System.out.println(this.toString());
        System.out.println(ev1.toString());

    }







    //----------------------------------
    //     Main Method (test harness)
    //----------------------------------
   /**
    * A test harness for the Floor class.
    */
    public static void main(String args[]) throws InterruptedException
    {

        Floor f1= new Floor(1);
        System.out.println(f1.toString());
    }
}



