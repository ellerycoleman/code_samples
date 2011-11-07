/*----------------------------------------------------------------------------
# File:		Passenger.java
# Date:		Sun Nov  6 19:05:16 GMT 2011
# Author:	Ellery Coleman <ellerycoleman@fas.harvard.edu>
# Abstract:	Implements an Passenger class for cscie160, hw3.
#-----------------------------------------------------------------------------
# Revision: $Id$
#---------------------------------------------------------------------------*/
package cscie160.hw3;





public class Passenger
{

    //-----------------------
    //    3 Data Members
    //-----------------------
    private String  name;  //helpful for debugging
    private int     currentFloor;
    private int     destFloor;





    //-----------------------
    //     Constructor
    //-----------------------

   /**
    * A constructor that instantiates passengers according
    * to input parameters.
    */
    public Passenger(String name, int currentFloor, int destFloor)
    {   this.name         = name;
        this.currentFloor = currentFloor;
        this.destFloor    = destFloor;
    }





    //-----------------------
    //     Method Members
    //-----------------------


    /*---------------------------------------------------------------------
    | method name: getCurrentFloor
    | return type: int
    | param  type: none
    |    Abstract: getter for currentFloor.
    +--------------------------------------------------------------------*/
   /**
    *  Returns the value of currentFloor.
    */
    public int getCurrentFloor()
    {   return currentFloor;
    }


    /*---------------------------------------------------------------------
    | method name: setCurrentFloor
    | return type: void
    | param  type: int (current floor number)
    |    Abstract: setter for currentFloor.
    +--------------------------------------------------------------------*/
   /**
    *  Sets the value of currentFloor.
    */
    public void setCurrentFloor(int currentFloor)
    {   this.currentFloor= currentFloor;
    }


    /*---------------------------------------------------------------------
    | method name: getDestFloor
    | return type: int
    | param  type: none
    |    Abstract: getter for destFloor.
    +--------------------------------------------------------------------*/
   /**
    *  Returns the value of destFloor.
    */
    public int getDestFloor()
    {   return destFloor;
    }



    /*---------------------------------------------------------------------
    | method name: setDestFloor
    | return type: void
    | param  type: int (destination floor number)
    |    Abstract: getter for destFloor.
    +--------------------------------------------------------------------*/
   /**
    *  Returns the value of destFloor.
    */
    public void setDestFloor(int destFloor)
    {   this.destFloor= destFloor;
    }



    /*---------------------------------------------------------------------
    | method name: arrive
    | return type: void
    | param  type: void
    |    Abstract: Updates the passenger's state upon arrival.
    +--------------------------------------------------------------------*/
   /**
    *  Returns the value of destFloor.
    */
    public void arrive()
    {   System.out.println("You have arrived\n");
    }




    /*---------------------------------------------------------------------
    | method name: toString
    | return type: String
    | param  type: void
    |    Abstract: Returns a string containing Passenger's status.
    +--------------------------------------------------------------------*/
   /**
    *  Returns a String containing Passenger's status.
    */
    public String toString()
    {   String status= "+--- " + name + " ---\n";
        status+= "| currFloor: " + currentFloor + "\n";
        status+= "| destFloor: " + destFloor + "\n";
	status+= "+----------------";
	return status;
    }







    //----------------------------------
    //     Main Method (test harness)
    //----------------------------------
   /**
    * A simple test harness for the Passenger class.
    */
    public static void main(String args[])
    {
        System.out.println("Running test harness...\n");

        // Create a few passengers and display their state.
        Passenger   devin= new Passenger("devin",3,5);
        Passenger vanessa= new Passenger("vanessa",2,4);


        // Display passenger status
	System.out.println(devin.toString());
	System.out.println("\n\n");
	System.out.println(vanessa.toString());

    }
}


