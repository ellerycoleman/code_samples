/*----------------------------------------------------------------------------
# File:		Elevator.java
# Date:		Tue Sep 13 17:13:24 EDT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements an Elevator class for cscie160, hw1.
#-----------------------------------------------------------------------------
# Revision: $Id: Elevator.java 8 2011-10-16 23:57:41Z ellery $
#---------------------------------------------------------------------------*/
package cscie160.hw2;
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
    * integer variable; 1 for UP.
    */
    public static final int UP          = 1;


   /**
    * The direction of an elevator is set with an
    * integer variable; 0 for DOWN.
    */
    public static final int DOWN        = 0;







    //-----------------------
    //     Data Members
    //-----------------------
    private int floorNum;
    private int passengers;
    private int direction; // 1 is up, 0 is down
    private int destRequests[]= new int[maxFloor+1];
    private int passengersToFloor[]= new int[maxFloor+1];
    private ArrayList<Floor> Floors= new ArrayList<Floor>(maxFloor+1);





    //-----------------------
    //     Constructor
    //-----------------------

   /**
    * A constructor that doesn't take any arguments; initializes
    * the elevator with zero passengers, first floor position, and
    * a direction of UP.
    */
    public Elevator()
    {   System.out.println("Initializing elevator...\n");
        passengers=0;
        floorNum= 1;
        direction= UP;
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
    |    Abstract: Overrides toString() method of java.lang.Object;
    |              returns string with status of elevator.
    +--------------------------------------------------------------------*/
   /**
    *  Returns string with status of elevator.
    */
    public String toString()
    {   String status,requests;
        requests="";
        status= "+--------Elevator-----------"               + "\n" +
                "|         current Floor: "  + this.floorNum       + "\n" +
                "|    current passengers: "  + this.passengers  + "\n" +
                "|     current direction: "  +
                    ((this.direction == UP) ? "up":"down")   + "\n" +
                "|  destination requests: ";

        for(int i=1; i<=maxFloor; i++)
        {   if((destRequests[i] != 0)  || (passengersToFloor[i] != 0))
            {   requests+= "\n" +
                           "|      Floor_" + (i) + "--> requests: " + destRequests[i];
            }
        }
        if(requests.isEmpty())
        {   status+= "none";
        }
        else
        {   status+= requests;
        }

        status+= "\n+---------------------------\n\n";
        return status;
    }




    /*---------------------------------------------------------------------
    | method name: move
    | return type: void
    |    Abstract: Moves elevator by one floor depending on current
    |              direction.
    +--------------------------------------------------------------------*/
   /**
    * Moves elevator by one floor depending on current direction.
    * Changes direction as appropriate.
    */
    public void move()
    {   System.out.println("Leaving floor " + floorNum);

        if((this.direction == UP)  && (this.floorNum < maxFloor))
        {   this.floorNum++;
        }
        else if((this.direction == UP)  && (this.floorNum == maxFloor))
        {   this.direction=DOWN;
            this.floorNum--;
        }
        else if ((this.direction == DOWN)  && (this.floorNum > baseFloor))
        {   this.floorNum--;
        }
        else if ((this.direction == DOWN)  && (this.floorNum == baseFloor))
        {   this.direction=UP;
            this.floorNum++;
        }

        if(destRequests[this.floorNum] > 0)
        {   stop();
        }
    }



    /*---------------------------------------------------------------------
    | method name: stop
    | return type: void
    |    Abstract: Stops the elevator, does the appropriate book keeping,
    |              and then displays the state of the elevator after the
    |              processing.
    +--------------------------------------------------------------------*/
   /**
    * Stops the elevator, does the appropriate book keeping,
    * and then displays the state of the elevator after the
    * processing.
    */
    public void stop()
    {  System.out.print("\n\n\nElevator stopped at floor " + this.floorNum + ", ");
       System.out.println(Floors.get(floorNum));
       Floors.get(floorNum).unloadPassengers(this);
       //int unloading= passengersForFloor(floorNum);
       //unloadPassenger(unloading);
       //System.out.println("dropped off " + unloading + " passenger(s).");
       //System.out.println(this.toString());
    }



    /*---------------------------------------------------------------------
    | method name: unloadPassenger
    | return type: void
    | param  type: int (number of passengers to unload)
    |    Abstract: Removes a passenger from the elevator.
    +--------------------------------------------------------------------*/
   /**
    * Removes the specified number of passengers from the elevator.
    */
    public void unloadPassenger(int count)
    {   if(count <= passengers)
        {   passengers-= count;                   // reduce elevator passenger count
	    passengersToFloor[floorNum]-= count;  // reduce num of passengers destined for this floor
	    destRequests[floorNum]-= count;       // reduce num of requests for this floor
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
    |    Abstract: Adds a passenger to the elevator and handles the
    |              appropriate accounting.
    +--------------------------------------------------------------------*/
   /**
    * Adds a passenger to the elevator and handles the appropriate class
    * book keeping of increasing the passenger count on the elevator,
    * registering the destination request, and increasing the count of
    * passengers headed to the destination floor.
    */
    public void boardPassenger(int floorNum)
    {   System.out.println("Boarding one passenger for floor " + floorNum + ".");
        this.passengers++;
        destRequests[floorNum]++;
        passengersToFloor[floorNum]++;
    }





    //----------------------------------
    //     Main Method (test harness)
    //----------------------------------
   /**
    * A test harness for the Elevator class.  According to HW1 Spec, this
    * method boards 2 passengers for the 2nd floor and 1 for the 3rd floor.
    * In accordance with the Spec the elevator continues to sweep the building
    * after servicing these passengers.
    */
    public static void main(String args[]) throws InterruptedException
    {

        Elevator ev1= new Elevator();
        ev1.boardPassenger(2);
        ev1.boardPassenger(2);
        ev1.boardPassenger(3);

        while(true)
        {   
            Thread.sleep(1000);  //sleep - helps interpret output in real time
            ev1.move();
        }
    }
}



