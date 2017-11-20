

public class TheBar {
	public static void main(String[] args) {
        if (args.length != 1) {
            printUsage();
        }

        int num = 0;
        try {
            num = Integer.parseInt(args[0]);
        }
        catch (NumberFormatException e) {
            printUsage();
        }

        System.out.println("Customer:\t\t\t\t\t\t\t\t\t\t\t| Employee:");
        System.out.print("Traveling\tArrived\t\tOrdering\tBrowsing\tAt Register\tLeaving");
        System.out.println("\t\t| Waiting\tMixing Drinks\t\tAt Register\tPayment Accepted");
        System.out .println("---------------------------------------------------"
                        + "---------------------------------------------+--------"
                        + "-------------------------------------------------------------------");

        Thread emp = new Bartender();
        emp.start();

        Thread[] custs = new Thread[num];
        for (int i = 0; i < num; i++) {
            custs[i] = new Customer(i);
            custs[i].start();
        }
        for (int i = 0; i < num; i++) {
            try {
                custs[i].join();
            }
            catch (InterruptedException e) {
            }
        }

        System.exit(0);
    }

    private static void printUsage() {
        System.out.println("Usage: java TheBar <num>");
        System.out.println("  <num>: Number of customers.");
        System.exit(-1);
    }

}


