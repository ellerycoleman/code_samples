/*----------------------------------------------------------------------------
# File:		Elevator.java
# Date:		Fri Oct 21 02:40:11 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements an Elevator class for cscie160, hw2.
#-----------------------------------------------------------------------------
# Revision: $Id$
#---------------------------------------------------------------------------*/
package cscie160.hw3;
import java.util.*;





public class Elevator
{

    //-----------------------
    //     Constants
    //-----------------------


   /**
    * defines the base floor in the building
    */
    public static final int baseFloor   = 1;



   /**
    * defines the top floor in the building
    */
    public static final int maxFloor    = 7;


   /**
    * defines the max capacity of an elevator
    */
    public static final int maxCapacity = 10;


   /**
    * The direction of an elevator is set with an
    * enum since there are only 2 possible directions.
    */
    enum Direction { UP, DOWN };







    //-----------------------
    //    6 Data Members
    //-----------------------
    private int floorNum;
    private int passengers;
    private Direction direction;
    private boolean destRequests[];
    private int passengersToFloor[];
    private ArrayList<Floor> Floors;





    //-----------------------
    //     Constructor
    //-----------------------

   /**
    * A constructor that doesn't take any arguments; initializes
    * the elevator with zero passengers, first floor position, and
    * a direction of UP.
    */
    public Elevator()
    {   System.out.print("Initializing elevator...\n");

        // Initializing the 6 data members
	//--------------------------------
        floorNum    = 1;
        passengers  = 0;
        direction   = Direction.UP;
        destRequests       = new boolean[maxFloor+1];
        passengersToFloor  = new int[maxFloor+1];
        Floors             = new ArrayList<Floor>(maxFloor+1);
        for(int i=0; i<=maxFloor; i++)
        {   Floors.add(new Floor(i));
        }
    }





    //-----------------------
    //     Method Members
    //-----------------------

    /*---------------------------------------------------------------------
    | method name: toString
    | return type: String
    | param  type: none
    |    Abstract: Overrides toString() method of java.lang.Object;
    |              returns string with status of elevator.
    +--------------------------------------------------------------------*/
   /**
    *  Returns a string containing the status of the elevator.
    */
    public String toString()
    {   String status,requests;
        requests="";
        status= "+--------Elevator-----------"               + "\n" +
                "|         current Floor: "  + floorNum       + "\n" +
                "|    current passengers: "  + passengers  + "\n" +
                "|     current direction: "  + ((direction == Direction.UP) ? "up":"down")   + "\n" +
                "|  destination requests: ";

        for(int i=1; i<=maxFloor; i++)
        {   if((destRequests[i] != false)  || (passengersToFloor[i] != 0))
            {   requests+= "\n" +
                           "|      Floor_" + i + ": " + passengersToFloor[i] + " passengers";
                if(passengersToFloor[i] == 0)
		{   requests+= ", pickup Requested";
		}
            }
        }
        if(requests.isEmpty())
        {   status+= "none";
        }
        else
        {   status+= requests;
        }

        status+= "\n+---------------------------";
        return status;
    }




    /*---------------------------------------------------------------------
    | method name: move
    | return type: void
    | param  type: none
    |    Abstract: Moves elevator by one floor depending on current
    |              direction. Changes direction as appropriate.
    +--------------------------------------------------------------------*/
   /**
    * Moves elevator by one floor depending on current direction.
    * Changes direction as appropriate.
    */
    public void move() throws ElevatorFullException
    {   System.out.println("Leaving floor " + floorNum + " with " + passengers + " passenger(s).");

        if((direction == Direction.UP)  && (floorNum < maxFloor))
        {   floorNum++;
        }
        else if((direction == Direction.UP)  && (floorNum == maxFloor))
        {   direction=Direction.DOWN;
            floorNum--;
        }
        else if ((direction == Direction.DOWN)  && (floorNum > baseFloor))
        {   floorNum--;
        }
        else if ((direction == Direction.DOWN)  && (floorNum == baseFloor))
        {   direction=Direction.UP;
            floorNum++;
        }
        if(destRequests[floorNum] == true)
        {   stop();
        }
    }



    /*---------------------------------------------------------------------
    | method name: stop
    | return type: void
    | param  type: none
    |    Abstract: Stops the elevator and cooperates with the floor to
    |              load and unload passengers as appropriate.
    +--------------------------------------------------------------------*/
   /**
    * Stops the elevator and cooperates with the floor to load and
    * unload passengers as appropriate.
    */
    public void stop() throws ElevatorFullException
    {  System.out.print("\n\n\n\nElevator stopped at floor " + floorNum);
       System.out.println(Floors.get(floorNum));
       Floors.get(floorNum).unloadPassengers(this);
    }




    /*---------------------------------------------------------------------
    | method name: registerRequest
    | return type: void
    | param  type: int (number of floor requesting a stop)
    |    Abstract: Modifies the destRequest array, causing the elevator to
    |              stop on the specified floor on it's next sweep.
    +--------------------------------------------------------------------*/
   /**
    * Signals the elevator to stop on the specified floor during the next
    * sweep.
    */
    public void registerRequest(int floorNum)
    {   if((floorNum >= baseFloor)  &&  (floorNum <= maxFloor))
        {   if(destRequests[floorNum] == false)
	    {   destRequests[floorNum]= true;
	    }
	}
    }




    /*---------------------------------------------------------------------
    | method name: addOccupants
    | return type: void
    | param  type: int,int (number of occupants, floorNum)
    |    Abstract: Adds the specified number of occupants to the specified
    |              floor and registers a pickup request with the elevator.
    |              Ideally this method would be located in the Floor class,
    |              but since the Elevator has the Floors it's cleaner to
    |              put it here.
    +--------------------------------------------------------------------*/
   /**
    * Adds the specified number of occupants to the specified floor and
    * registers a pickup request with the elevator.
    */
    public void addOccupants(int numOfOccupants,int floorNum)
    {   Floors.get(floorNum).addOccupants(numOfOccupants);
        registerRequest(floorNum);
	System.out.println(numOfOccupants + " occupants have been added to floor #" + floorNum);
    }




    /*---------------------------------------------------------------------
    | method name: unloadPassenger
    | return type: void
    | param  type: int (number of passengers to unload)
    |    Abstract: Removes the specified number of passengers from the
    |              elevator.
    +--------------------------------------------------------------------*/
   /**
    * Removes the specified number of passengers from the elevator.
    */
    public void unloadPassenger(int count)
    {   if(count <= passengers)
        {   passengers-= count;                   // reduce elevator passenger count
	    passengersToFloor[floorNum]-= count;  // reduce num of passengers destined for this floor
	    destRequests[floorNum]= false;        // clear the request for this floor
        }
        else
        {   System.err.println("Cannot take " + count + "passengers from elevator.");
            System.err.println("There are only " + passengers + "passengers on the elevator.");
        }
    }



    /*---------------------------------------------------------------------
    | method name: passengersForFloor()
    | return type: int
    | param  type: int (floor number)
    |    Abstract: returns the number of passengers destined for the
    |              specified floor.
    +--------------------------------------------------------------------*/
   /**
    * Returns the number of passengers destined for the specified floor.
    */
    public int passengersForFloor(int aFloorNum)
    {   if((aFloorNum >= baseFloor)  &&  (aFloorNum <= maxFloor))
        {   return passengersToFloor[aFloorNum];
	}
	else
	{   System.err.println("passengersForFloor(), No such floor: " + aFloorNum);
	    return 0;
	}
    }





    /*---------------------------------------------------------------------
    | method name: boardPassenger
    | return type: void
    | param  type: int (floor number that passenger is destined for)
    |    Abstract: Adds a passenger to the elevator and handles the
    |              appropriate accounting.
    +--------------------------------------------------------------------*/
   /**
    * Adds a passenger to the elevator and handles the appropriate 
    * book keeping of increasing the passenger count on the elevator,
    * registering the destination request, and increasing the count of
    * passengers headed to the destination floor.
    */
    public void boardPassenger(int floorNum) throws ElevatorFullException
    {   System.out.println("Boarding one passenger for floor " + floorNum + ".");
        try
	{   if(passengers < maxCapacity)
	    {   passengers++;
                registerRequest(floorNum);
                passengersToFloor[floorNum]++;
            }
	    else
	    {   throw new ElevatorFullException("No room for additional passengers at this time");
	    }
        }
	catch (ElevatorFullException e)
	{   throw e;  // pass the exception up the chain so that
	              // the Floor object can deal with it.
	}
    }







    //----------------------------------
    //     Main Method (test harness)
    //----------------------------------
   /**
    * A test harness for the Elevator class.  
    * According to the HW2 Spec, this method boards an abitrary number
    * of passengers destined for different floors.  It also initializes
    * some of the floors with occupants who are waiting for the elevator.
    * The combination of passengers on the elevator and occupants waiting
    * on the floor causes the elevator to reach it's max and the
    * ElevatorFullException is thrown.
    *
    * Keeping with assumption from the HW1 spec, the elevator uses a simple
    * sweep algorithm; continuously traveling up and down the building.  It
    * only stops when it has passengers to drop off or pick up.
    */
    public static void main(String args[]) throws InterruptedException,ElevatorFullException
    {

        // Initialize the Elevator and Floors
        Elevator ev1= new Elevator();


	// Initialize the test scenario
	System.out.print("\n\n");
	System.out.print("Initializing test scenario...\n");
        ev1.boardPassenger(3);  //board passenger destined for floor 3
        ev1.boardPassenger(6);  //board passenger destined for floor 6
        ev1.boardPassenger(7);  //board passenger destined for floor 7
	ev1.addOccupants(6,3);  //add 6 occupants to floor 3
	ev1.addOccupants(6,4);  //add 6 occupants to floor 4;



        // Run the Elevator
        for(int i=0; i<=50; i++)
        {   Thread.sleep(1);  //sleeping helps interpret the output in real time
            ev1.move();
        }
    }
}


