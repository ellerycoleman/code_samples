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
    private Direction direction;
    private ArrayList<Passenger> passengers;
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
        direction   = Direction.UP;
        passengers  = new ArrayList<Passenger>();
        destRequests       = new boolean[maxFloor+1];
        passengersToFloor  = new int[maxFloor+1];
        Floors             = new ArrayList<Floor>(maxFloor+1);
        for(int i=0; i<=maxFloor; i++)
        {   Floors.add(new Floor(i));
        }



	//debug
	System.out.print("The initial size of passengers ArrayList: " + passengers.size() + "\n\n");
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
                "|    current passengers: "  + passengers.size()  + "\n" +
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
    {   System.out.println("Leaving floor " + floorNum + " with " + passengers.size() + " passenger(s).");

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
    {  System.out.print("\n\n\n\n" + 
                        "Elevator stopped at floor " + floorNum + " with " +
			 passengers.size() + " passengers\n");
       System.out.println(Floors.get(floorNum));
       Floors.get(floorNum).unloadPassengers(this);


       // display all passengers
       System.out.println("These passengers are on the elevator: \n");
       for (Passenger p : passengers)
       {   System.out.print(p.toString() + "\n\n");
       }
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
    | method name: addOccupant
    | return type: void
    | param  type: Passenger 
    |    Abstract: Adds the specified Passenger to the specified Floor and
    |              registers a pickup request with the elevator.
    |              The Passenger is placed into the appropriate ArrayList
    |              for the appropriate Floor based on the values of its
    |              data members.
    |
    |              Ideally this method would be located in the Floor class,
    |              but since the Elevator has the Floors it's cleaner to
    |              put it here.
    +--------------------------------------------------------------------*/
   /**
    * Adds the specified Passenger to the specified Floor and registers a
    * pickup request with the elevator.  The Passenger is placed into the
    * appropriate queue (resident, downQueue, upQueue) based on the values
    * of its data members.
    */
    public void addOccupant(Passenger p)
    {   int floorNum= p.getCurrFloor();
        Floors.get(floorNum).addOccupant(p);
        registerRequest(floorNum);
	System.out.println("Occupant '" + p.getName() + "' has been added to floor #" + floorNum);
    }




    /*---------------------------------------------------------------------
    | method name: unloadPassenger
    | return type: Passenger
    | param  type: int (destination floor number)
    |    Abstract: Removes a passenger destined for the specified floor.
    |              Returns the Passenger object to the Floor object.
    +--------------------------------------------------------------------*/
   /**
    * Removes a passenger destined for a specified floor.  Returns the
    * Passenger object that was removed from the elevator.
    */
    public Passenger unloadPassenger(int destFloor)
    {   Passenger p= new Passenger();
    
        if(passengers.size() > 0)
        {   for(int i=0; i<passengers.size(); i++)
	    {   if(passengers.get(i).getDestFloor() == destFloor)
	        {   System.out.println("removing " + passengers.get(i).getName() + "\n");
		    p.setName(passengers.get(i).getName());
		    p.setCurrFloor(passengers.get(i).getCurrFloor());
		    p.setDestFloor(passengers.get(i).getDestFloor());
        	    passengers.remove(i);
        	    passengers.trimToSize();
	            passengersToFloor[floorNum]-= 1; // reduce num of passengers destined for this floor
	            destRequests[floorNum]= false;   // clear the request for this floor
		    i=maxCapacity + 1;               // break out of for-loop
                }
            }
        }
        else
        {   System.err.println("There are no passengers on this Elevator.\n");
        }


	return p;
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
    | param  type: String, int, int  (name of passenger, currFloor, destFloor)
    |    Abstract: Initializes a Passenger object according to the supplied
    |              parameters.  Adds the initialized Passenger object to
    |              the passengers ArrayList.
    +--------------------------------------------------------------------*/
   /**
    * Adds a passenger to the elevator and handles the appropriate
    * book keeping of increasing the passenger count on the elevator,
    * registering the destination request, and increasing the count of
    * passengers headed to the destination floor.
    */
    public void boardPassenger(String name, int currFloor, int destFloor) throws ElevatorFullException
    {   System.out.println("Boarding one passenger for floor " + destFloor + ".");
        try
	{   if(passengers.size() < maxCapacity)
	    {   passengers.add(new Passenger(name,currFloor,destFloor));
                registerRequest(destFloor);
                passengersToFloor[destFloor]++;
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
        ev1.boardPassenger("ellery",1,3);  //board passenger destined for floor 3
        ev1.boardPassenger("keyan",1,6);   //board passenger destined for floor 6
        ev1.boardPassenger("torre",1,7);   //board passenger destined for floor 7

	// Adding 6 passengers to floor 3
	for(int i=1; i<=6; i++)
	{   int currFloor= 3;
	    int destFloor= 1;
	    String name= "p" + i + "_start" + currFloor + "_dest" + destFloor;
	    Passenger p= new Passenger(name,currFloor,destFloor);
	    ev1.Floors.get(currFloor).addOccupant(p);
	}


	// Adding 6 passengers to floor 4
	for(int i=7; i<=12; i++)
	{   int currFloor= 4;
	    int destFloor= 1;
	    String name= "p" + i + "_start" + currFloor + "_dest" + destFloor;
	    Passenger p= new Passenger(name,currFloor,destFloor);
	    ev1.Floors.get(currFloor).addOccupant(p);
	}



        //display all passengers
	for ( Passenger p : ev1.passengers)
	{   System.out.println(p.toString() + "\n\n");
	}


        // Run the Elevator
        for(int i=0; i<=50; i++)
        {   Thread.sleep(1);  //sleeping helps interpret the output in real time
            ev1.move();
        }
    }
}


