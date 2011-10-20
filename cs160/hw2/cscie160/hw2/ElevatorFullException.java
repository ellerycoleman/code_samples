package cscie160.hw2;

    //-----------------------
    //   Custom Exceptions
    //-----------------------
    public class ElevatorFullException extends Exception
    {   public ElevatorFullException(String msg)
        {   super("ElevatorFullException: " + msg);
	}
    }



