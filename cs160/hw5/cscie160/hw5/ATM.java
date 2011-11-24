package cscie160.hw5;
import java.rmi.*;

/**
 * Interface for an ATM.
 */
public interface ATM extends Remote
{
    // Method Members
    public void deposit(int account, float amount) throws ATMException, RemoteException;
    public void withdraw(int account, float amount) throws ATMException, RemoteException;
    public Float getBalance(int account) throws ATMException, RemoteException;
}
