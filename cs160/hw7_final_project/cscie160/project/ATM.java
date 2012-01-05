package cscie160.project;
import java.rmi.*;
import java.util.*;

/**
 * Interface for an ATM.
 */
public interface ATM extends Remote
{
    // Method Members
    public void  deposit(AccountInfo account, float amount) throws ATMException, RemoteException;
    public void  withdraw(AccountInfo account, float amount) throws ATMException, NSFException, RemoteException;
    public Float getBalance(AccountInfo account) throws ATMException, RemoteException;
    public void  transfer(AccountInfo fromAccount, AccountInfo toAccount, float amount) throws ATMException, NSFException, RemoteException;
    public void  addObserver(ATMListener o) throws RemoteException;
}
