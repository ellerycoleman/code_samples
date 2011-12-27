package cscie160.project;
import java.rmi.*;

/**
 * Interface for an ATM.
 */
public interface ATM extends Remote
{
    // Method Members
    public void  deposit(int account, float amount) throws ATMException, RemoteException;
    public void  withdraw(int account, float amount) throws ATMException, RemoteException;
    public Float getBalance(int account) throws ATMException, RemoteException;
    public void  transfer(int fromAccount, int toAccount, float amount) throws ATMException, RemoteException;
}
