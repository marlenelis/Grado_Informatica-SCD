import monitor.*;
import java.util.Random;
/**
 * fumadores.java
 * @author Marlene E. Vásquez Calero
 * @version 1.0
 * @date 07-nov-2016
 **/

//----------------------------------------------------------------------------
class Estanco extends AbstractMonitor {
    
    private final Condition[] fumadores ;//condition fumadores
    private final Condition estanquero;//condition estanquero
    private int fumador;    //vble para comprobar que fumador esta dentro del monitor
  
    public Estanco(){
        enter();
            this.fumadores = new Condition[3];
            this.estanquero = makeCondition();
            this.fumador = -1;
            for(int i =0 ; i<3 ; i++)
                this.fumadores[i]= makeCondition();
        leave();
    }
    //----------------------------------------------------------------------------
    // invocado por cada fumador indicando su ingrediente o numero
    //----------------------------------------------------------------------------
    public void obtenerIngrediente(int ingrediente){
        enter();
        String aux="";
            //comprueba si el fumador se correspoende con el ingrediente
            if(this.fumador != ingrediente)//si son distintos
                this.fumadores[ingrediente].await(); // el fumador espera
            switch(ingrediente){
                case 0: aux = "Cerillas";break;
                case 1: aux = "Tabaco";break;
                case 2: aux = "Papel";break;   
            }
            System.out.println("\033[31mFumador\033[0m["+(this.fumador+1)+"]: obteniendo "+ aux );
            this.fumador = -1;   // vble cambiamos su estado  
            this.estanquero.signal(); //despierta al estanquero
        leave();
    }
    //----------------------------------------------------------------------------
    //invocado por el estanquero indicando el ingrediente que pone 
    //----------------------------------------------------------------------------
    public void ponerIngrediente(int ingrediente){
        enter();
        String aux="";
            this.fumador = ingrediente;      
            switch(ingrediente){
                case 0: aux = "Cerillas";break;
                case 1: aux = "Tabaco";break;
                case 2: aux = "Papel";break;   
            }
            System.out.println("\033[36mEstanquero\033[0m: colocando "+ aux );
            this.fumadores[ingrediente].signal();//despierta al fumador que se corresponde con el ingrediente generado
        leave();        
    }
    //----------------------------------------------------------------------------
    //invocado por el estanquero
    //----------------------------------------------------------------------------
    public void esperarRecogidaIngrediente(){
        enter();
            if(this.fumador != -1) this.estanquero.await();  
            System.out.println("\033[32mIngrediente recogido!!\033[0m\n" );
        leave();
    }
}
//----------------------------------------------------------------------------
class Estanquero implements Runnable{

    public Thread thr;
    Estanco estanco;
    
    public Estanquero(Estanco est){
        this.estanco = est;
        this.thr = new Thread(this, "Estanquero");
    }
    
    @Override
    public void run() {
        try{ 
            int ingrediente;
            while(true){
                ingrediente =(int) (Math.random()*3.0);//0,1 o 2
                this.estanco.ponerIngrediente(ingrediente); 

                this.estanco.esperarRecogidaIngrediente();
            }
        }catch( Exception e ) { System.err.println("Excepcion en run Estanquero: " + e);} 
    }
}
//----------------------------------------------------------------------------
class Fumador implements Runnable {
    int miIngrediente;
    public Thread thr;
    Estanco estanco;
    
    public Fumador (int mi_Ingrediente, Estanco est){
        this.estanco = est;
        this.miIngrediente = mi_Ingrediente;
        String nombre ="";
        switch(this.miIngrediente){
            case 0: nombre="Fumador 1";break;
            case 1: nombre="Fumador 2";break;
            case 2: nombre="Fumador 3";break;   
        }
        this.thr = new Thread(this,nombre);
    }

    @Override
    public void run() {
        try{ 
            while(true){
               this.estanco.obtenerIngrediente(this.miIngrediente);
               aux.dormir_max(2000);               
           }
        }catch( Exception e ) { System.err.println("Excepcion en run Fumador: " + e);} 
    }
}
//------------------------------------------------------------------------
public class fumadores {
   
    public static void main(String[] args) {
        
        Estanco estanco = new Estanco();
        Thread estanquero = new Thread(new Estanquero(estanco));
        estanquero.start();
        
        Thread[] fumador = new  Thread[3];
        for(int i = 0; i < fumador.length; i++){
            fumador[i] = new Thread(new Fumador(i, estanco));
            fumador[i].start();
        }    
    }
}
