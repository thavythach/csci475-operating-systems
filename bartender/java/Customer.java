
public class Customer extends Thread {

    private int id;

    public Customer(int id) {
        this.id = id;
    }

    public void run() {
        travelToBar();
        arriveAtBar();
        placeOrder();
        browseArt();
        atRegister();
        leaveBar();
    }

    private void travelToBar() {
        System.out.println("Customer " + id + "\t\t\t\t\t\t\t\t\t\t\t|");
    }

    private void arriveAtBar() {
        System.out.println("\t\tCustomer " + id + "\t\t\t\t\t\t\t\t\t|");
    }

    private void placeOrder() {
        System.out.println("\t\t\t\tCustomer " + id + "\t\t\t\t\t\t\t|");
    }

    private void browseArt() {
        System.out.println("\t\t\t\t\t\tCustomer " + id + "\t\t\t\t\t|");
       
    }

    private void atRegister() {
        System.out.println("\t\t\t\t\t\t\t\tCustomer " + id + "\t\t\t|");
    }

    private void leaveBar() {
        System.out.println("\t\t\t\t\t\t\t\t\t\tCustomer " + id + "\t|");
    }
}
