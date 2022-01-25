// Classe do monitor (objeto compartilhado)
class LE {
    private int leit, escr; // quantidade de leitores e escritores
    static int num = 0;

    public LE() {
        this.leit = 0;
        this.escr = 0;
    }

    public synchronized void entraLeitor (int id) {
        try {
            while (this.escr > 0) {
                System.out.println ("Monitor: leitor bloqueado ("+id+")");
                wait(); //bloqueia pela condicao logica da aplicacao
            }
            this.leit++; //registra que ha mais um leitor lendo
            System.out.println ("Monitor: leitor lendo ("+id+")");
        } catch (InterruptedException e) { }
    }

    public synchronized void saiLeitor (int id) {
        this.leit--; //registra que um leitor saiu
        if (this.leit == 0)
            this.notify(); //libera escritor (caso exista escritor bloqueado)
        System.out.println ("Monitor: leitor saindo ("+id+")");
    }

    public synchronized void entraEscritor (int id) {
        try {
            while ((this.leit > 0) || (this.escr > 0)) {
                //if ((this.leit > 0) || (this.escr > 0)) {
                System.out.println ("Monitor: escritor bloqueado ("+id+")");
                wait(); //bloqueia pela condicao logica da aplicacao
            }
            this.escr++; //registra que ha um escritor escrevendo
            System.out.println ("Monitor: escritor escrevendo ("+id+")");
        } catch (InterruptedException e) { }
    }

    public synchronized void saiEscritor (int id) {
        this.escr--; //registra que o escritor saiu
        notifyAll(); //libera leitores e escritores (caso existam leitores ou escritores bloqueados)
        System.out.println ("Monitor: escritor saindo ("+id+")");
    }
}


class Leitor extends Thread {
    private int id; //identificador da thread
    private LE monitor;//objeto monitor para coordenar a lógica de execução das threads

    public Leitor (int id, LE m) {
        this.id = id;
        this.monitor = m;
    }

    public static String parOuImpar(int n) {
        return n % 2 == 0 ? "par" : "ímpar";
    }

    @Override
    public void run () {
        while (true) {
            this.monitor.entraLeitor(this.id);
            System.out.printf("Leitor (%d) --- %d é %s\n", this.id, LE.num, parOuImpar(LE.num));
            this.monitor.saiLeitor(this.id);
            try {
                sleep(700);
            }
            catch (InterruptedException e) { }
        }
    }
}


class Escritor extends Thread {
    private int id; //identificador da thread
    private LE monitor; //objeto monitor para coordenar a lógica de execução das threads

    public Escritor (int id, LE m) {
        this.id = id;
        this.monitor = m;
    }

    @Override
    public void run () {
        while (true) {
            this.monitor.entraEscritor(this.id);
            LE.num = this.id;
            System.out.printf("Escritor (%d) escreveu %d\n", this.id, this.id);
            this.monitor.saiEscritor(this.id);
            try {
                sleep(700);
            }
            catch (InterruptedException e) { }
        }
    }
}


class LeitorEscritor extends Thread {
    private int id;
    private LE monitor;

    public LeitorEscritor(int id, LE m) {
        this.id = id;
        this.monitor = m;
    }

    @Override
    public void run () {
        while (true) {
            this.monitor.entraLeitor(this.id);
            System.out.printf("LeitorEscritor (%d) --- %d é %s\n", this.id, LE.num, Leitor.parOuImpar(LE.num));
            this.monitor.saiLeitor(this.id);

            // processamento adicional
            try {
                sleep(500);
            }
            catch (InterruptedException e) { }

            this.monitor.entraEscritor(this.id);
            LE.num = this.id;
            System.out.printf("Escritor (%d) escreveu %d\n", this.id, this.id);
            this.monitor.saiEscritor(this.id);
        }
    }
}


public class Laboratorio8 {
    static final int LEITORES = 4;
    static final int ESCRITORES = 3;
    static final int LEIT_ESCR = 2;

    public static void main(String[] args) {
        LE monitor = new LE();
        Leitor[] leitores = new Leitor[LEITORES];
        Escritor[] escritores = new Escritor[ESCRITORES];
        LeitorEscritor[] leit_escr = new LeitorEscritor[LEIT_ESCR];

        for (int i = 0 ; i < LEITORES ; i++) {
            leitores[i] = new Leitor(i, monitor);
            leitores[i].start();
        }

        for (int i = 0 ; i < ESCRITORES ; i++) {
            escritores[i] = new Escritor(i, monitor);
            escritores[i].start();
        }

        // comeca em 100 para diferenciar os IDs das outras threads
        for (int i = 100 ; i < 100 + LEIT_ESCR ; i++) {
            leit_escr[i - 100] = new LeitorEscritor(i, monitor);
            leit_escr[i - 100].start();
        }
    }
}
