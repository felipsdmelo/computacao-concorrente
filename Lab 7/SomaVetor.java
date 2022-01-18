import java.util.ArrayList;
import java.util.Random;

class Compartilha {
    private int soma;
    private ArrayList<Integer> v;
    private int nThreads;

    public Compartilha(ArrayList<Integer> v, int nThreads) {
        this.v = v;
        this. nThreads = nThreads;
        this.soma = 0;
    }

    public synchronized int getSoma() {
        return this.soma;
    }

    public synchronized void incrementaSoma(int inc) {
        this.soma += inc;
    }

    public ArrayList<Integer> getV() {
        return this.v;
    }

    public int getNThreads() {
        return this.nThreads;
    }

    public void setNThreads(int nThreads) {
        this.nThreads = nThreads;
    }
}

class T extends Thread {
    private Compartilha objeto;
    private int id;

    public T(int id, Compartilha obj) {
        this.id = id;
        this.objeto = obj;
        // this.objeto.setV(obj.getV());
        // this.objeto.setNThreads(obj.getNThreads());
    }

    @Override
    public void run() {
        int tamBloco = objeto.getV().size() / objeto.getNThreads();
        int inicio = this.id * tamBloco;
        int fim;

        if (this.id == objeto.getNThreads() - 1)    // ultima thread
            fim = objeto.getV().size();
        else                                       // demais threads
            fim = inicio + tamBloco;

        for (int i = inicio ; i < fim ; i++)
            objeto.incrementaSoma(objeto.getV().get(i));
    }
}

class SomaVetor {
    static final int N = 4; // numero de threads
    static final int TAM = 1000000; // tamanho do vetor

    public static void preencheVetor(ArrayList<Integer> vet) {
        Random rand = new Random();
        for (int i = 0 ; i < TAM ; i++)
            vet.add(rand.nextInt(101)); // 0 <= x <= 100
    }

    public static int somaSequencial(ArrayList<Integer> vet) {
        int soma = 0;
        for (int i = 0 ; i < vet.size() ; i++)
            soma += vet.get(i);
        return soma;
    }

    public static void main(String[] args) {
        Thread[] threads = new Thread[N];
        ArrayList<Integer> vet = new ArrayList<>(TAM);
        Compartilha objeto = new Compartilha(vet, N);

        preencheVetor(vet);

        // cria as threads
        for (int i = 0 ; i < N ; i++) {
            threads[i] = new T(i, objeto);
        }

        // inicia as threads
        for (int i = 0 ; i < N ; i++)
            threads[i].start();

        // aguarda o fim das threads
        for (int i = 0 ; i < N ; i++) {
            try {
                threads[i].join();
                // soma += threads[i].getSoma();
            }
            catch (InterruptedException e) {
                return;
            }
        }

        if (TAM <= 15) {
            System.out.println(vet);
            System.out.println();
        }

        if (objeto.getSoma() == somaSequencial(vet))
            System.out.println("A soma foi realizada corretamente");
        else
            System.out.println("A soma não foi realizada corretamente");
        System.out.printf("\nSoma do vetor: %d\n", objeto.getSoma());
    }
}
