import java.util.Random;

class Driver implements Runnable {
  private String name;
  private int age;
  private KartTrack kartTrack;
  private long waitTime;

  public Driver(String name, int age, KartTrack kartTrack) {
      this.name = name;
      this.age = age;
      this.kartTrack = kartTrack;
  }

  @Override
  public void run() {
      long arrivalTime = System.currentTimeMillis();

      System.out.println("Piloto: " + name + "; Idade: " + age + " chegou na pista.");

      if (age < 18) {
          if (age < 14) {
              kartTrack.acquirePriorityHelmet(this);
          } else {
              kartTrack.acquireCommonHelmet(this);
          }
          kartTrack.acquireKart(this);
      } else {
          kartTrack.acquireKart(this);
          kartTrack.acquireCommonHelmet(this);
      }

      waitTime = (System.currentTimeMillis() - arrivalTime) / 1000;
      kartTrack.accumulateWaitTime(waitTime);

      System.out.println("Piloto: " + name + "; Tempo de espera: " + waitTime + " segundos.");
      
      System.out.println("Piloto: " + name + "; Idade: " + age + " está na pista.");


      try {
          Thread.sleep(new Random().nextInt(2000) + 1000); // Simula o tempo da corrida
      } catch (InterruptedException e) {
          e.printStackTrace();
      }

      kartTrack.releaseHelmet();
      kartTrack.releaseKart();

      System.out.println("Piloto: " + name + "; Idade: " + age + " deixou a pista.");

  }

  public int getAge() {
      return age;
  }

  public long getWaitTime() {
      return waitTime;
  }
}