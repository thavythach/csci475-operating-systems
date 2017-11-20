
public class Bartender extends Thread {

    public void run() {
        while (true) {
            waitForCustomer();
            mixDrink();
            atCashRegister();
            paymentAccepted();
        }
    }

    private void waitForCustomer() {
        System.out.println("\t\t\t\t\t\t\t\t\t\t\t\t| Bartender");
    }

    private void mixDrink() {
        System.out.println("\t\t\t\t\t\t\t\t\t\t\t\t| \t\tBartender");
    }

    private void atCashRegister() {
        System.out.println("\t\t\t\t\t\t\t\t\t\t\t\t| \t\t\t\t\tBartender");
    }

    private void paymentAccepted() {
        System.out.println("\t\t\t\t\t\t\t\t\t\t\t\t| \t\t\t\t\t\t\tBartender");
    }
}