/*----------------------------------------------------------------------------
# File:		Elevator.java
# Date:		Tue Sep 13 17:13:24 EDT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements an Elevator class for cscie160, hw1.
#-----------------------------------------------------------------------------
# Revision: $Id: Elevator.java 8 2011-10-16 23:57:41Z ellery $
#---------------------------------------------------------------------------*/
package cscie160.hw2;





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
    //     Constructor
    //-----------------------

   /**
    * A constructor that doesn't take any arguments; initializes
    * the elevator with zero passengers, first floor position, and
    * a direction of UP.
    */
    public Elevator()
    {   System.out.println("Initializing elevator...\n");
        this.passengers=0;
        this.floor= 1;
        this.direction= UP;
        Floors= new Floor[maxFloor];
    }
    



    //-----------------------
    //     Data Members
    //-----------------------
    private int floor;
    private int passengers;
    private int direction; // 1 is up, 0 is down
    private int destRequests[]= new int[maxFloor+1];
    private int passengersToFloor[]= new int[maxFloor+1];
    private Floor Floors[];






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
                "|         current Floor: "  + this.floor       + "\n" + 
                "|    current passengers: "  + this.passengers  + "\n" +
                "|     current direction: "  + 
                    ((this.direction == UP) ? "up":"down")   + "\n" +
                "|  destination requests: ";

        for(int i=1; i<=maxFloor; i++)
        {   if((destRequests[i] != 0)  || (passengersToFloor[i] != 0))
            {   requests+= "\n" +
                           "|      Floor_" + (i) + "--> requests: " + 
                           destRequests[i] + ", " +
                           " passengers destined for floor: " +
                           passengersToFloor[i] + "\n";
            }
        }
        if(requests.isEmpty())
        {   status+= "none\n";
        }
        else
        {   status+= requests;
        }
 
        status+= "+---------------------------\n\n\n\n";
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
    {   if((this.direction == UP)  && (this.floor < maxFloor))
        {   this.floor++;
        }
        else if((this.direction == UP)  && (this.floor == maxFloor))
        {   this.direction=DOWN;
            this.floor--;
        }
        else if ((this.direction == DOWN)  && (this.floor > baseFloor))
        {   this.floor--;
        }
        else if ((this.direction == DOWN)  && (this.floor == baseFloor))
        {   this.direction=UP;
            this.floor++;
        }

        if(destRequests[this.floor] > 0)
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
    {  System.out.print("\n\n\nElevator stopped at floor " + this.floor + ", ");
       int unloading= destRequests[floor];
       destRequests[floor]=0;
       passengersToFloor[floor]-= unloading;
       passengers-= unloading;
       System.out.println("dropped off " + unloading + " passenger(s).");
       System.out.println(this.toString());
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
    public void boardPassenger(int floor)
    {   System.out.println("Boarding one passenger for floor " + floor + ".");
        this.passengers++;
        destRequests[floor]++;
        passengersToFloor[floor]++;
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
        {   ev1.move();
            Thread.sleep(1000);  //sleep - helps interpret output in real time
        }
    }
}

  

