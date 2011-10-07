/*----------------------------------------------------------------------------
# File:		Elevator.java
# Date:		Tue Sep 13 17:13:24 EDT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements an Elevator class for cscie160, hw1.
#-----------------------------------------------------------------------------
# Revision: $Id$
#---------------------------------------------------------------------------*/
package cscie160.hw1;





public class Elevator
{

    //-----------------------
    //     Constants
    //-----------------------
    public static final int maxFloors   = 7;
    public static final int maxCapacity = 10;
    public static final int UP   = 1;
    public static final int DOWN = 0;
    



    //-----------------------
    //     Constructor
    //-----------------------
    public Elevator()
    {   this.passengers=0;
        this.floor= 1;
        this.direction= UP;
    }
    



    //-----------------------
    //     Data Members
    //-----------------------
    private int floor;
    private int passengers;
    private int direction; // 1 is up, 0 is down
    private int destRequests[]= new int[maxFloors+1];
    private int passengersToFloor[]= new int[maxFloors+1];






    //-----------------------
    //     Method Members
    //-----------------------
    
    /*---------------------------------------------------------------------
    | method name: toString
    | return type: String
    |    Abstract: Overrides toString() method of java.lang.Object;
    |              returns string with status of elevator.
    +--------------------------------------------------------------------*/
    public String toString()
    {   String status,requests;
        requests="";
        status= "+--------Elevator-----------"               + "\n" + 
                "|         current Floor: "  + this.floor       + "\n" + 
                "|    current passengers: "  + this.passengers  + "\n" +
                "|     current direction: "  + 
                    ((this.direction == UP) ? "up":"down")   + "\n" +
                "|  destination requests: ";

        for(int i=1; i<=maxFloors; i++)
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
 
        status+= "+---------------------------\n";
        return status;
    }





    /*---------------------------------------------------------------------
    | method name: toString
    | return type: String
    |    Abstract: Overrides 
    +--------------------------------------------------------------------*/
    public void pickupRequest(int upOrDown, int destFloor)
    {   
    }






    /*---------------------------------------------------------------------
    | method name: boardPassenger
    | return type: void
    |    Abstract: Adds a passenger to the elevator and handles the
    |              appropriate accounting.
    +--------------------------------------------------------------------*/
    public void boardPassenger(int floor)
    {   this.passengers++;
        destRequests[floor]++;
        passengersToFloor[floor]++;
    }





    //----------------------------------
    //     Main Method (test harness)
    //----------------------------------
    public static void main(String args[]) throws InterruptedException
    {   //Thread.sleep(3000); //showing splash image

        Elevator ev1= new Elevator();
        System.out.println("\n\n\n" +
                           "#-----------------------\n"   +
                           "#             BEFORE\n" +
                           "#-----------------------\n"
                          );
        System.out.println(ev1);


        ev1.boardPassenger(5);
        System.out.println("\n\n\n" +
                           "#-----------------------\n"   +
                           "#             AFTER\n" +
                           "#-----------------------\n"
                          );
        System.out.println(ev1);



    }


}

  

