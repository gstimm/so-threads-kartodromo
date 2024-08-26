import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Main {
    private static final int NUM_GROUPS = 3;
    private static final int MIN_DRIVERS_GROUP = 15;
    private static final int MAX_DRIVERS_GROUP = 30;

    public static void main(String[] args) {
        Random rand = new Random();
        KartTrack kartTrack = new KartTrack();

        for (int i = 0; i < NUM_GROUPS; i++) {
            int numDrivers = MIN_DRIVERS_GROUP + rand.nextInt(MAX_DRIVERS_GROUP - MIN_DRIVERS_GROUP + 1);
            System.out.println("\n Grupo " + (i + 1) + " com " + numDrivers + " pilotos chegou.");

            List<Thread> threads = new ArrayList<>();
            for (int j = 0; j < numDrivers; j++) {
                String name = "Piloto" + (j + 1);
                int age = rand.nextInt(45) + 5; //Idades entre 5 e 50 anos
                Driver driver = new Driver(name, age, kartTrack);
                Thread thread = new Thread(driver);
                threads.add(thread);
                thread.start();
            }

            for (Thread thread : threads) {
                try {
                    thread.join();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }

        kartTrack.printReport();
    }
}