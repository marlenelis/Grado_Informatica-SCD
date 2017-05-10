import monitor.* ;
import java.util.Random;
/**
 * prod-cons.java
 * @author Marlene E. Vásquez Calero
 * @version 1.0
 * @date 06-nov-2016
 */

//===============================================================================================
class Buffer extends AbstractMonitor
{ 
    private int numSlots = 0, 
                cont = 0;   
    private double[] buffer = null;  
  // Utilizamos 2 colas para gestionar los productores y los consumidores:  
    private final Condition consumir ;
    private final Condition producir ;
  
  public Buffer( int p_numSlots ) 
  { 
    this.consumir = makeCondition();
    this.producir = makeCondition();
    this.numSlots = p_numSlots ; 
    this.buffer = new double[this.numSlots] ;
  }
  
  // Cuando el buffer esté por el final el proceso depositar pasa a la cola de producir,
  // incrementamos el indice y liberamos un proceso de la cola de consumir.
  
  public void depositar( double valor ){ 
    enter();
        if( this.cont == this.numSlots) this.producir.await();              
        this.buffer[this.cont] = valor; 
        this.cont++;      
        this.consumir.signal();
    leave();
  }
  
  // Si el buffer está a 0 elementos el proceso extraer pasa a la cola de consumir,
  // se decrementa el indice del buffer y liberamos un proceso de la cola de producir.
  
  public double extraer() {
    double valor;
    enter();
        if( this.cont == 0 ) this.consumir.await() ;
        this.cont--; 
        valor = this.buffer[this.cont] ;
        this.producir.signal();
    leave();
    return valor;//devuelve valor
  }
}
 //====================================================================

class Productor implements Runnable { 
    private Buffer bb ;//monitor
    int veces; 
    int numP ;
    Thread thr ;
    public Productor( Buffer pbb, int pveces, int pnumP ){
        bb    = pbb;
        veces = pveces;
        numP  = pnumP ;
        thr   = new Thread(this,"productor "+numP);
    }
    public void run(){
        try{ 
            double item = 100*numP ;
            for( int i=0 ; i<veces ; i++ ){ 
                System.out.println(thr.getName()+", produciendo " + item);
                bb.depositar( item++ );
            }
        }
        catch( Exception e ){
            System.err.println("Excepcion en productor: " + e);
        }
    }
}
//====================================================================
class Consumidor implements Runnable 
{ private Buffer bb ;
  int veces; 
  int numC ;
  Thread thr ;
  public Consumidor( Buffer pbb, int pveces, int pnumC ) 
  { bb    = pbb;
    veces = pveces;
    numC  = pnumC ;
    thr   = new Thread(this,"consumidor "+numC);
  }
  public void run() 
  { try 
    { for( int i=0 ; i<veces ; i++ )
      { double item = bb.extraer ();
        System.out.println(thr.getName()+", consumiendo "+item);
      } 
    }
    catch( Exception e ) 
    { System.err.println("Excepcion en consumidor: " + e);
    }
  }
}
//====================================================================
public class prodcons {

    public static void main(String[] args) {
        // leer parametros, crear vectores y buffer intermedio
    Consumidor[] cons = new Consumidor[20] ;
	  Productor[]  prod = new Productor[10] ;
	  Buffer buffer = new Buffer(5);
	  int iter_cons = 2;
	  int iter_prod = 4;
	  if ( cons.length*iter_cons != prod.length*iter_prod )
	  { System.err.println("no coinciden número de items a producir con a cosumir");
      return ;
    }    
	  // crear hebras
	  for(int i = 0; i < cons.length; i++) 
	    cons[i] = new Consumidor(buffer,iter_cons,i) ;
	  for(int i = 0; i < prod.length; i++)
	    prod[i] = new Productor(buffer,iter_prod,i) ;
	  // poner en marcha las hebras
	  for(int i = 0; i < prod.length; i++) prod[i].thr.start();
	  for(int i = 0; i < cons.length; i++) cons[i].thr.start();
    }
    
}
