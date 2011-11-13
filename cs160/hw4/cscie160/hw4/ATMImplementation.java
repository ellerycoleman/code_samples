

package cscie160.hw4;



public class ATMImplementation implements ATM
{

    // Data Members
    //--------------
    private Account account;




    // Constructor
    //-------------
    public ATMImplementation()
    {   account= new Account();
    }
    



    // Method members
    //-----------------
    public void deposit(float amount) throws ATMException
    {   System.out.print("ATMImplementation.deposit() has been invoked.\n");
    }

    public void withdraw(float amount) throws ATMException
    {   System.out.print("ATMImplementation.withdraw() has been invoked.\n");
    }

    public Float getBalance() throws ATMException
    {   System.out.print("ATMImplementation.getBalance() has been invoked.\n");
        return account.getBalance();
    }
}


