import java.math.BigDecimal;
import java.math.RoundingMode;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

class KartTrack {
  private int availableKarts = 10;
  private int availableHelmets = 10;
  private int childrenUnder14 = 0;
  private int totalCustomersServed = 0;
  private long totalWaitTime = 0;

  private final Lock lockKarts = new ReentrantLock();
  private final Lock lockHelmets = new ReentrantLock();
  private final Condition condKarts = lockKarts.newCondition();
  private final Condition condHelmets = lockHelmets.newCondition();

  public void acquireKart(Driver driver) {
      lockKarts.lock();
      try {
          while (availableKarts == 0) {
              condKarts.await();
          }
          availableKarts--;
          totalCustomersServed++;
      } catch (InterruptedException e) {
          e.printStackTrace();
      } finally {
          lockKarts.unlock();
      }
  }

  public void acquireCommonHelmet(Driver driver) {
      lockHelmets.lock();
      try {
          while (availableHelmets == 0 || childrenUnder14 > 0) {
              condHelmets.await();
          }
          availableHelmets--;
      } catch (InterruptedException e) {
          e.printStackTrace();
      } finally {
          lockHelmets.unlock();
      }
  }

  public void acquirePriorityHelmet(Driver driver) {
      lockHelmets.lock();
      try {
          childrenUnder14++;
          while (availableHelmets == 0) {
              condHelmets.await();
          }
          availableHelmets--;
          childrenUnder14--;
      } catch (InterruptedException e) {
          e.printStackTrace();
      } finally {
          lockHelmets.unlock();
      }
  }

  public void releaseKart() {
      lockKarts.lock();
      try {
          availableKarts++;
          condKarts.signal();
      } finally {
          lockKarts.unlock();
      }
  }

  public void releaseHelmet() {
      lockHelmets.lock();
      try {
          availableHelmets++;
          condHelmets.signal();
      } finally {
          lockHelmets.unlock();
      }
  }

  public void accumulateWaitTime(long waitTime) {
      lockKarts.lock();
      try {
          totalWaitTime += waitTime;
      } finally {
          lockKarts.unlock();
      }
  }

  public void printReport() {
      double averageWaitTime = totalCustomersServed > 0 ? (double) totalWaitTime / totalCustomersServed : 0;
      double truncatedAverageWaitTime = BigDecimal
          .valueOf(averageWaitTime)
          .setScale(3, RoundingMode.HALF_UP)
          .doubleValue();

      System.out.println("\nRelatório Final da Pista de Kart:\n");
      System.out.println("Total de Clientes Atendidos: " + totalCustomersServed);
      System.out.println("Tempo Total de Espera: " + totalWaitTime + " segundos");
      System.out.println("Tempo Médio de Espera: " + truncatedAverageWaitTime + " segundos");
      System.out.println("Karts Disponíveis: " + availableKarts);
      System.out.println("Capacetes disponíveis: " + availableHelmets);
  }
}