import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class SP23_BSE_106 {
    
    // Thread Class
    public static class MultiThreading implements Runnable {
        private final int opponent;
        private final int i;
        private final int[] balances;
        private final int cutOffValue;

        // For Moving Data
        public MultiThreading(int i, int[] balances, int cutOffValue, int opponent) {
            this.i = i;
            this.balances = balances;
            this.cutOffValue = cutOffValue;
            this.opponent = opponent;
        }

	//MUltiThreading Task Handler
        @Override
        public void run() {
            // Determine the bet amount
            int betAmount = Math.min(balances[i], balances[opponent]) / 5;
            // Flip the coin and determine the winner
            Random rand = new Random();
            boolean coinFlip = rand.nextBoolean();
            if (coinFlip) {
                balances[i] += betAmount;
                balances[opponent] -= betAmount;
            } else {
                balances[i] -= betAmount;
                balances[opponent] += betAmount;
            }

            Thread currentThread = Thread.currentThread();
            String threadName = currentThread.getName();
            long time = System.currentTimeMillis();

            // Print the results of the bet
            System.out.println("Thread " + threadName + " is going to start game at " + time);
            System.out.println("Player " + i + " (" + balances[i] + ") vs Player " + opponent + " (" + balances[opponent] + ")");
        }
    }

    //Main Method
    public static void main(String[] args) {
        if (args.length != 4) {
            System.out.println("Please provide number_of_Players initital_balance cut_Off_Value zakat_on_off");
            System.exit(1);
        }

        // Parse command line arguments
        int numPlayers = Integer.parseInt(args[0]);
        int initialBalance = Integer.parseInt(args[1]);
        int cutOffValue = Integer.parseInt(args[2]);
        int zakat = Integer.parseInt(args[3]);
        int rounds = 0;

        // Get the number of available CPU cores
        int numThreads = Runtime.getRuntime().availableProcessors();
        System.out.println("Total No Of Threads : " + numThreads);

        // Create a fixed thread pool
        ExecutorService executor = Executors.newFixedThreadPool(numThreads);

        // Initialize player balances
        int[] balances = new int[numPlayers];
        for (int i = 0; i < numPlayers; i++) {
            balances[i] = initialBalance;
        }

        Random rand = new Random();

        // Main game loop
        while (numPlayers > 1) {
            rounds++;
            System.out.println("\n--- Round No " + rounds + " ---");

            // Schedule matches in parallel using the thread pool
            for (int i = 0; i < numPlayers; i++) {
                if (balances[i] < cutOffValue) {
                	continue; // Skip eliminated players
                }

                int opponent;
                do {
                    opponent = rand.nextInt(numPlayers);
                } while (opponent == i || balances[opponent] < cutOffValue);

                // Submit a match task to the thread pool
                executor.execute(new MultiThreading(i, balances, cutOffValue, opponent));
            }

            // Simulate round completion delay (to allow all matches to finish)
            try {
                Thread.sleep(800);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }

            // Eliminate players
            int remaining = 0;
            int[] newBalances = new int[numPlayers];

            for (int i = 0; i < numPlayers; i++) {
                if (balances[i] >= cutOffValue) {
                    newBalances[remaining++] = balances[i];
                } else {
                    System.out.println("Player " + i + " eliminated with balance " + balances[i]);
                }
            }

            numPlayers = remaining;
            balances = newBalances;
        }

        // Shut down the executor
        executor.shutdown();

        System.out.printf("Game finished after %d rounds\n", rounds);
    }
}

